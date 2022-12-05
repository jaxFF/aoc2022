#ifndef AOC_SHARED_H
#define AOC_SHARED_H

#if defined(_WIN32) || defined(_WIN64) 
#define PathSeperator '\\'
#define PathSeperatorOpp '/'
#else
#define PathSeperator '/'
#define PathSeperatorOpp '\\'
#endif

global string_list strings_from_args(char* args[], int argc) {
    string_list result = {0};
    memory_arena arena = arena_create(global_crt_memory());
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
    return 0;
}

#if 0
global int aoc_run_solutions(aoc_days days, string_list args) {
    args;
    memory_arena arena = arena_create(global_crt_memory());
    arena;

    aoc_day_func** days_ptr = days.days;
    u32 day_count = days.day_count; 
    days_ptr[day_count] = 0;

    b32 single_day = (day_count == 1);
    single_day;

    aoc_day_func** ptr = days_ptr;
    aoc_day_func* last_day = days_ptr[day_count - 1];
    for (; *ptr <= last_day && *ptr;) {
        aoc_day_func* current_day_func = *ptr;

        string day_name = { 0 };
        if (single_day) {
            string name_temp = args.first->str;
            day_name;
            name_temp;
            
            string_list split = string_split(&arena, name_temp, 
                    (u8[]){ PathSeperator, PathSeperatorOpp }, 2);
            name_temp = split.last->str;

            split = string_split(&arena, name_temp,
                (u8[]) { '.' }, 1);
            name_temp = split.first->str;
//            Breakpoint;
        }

        buffer input = { 0 };
        current_day_func(input);
        ++ptr;
    }

//    os_cmd_line_pause();
    return 0;
}
#endif

/// String implementation

global inline u32 string_length(char* str) {
    int result_count = 0;
    while (*str++) {
        ++result_count;
    }
    return result_count;
}

global inline string make_string(u8* bytes, u64 size) {
    string result = {0};
    result.data = bytes;
    result.count = size;
    return result;
}

global inline string string_copy(memory_arena* arena, string str) {
    string result = {0};
    result.data = arena_push_copy(arena, str.count, str.data);
    result.count = str.count;
    result.data[result.count] = 0;
    return result;
}

global string make_string_range(u8* first, u8* one_past_last) {
    string result = make_string(first, (u64)(one_past_last-first));
    return result;
}

global inline string string_chop(string str, u64 count) {
    u64 chop_amnt = Minimum(count, str.count);
    string result = {str.data, str.count - chop_amnt};
    return result;
}

global inline string string_substring(string str, string_range range) {
    string result = {(u8*)str.data + range.start, range.end};
    return result;
}

global void string_list_push(memory_arena* arena, string_list* list, string str) {
    string_node* node = arena_push_struct(arena, string_node);
    node->str = string_copy(arena, str);

    string_list_push_node(list, node);
}

global void string_list_push_node(string_list* list, string_node* node) {
    string str = node->str;
    SLLPushBack_Queue(list->first, list->last, node);
    list->total_size = str.count; 
    ++list->count;
}

global string string_concat(memory_arena* arena, string_list* list) {
    string result = {0};

    u64 size = list->total_size;
    u8* data = arena_push_array(arena, size, u8);
    u8* write_ptr = data;
    for (string_node* at = list->first; at; at = at->next) {
        memcpy(write_ptr, at->str.data, at->str.count);
        write_ptr += at->str.count;
    }

    result.data = data;
    result.count = string_length((char*)data);
    result.data[result.count] = 0;
    return result;
}

// basically is strstr, tokenizes strings based on provided delims
global string_list string_split(memory_arena* arena, string str, u8* split_bytes, u32 split_count) {
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
