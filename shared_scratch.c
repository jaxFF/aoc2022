#define _CRT_SECURE_NO_WARNINGS 1
#include "types.h"
#include "mem.h"

#if defined(AOC_BUILD_EXE)
#include "shared.c"
#include "mem.c"
#endif

buffer load_input_custom() {
    return (buffer){0};
}

string day1_test_stuff(buffer input) {
    input;

    u64 start_ticks = os_time_elapsed_ticks();

#if 1
    string str = string_literal("yeah");
    str;

    memory_arena arena = arena_create(global_crt_memory());
    arena;

    string_list concat_strs = {0};
    string_list_push(&arena, &concat_strs, str);
    string_list_push(&arena, &concat_strs, string_literal(", swag."));

    string concatted = string_concat(&arena, &concat_strs);
    concatted;

    string_list splitted = string_split(&arena, string_literal("death, murder mayham; destruction"),
                                        (u8[]){ ',', ' ', ';' }, 3);
    splitted;

    string subbed = string_substring(string_literal("part1 SPLIT part2"), 
                                     string_to_range(string_literal("part1")));
    subbed;

#if 1
    void* crtmem1 = global_crt_memory()->reserve(64);
    memset(crtmem1, 0xAA, 64);
#endif

    memory_arena test = arena_create(global_crt_memory());
    char* yo = "hey what's up";
    void* Test1 = arena_push_size(&test, string_length(yo));
    memcpy(Test1, yo, string_length(yo));

    void* Test2 = arena_push_size(&test, string_length(yo));
    memcpy(Test2, yo, string_length(yo) - 1);

    void* Test3 = arena_push_size(&test, string_length(yo));
    memcpy(Test3, yo, string_length(yo) - 2);

    void* Test4 = arena_push_size(&test, string_length(yo));
    memcpy(Test4, yo, string_length(yo) - 3);

    arena_pop_to_pos(&test, string_length(yo)*3); // nice.
    arena_push_size(&test, 5);
    arena_pop(&test, 4);

    arena_push_size(&test, DEFAULT_COMMIT_SIZE);
    arena_pop(&test, DEFAULT_COMMIT_SIZE);
    arena_pop(&test, DEFAULT_COMMIT_SIZE);
    arena_pop(&test, DEFAULT_COMMIT_SIZE);
    arena_pop(&test, DEFAULT_COMMIT_SIZE);
    arena_pop(&test, DEFAULT_COMMIT_SIZE);
#endif

    // TODO: Should probably litter some of this test code with more Asserts, but I've 
    //  guaranteed expected behaviour through the debugger so far.
    memory_arena testarena = arena_create();
    void* ptr1 = arena_push_size(&testarena, Kilobytes(2));
    void* ptr2 = arena_push_size(&testarena, Kilobytes(1));
    Assert(ptr1 && ptr2);
    Assert(testarena.block->memory_base == ptr1);

    memset(ptr2, 0xAA, Kilobytes(1));

    arena_pop(&testarena, Kilobytes(1));
    arena_pop_to_pos(&testarena, Kilobytes(2)); // Should perform the same operation as last, effectively a noop here

    void* ptr2a = arena_push_size(&testarena, Kilobytes(2)); // Round off the usage of this last block

    void* ptr3 = arena_push_size(&testarena, DEFAULT_COMMIT_SIZE); // Should commit/alloc a new block

    memset(ptr3, 0xAA, DEFAULT_COMMIT_SIZE);
    
    arena_pop(&testarena, DEFAULT_COMMIT_SIZE); // This should force a free of this new block
    Assert(ptr1 && ptr2 && ptr2a);

//    void* ptr4 = arena_push_size(&testarena, Megabytes(16)); // A really large chunk

    u64 end_ticks = os_time_elapsed_ticks();
    u64 elapsed_ticks = (end_ticks - start_ticks);

    // I fucking hate dimensional analysis. This is the reciprocal of the tps, producing a timer resolution.

    // Seconds to ms is (seconds * 1ULL)
    // Microsecs(us) to ms is (seconds / 1e9)

    f64 reciprocal_factor = 1.0 / os_ticks_per_second();

    f64 ms_per_tick = (1ULL * reciprocal_factor); // good.
    f64 ns_per_tick = (1e6 * reciprocal_factor); // good.
    f64 us_per_tick = (1000ULL * reciprocal_factor);
    f64 s_per_tick = 1.0 / (1000ULL / reciprocal_factor);

    f64  elapsed_s = (elapsed_ticks) * s_per_tick;
    f64 elapsed_ms = (elapsed_ticks) * ms_per_tick;
    f64 elapsed_us = (elapsed_ticks) * us_per_tick;
    f64 elapsed_ns = (elapsed_ticks) * ns_per_tick;

    printf("Runtime took %0.4fns (%0.4fus)\n", elapsed_ns, elapsed_us);

    return input;
}

#if defined(AOC_BUILD_EXE)
int main(int argc, char** args) {
    string_list arg_list = strings_from_args(args, argc); arg_list;
    string_list null_args = {0};
    advent_solution sol_day1 = make_solution(1, day1_test_stuff, day1_test_stuff);
    advent_solution* solutions[] = {&sol_day1};
    return aoc_run_solutions(solutions, ArrayCount(solutions), null_args);
}
#endif
