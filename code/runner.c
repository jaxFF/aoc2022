#define AOC_USE_DEBUG 0 // Ensure we are optimized
#include "types.h"
#include "mem.h"
#include "shared.c"
#include "mem.c"
    
extern aoc_part_func day1_test_stuff;

int main(int argc, char* argv[]) {
//    string_list arg_list = strings_from_args(argv, argc);
    string_list null_args = {0};
    advent_solution sol_day1 = make_solution(1, day1_test_stuff, day1_test_stuff);
    advent_solution* solutions[] = {&sol_day1};
    return aoc_run_solutions(solutions, ArrayCount(solutions), null_args);
}
