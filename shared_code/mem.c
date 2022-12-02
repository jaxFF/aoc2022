global memory_block_node CRT_memory_sentinel = {0};

void* crt_reserve_memory(u64 size) {
    // Ensure that a block header never changes an allocations alignment
    Assert(sizeof(memory_block_node) == 64); 

    if (!CRT_memory_sentinel.next) {
        DLLInit(&CRT_memory_sentinel);
    }

    u64 total_size = size + sizeof(memory_block_node);
    u64 base_offset = sizeof(memory_block_node);

    void* block = malloc(total_size);
    memset(block, 0x00, total_size);
    memory_block_node* block_node = (memory_block_node*)block;
    block_node->memory_base = (u8*)block + base_offset;

    memory_block_node* first_node = &CRT_memory_sentinel;
    DLLPushFront(first_node, block_node);
    block_node->size = size;
    block_node->total_block_size = total_size;

    void* result = block_node->memory_base;
    return result;
}

void crt_release_memory(void* ptr, u64 size) {
    size;
    memory_block_node* block = (memory_block_node*)((u8*)ptr - sizeof(memory_block_node));
    Assert(block);
    u64 block_ptr_size = block->size;
    if (block->memory_base) {
        DLLRemove(block);
        free((u8*)block);
        memset(block, 0x00, size);
        block = 0;
    }
}

memory_funcs* global_crt_memory() {
    global memory_funcs crt_memory_result = {0};
    if (crt_memory_result.reserve == 0) {
        crt_memory_result.reserve = crt_reserve_memory;
        crt_memory_result.commit = memory_realloc_stub;
        crt_memory_result.decommit = memory_decommit_stub;
        crt_memory_result.release = crt_release_memory;
    }
    return &crt_memory_result;
}

///

memory_arena arena_create_() {
    memory_funcs* backing_funcs = global_crt_memory();
    return arena_create__(backing_funcs);
}

memory_arena arena_create__(memory_funcs* backing_funcs) {
    memory_arena result = {0};
    memory_block_node* block = (memory_block_node*)
        ((u8*)backing_funcs->reserve(DEFAULT_RESERVE_SIZE) - sizeof(memory_block_node));
    result.block = block;
    result.funcs = backing_funcs;
    return result;
}

void* arena_push_size(memory_arena* arena, u64 size) {
    void* result = 0;
    memory_funcs* funcs = arena->funcs;

    if ((arena->block->pos + size) > arena->block->size) {
        u64 reserve_size = Maximum(size, DEFAULT_RESERVE_SIZE);
        reserve_size = AlignPow2(reserve_size, 4096);
        
        memory_block_node* new_block = (memory_block_node*)
            ((u8*)funcs->reserve(reserve_size) - sizeof(memory_block_node));
        new_block->arena_prev = arena->block;
        arena->block = new_block;
    }

    result = (void*)(arena->block->memory_base + arena->block->pos);
    arena->prev_pos = arena->block->pos;
    arena->block->pos += size;

    memset(result, 0x00, size);

    return result;
}

void* arena_push_copy(memory_arena* arena, u64 size, void* source) {
    void* result = arena_push_size(arena, size);
    memcpy(result, (void*)source, size);
    return result;
}

void arena_pop_to_pos(memory_arena* arena, u64 pos) {
    memory_funcs* funcs = arena->funcs;
    u64 total_size = 0;

    // TODO: Freeing and or consolidating unused blocks

    if (pos < arena->block->pos) {
#if 0
        u64 aligned_pos = Minimum(pos, DEFAULT_COMMIT_SIZE);
        u64 next_pos = AlignPow2(aligned_pos, 4096);
        u64 new_pos = next_pos - arena->block->pos;
#endif
        arena->prev_pos = arena->block->pos;
        arena->block->pos = pos;
    }
}

void arena_pop(memory_arena* arena, u64 size) {
    if (size <= arena->block->pos) {
        arena_pop_to_pos(arena, arena->block->pos - size);
    }
}
