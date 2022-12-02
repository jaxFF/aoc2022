#ifndef AOC_MEMORY_H
#define AOC_MEMORY_H

global void* memory_realloc_stub(void* ptr, u64 size) { return ptr; size; }
global void memory_decommit_stub(void* ptr, u64 size) { 0; }

typedef void* memory_reserve_func(u64 size);
typedef void* memory_realloc_func(void* ptr, u64 size);
typedef void memory_release_func(void* ptr, u64 size);

typedef struct memory_funcs memory_funcs;
struct memory_funcs {
    memory_reserve_func* reserve;
    memory_realloc_func* commit;
    memory_release_func* decommit;
    memory_release_func* release;
};

memory_funcs* global_crt_memory();

typedef struct memory_block_node memory_block_node;
struct memory_block_node {
    _Alignas(max_align_t) 
    u8* memory_base; // Base ptr from which we write memory to
    u64 size;
    u64 total_block_size; // Including this node
    u64 pos; // Arena-specific bookkeeping
    u8* __pad[1];
    memory_block_node* next; 
    memory_block_node* prev;
    // If an arenas' alloc position is pushed back far enough, we can walk back to
    //  the previous block through this pointer.
    memory_block_node* arena_prev; // Arena-specific bookkeeping
};

// Arena Allocation stuff:
//  An arena is fundamentally just a 'stack' of dynamic memory that we continually 
//   push to, and pop from in a linear fashion.

// TODO: Handle commiting of contigous virtually-backed physical pages from the OS!?
// TODO: Implement xx_resize functionality

typedef struct memory_arena memory_arena;
struct memory_arena {
    memory_block_node* block;
    u64 prev_pos;

    memory_funcs* funcs;
};

// MB should force Windows to use not the heap, but directly VirtualAlloc to request memory. I cannot guarantee this behaviour.
#define DEFAULT_RESERVE_SIZE Kilobytes(4)//Megabytes(32)//Kilobytes(32)
#define DEFAULT_COMMIT_SIZE DEFAULT_RESERVE_SIZE

// Function overloading scheme cause C kinda sucks sometimes. This trick counts
//  the number of arguments to the macro, then expands the macro to take them.
#define arena_create0() arena_create_()
#define arena_create1(_1) arena_create__(_1)
#define __overload_sel__arena_create(func, _1, count, ...) count

#define arena_create(...) __overload_sel__arena_create(func, ##__VA_ARGS__, arena_create1, arena_create0)(__VA_ARGS__)
memory_arena arena_create_();
memory_arena arena_create__(memory_funcs* backing_funcs);

#define arena_push_struct(arena, type) (type*)arena_push_size(arena, sizeof(type))
#define arena_push_array(arena, count, type) (type*)arena_push_size(arena, (count)*sizeof(type))
void* arena_push_size(memory_arena* arena, u64 size);
void* arena_push_copy(memory_arena* arena, u64 size, void* source);
#define arena_resize_array(arena, memory, prev_count, count, type) \
    (type*)arena_resize(arena, memory, (prev_count) * sizeof(type), (count) * sizeof(type))
void* arena_resize(memory_arena* arena, void* target_memory, u64 prev_size, u64 size);
void arena_pop_to_pos(memory_arena* arena, u64 pos);
void arena_pop(memory_arena* arena, u64 size);

#endif
