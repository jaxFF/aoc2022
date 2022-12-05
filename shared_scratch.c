#include "types.h"

#include "mem.h"
#include "shared.h"
#include "mem.c"

void day1(buffer input) {
    input;

    f64 start_time = os_time_elapsed_ms();

    string str = string_literal("yeah");

    str;
    memory_arena arena = arena_create(global_crt_memory());

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

#if 0
    char* yo = "hey what's up";
    void* Test1 = arena_push_size(&arena, string_length(yo));
    memcpy(Test1, yo, string_length(yo));

    void* Test2 = arena_push_size(&arena, string_length(yo));
    memcpy(Test2, yo, string_length(yo) - 1);

    void* Test3 = arena_push_size(&arena, string_length(yo));
    memcpy(Test3, yo, string_length(yo) - 2);

    void* Test4 = arena_push_size(&arena, string_length(yo));
    memcpy(Test4, yo, string_length(yo) - 3);

    //arena_pop(&arena, string_length(yo)*3); // nice.
#endif

    double elapsed;
    f64 end_time = os_time_elapsed_ms();
    elapsed = end_time - start_time;
    printf("Runtime for solution took %0.4fns (%0.4fus)\n", time_elapsed_ns(elapsed), time_elapsed_us(elapsed));

}

#if defined(AOC_BUILD_EXE)
int main(int argc, char** args) {
    string_list arg_list = strings_from_args(args, argc);
    advent_solution sol_day1 = make_solution(1, day1, day1);
    advent_solution* solutions[] = {&sol_day1};
    return aoc_run_solutions(solutions, ArrayCount(solutions), arg_list);
}
#endif
