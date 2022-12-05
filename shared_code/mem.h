#ifndef AOC_MEMORY_H
#define AOC_MEMORY_H

typedef void* memory_reserve_func(u64 size);
typedef void* memory_realloc_func(void* ptr, u64 size);

typedef struct memory_funcs memory_funcs;
struct memory_funcs {
    memory_reserve_func* reserve;
    memory_realloc_func* commit;
    memory_realloc_func* release;
};

global memory_funcs* global_crt_memory();

typedef struct memory_block_node memory_block_node;
struct memory_block_node {
    _Alignas(max_align_t) 
    u8* memory_base; // Base ptr from which we write memory to
    u64 size;
    u64 total_block_size; // Including this node
    u64 pos;
    u8* __pad[1];
    memory_block_node* next; 
    memory_block_node* prev;
    // If an arenas' alloc position is pushed back far enough, we can walk back to
    //  the previous block through this pointer.
    memory_block_node* prev_arena; 
};

// Arena Allocation stuff:
//  An arena is fundamentally just a 'stack' of dynamic memory that we continually 
//   push to, and pop from in a linear fashion.

typedef struct memory_arena memory_arena;
struct memory_arena {
    memory_block_node* block;
    u64 prev_pos;
    u64 committed;

    memory_funcs* funcs;
};

#define DEFAULT_RESERVE_SIZE Kilobytes(2)//Kilobytes(32)
#define DEFAULT_COMMIT_SIZE DEFAULT_RESERVE_SIZE

global memory_arena arena_create(memory_funcs* backing_funcs);

#define arena_push_struct(arena, type) (type*)arena_push_size(arena, sizeof(type))
#define arena_push_array(arena, count, type) (type*)arena_push_size(arena, (count)*sizeof(type))
global void* arena_push_size(memory_arena* arena, u64 size);
global void* arena_push_copy(memory_arena* arena, u64 size, void* source);
#define arena_resize_array(arena, memory, prev_count, count, type) \
    (type*)arena_resize(arena, memory, (prev_count) * sizeof(type), (count) * sizeof(type))
global void* arena_resize(memory_arena* arena, void* target_memory, u64 prev_size, u64 size);
global void arena_pop_back(memory_arena* arena, u64 pos);
global void arena_pop(memory_arena* arena, u64 size);

#endif
