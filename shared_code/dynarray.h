#ifndef AOC_DYNARRAY_H
#define AOC_DYNARRAY_H

typedef struct _dynarr_header _dynarr_header;
struct _dynarr_header {
    u64 size;
    u64 cap;
};

#define dynarr_header(arr) ((arr) ? (arr - sizeof(_dynarr_header)) : 0)

global void* dynarr_growf(memory_arena* arena, void* array_base, u64 element_size, u64 count);

#endif
