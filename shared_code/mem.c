global memory_block_node CRT_memory_sentinel = {0};

global void* crt_reserve_memory(u64 size) {
    // Ensure that a block header never changes an allocations alignment
    Assert(sizeof(memory_block_node) == 64); 

    if (!CRT_memory_sentinel.next) {
        DLLInit(&CRT_memory_sentinel);
    }

    u64 total_size = size + sizeof(memory_block_node);
    u64 base_offset = sizeof(memory_block_node);

    void* block = malloc(total_size);
    memory_block_node* block_node = (memory_block_node*)block;
    block_node->memory_base = (u8*)block + base_offset;

    memory_block_node* first_node = &CRT_memory_sentinel;
    DLLPushFront(first_node, block_node);
    block_node->size = size;
    block_node->total_block_size = total_size;

    void* result = block_node->memory_base;
    return result;
}

global void* crt_commit_memory(void* ptr, u64 size) {
    void* result = ptr;
    size;
    return result;
}

global void* crt_release_memory(void* ptr, u64 size) {
    size;
    memory_block_node* block = (memory_block_node*)((u8*)ptr - sizeof(memory_block_node));
    if (block->memory_base) {
        DLLRemove(block);
        free((u8*)block);
    }
    return 0;
}

global memory_funcs* global_crt_memory() {
    global memory_funcs crt_memory_result = {0};
    if (crt_memory_result.reserve == 0) {
        crt_memory_result.reserve = crt_reserve_memory;
        crt_memory_result.commit = crt_commit_memory;
        crt_memory_result.release = crt_release_memory;
    }
    return &crt_memory_result;
}

///

global memory_arena arena_create(memory_funcs* backing_funcs) {
    memory_arena result = {0};
    memory_block_node* block = (memory_block_node*)
        ((u8*)backing_funcs->reserve(DEFAULT_RESERVE_SIZE) - sizeof(memory_block_node));
    result.block = block;
    result.funcs = backing_funcs;
    return result;
}

global void* arena_push_size(memory_arena* arena, u64 size) {
    void* result = 0;
    memory_funcs* funcs = arena->funcs;

    if ((arena->block->pos + size) > arena->block->size) {
        u64 reserve_size = Maximum(size, DEFAULT_RESERVE_SIZE);
        reserve_size = AlignPow2(size, reserve_size);
        
        memory_block_node* new_block = (memory_block_node*)
            ((u8*)funcs->reserve(reserve_size) - sizeof(memory_block_node));
        new_block->prev_arena = arena->block;
        arena->block = new_block;
        arena->block->pos = 0;
    }

    result = (void*)(arena->block->memory_base + arena->block->pos);
    arena->prev_pos = arena->block->pos;
    arena->block->pos += size;
    
    if ((arena->block->pos + size) > arena->committed) {
        u64 commit_size = AlignPow2(arena->block->pos + size, DEFAULT_COMMIT_SIZE);
        commit_size = Minimum(commit_size, arena->block->size);

        if (arena->funcs->commit(arena->block->memory_base + arena->committed, commit_size)) {
            arena->committed += commit_size;
        }
    }

    return result;
}

global void* arena_push_copy(memory_arena* arena, u64 size, void* source) {
    void* result = arena_push_size(arena, size);
    memcpy(result, (void*)source, size);
    return result;
}

global void arena_pop(memory_arena* arena, u64 size) {
    memory_funcs* funcs = arena->funcs;
    funcs;

    u64 total_size = 0;
    u64 pop_pos = 0;
    for (memory_block_node* node = arena->block; node; node = node->prev_arena) {
        total_size += node->size; 
    }
    pop_pos = total_size - size;

    if ((size - arena->block->size) != 0) {
        for (memory_block_node* node = arena->block; node; node = node->prev_arena) {
            if ((total_size - node->size) < pop_pos) { // We hit the limit of free-able memory blocks.
                break;
            }
            u64 aligned_pos = AlignPow2(size, DEFAULT_COMMIT_SIZE);
            aligned_pos = Minimum(aligned_pos, node->size);

            memory_block_node* temp_node = node->next;
//            funcs->release(node->memory_base, aligned_pos);
//            DLLRemove(node);
            arena->block = temp_node;
            total_size -= aligned_pos;
        }
    }

    if (total_size > size) {
        u64 aligned_pos = AlignPow2(size, DEFAULT_COMMIT_SIZE);
        u64 next_size = Minimum(aligned_pos, arena->block->size);
        u64 free_amnt = total_size - next_size;
        u64 pop_size = aligned_pos - next_size;
        pop_size;
        free_amnt;
#if 0
        if (0) {
 //           arena->funcs->release(arena->memory + arena->committed, arena->committed - aligned_pos);
            Breakpoint;
            arena->pos = size;
            arena->committed = aligned_pos;
        }
#endif
    }
}
