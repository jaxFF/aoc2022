#ifndef AOC_SHARED_H
#define AOC_SHARED_H

global b32 aoc_should_print_solution_fluff = false;
global b32 aoc_global_test2 = false;

global string_list strings_from_args(char* args[], int argc) {
    string_list result = {0};
    memory_arena arena = arena_create();
    for (int i = 0; i < argc; ++i) {
        string_list_push(&arena, &result, string_c(args[i]));
    }
    return result;
}

/* stdout should be something like:
 
   Solution for Day x:
       Part one: x
       Part two: x
       Runtime took %0.4ns

*/

global int aoc_run_solutions(advent_solution** solutions, u64 solution_count, string_list args) {
    solutions; solution_count; args;

    // TODO(@jax): 
    
    memory_arena arena = arena_create(); arena;

    buffer null_input = { 0 };
    b32 is_single_solution = (solution_count==1); is_single_solution;

    advent_solution** ptr = solutions;
    advent_solution* last_solution = solutions[solution_count-1];
    for (; *ptr <= last_solution && *ptr;) {
        advent_solution* solution = *ptr;

        string day_name = { 0 };
        if (is_single_solution) {
#if 0
            string name_temp = args.first->str;
            day_name;
            name_temp;
            
            string_list split = string_split(&arena, name_temp, 
                    (u8[]){ PathSeperator, PathSeperatorOpp }, 2);
            name_temp = split.last->str;

            split = string_split(&arena, name_temp,
                (u8[]) { '.' }, 1);
            name_temp = split.first->str;
#endif
        }

#define printf_solution_fmt(fmt, ...) fprintf(stdout, "%4s" fmt "", " ", ##__VA_ARGS__)
#define printf(fmt, ...) printf_solution_fmt(fmt, __VA_ARGS__)
        solution->part1(null_input);
#undef printf

        // NOTE(@jax): Hack
        if (is_single_solution || *ptr == last_solution) {
            break;
        }

        ++ptr;
    }

    return 0;
}

/// String implementation

inline u32 string_length(char* str) {
    int result_count = 0;
    while (*str++) {
        ++result_count;
    }
    return result_count;
}

inline string make_string(u8* bytes, u64 size) {
    string result = {0};
    result.data = bytes;
    result.count = size;
    return result;
}

inline string string_copy(memory_arena* arena, string str) {
    string result = {0};
    result.data = arena_push_copy(arena, str.count, str.data);
    result.count = str.count;
    result.data[result.count] = 0;
    return result;
}

string make_string_range(u8* first, u8* one_past_last) {
    string result = make_string(first, (u64)(one_past_last-first));
    return result;
}

inline string string_chop(string str, u64 count) {
    u64 chop_amnt = Minimum(count, str.count);
    string result = {str.data, str.count - chop_amnt};
    return result;
}

inline string string_substring(string str, string_range range) {
    string result = {(u8*)str.data + range.start, range.end};
    return result;
}

void string_list_push(memory_arena* arena, string_list* list, string str) {
    string_node* node = arena_push_struct(arena, string_node);
    node->str = string_copy(arena, str);

    string_list_push_node(list, node);
}

void string_list_push_node(string_list* list, string_node* node) {
    string str = node->str;
    SLLQueuePushBack(list->first, list->last, node);
    list->total_size += str.count; 
    ++list->count;
}

string string_concat(memory_arena* arena, string_list* list) {
    string result = {0};

    u64 size = list->total_size;
    u8* data = arena_push_array(arena, size, u8);
    u8* write_ptr = data;
    for (string_node* at = list->first; at; at = at->next) {
        memcpy(write_ptr, at->str.data, at->str.count);
        size += at->str.count;
        write_ptr += at->str.count;
    }

    *write_ptr = 0;

    result.data = data;
    result.count = list->total_size;
    return result;
}

// basically is strstr, tokenizes strings based on provided delims
string_list string_split(memory_arena* arena, string str, u8* split_bytes, u32 split_count) {
    split_bytes;
    split_count;
    arena;
    
    string_list result = {0};

    u8* start = str.data;
    u8* end = start + str.count;
    u8* ptr = start;
    for (ptr; ptr < end; ++ptr) {
        u8 byte = *ptr;
        byte;

        b32 should_split = false;
        for (u32 i = 0; i < split_count; ++i) {
            u8 split_byte = split_bytes[i];
            if (byte == split_byte) {
                should_split = true;
                break;
            }
        }

        if (should_split) {
            if (start < ptr) {
                string split = make_string_range(start, ptr);
                string_list_push(arena, &result, split);
            }
            start = ptr + 1;
        }
    }

    if (start < ptr) {
        string split = make_string_range(start, ptr);
        string_list_push(arena, &result, split);
    }

    return result;
}

#endif
