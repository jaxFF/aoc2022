#ifndef AOC_TYPES_H
#define AOC_TYPES_H

#define AOC_USE_DEBUG 1

#ifndef AOC_USE_DEBUG
#define AOC_USE_DEBUG 1
#endif

#if AOC_USE_DEBUG
#pragma optimize("", off)
#endif

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

typedef int16_t int16;
typedef int32_t int32;

typedef int16 s16; // signed 16-bit integer
typedef int32 s32;
typedef int32 b32;

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

typedef uint8 u8;
typedef uint16 u16;
typedef uint32 u32;
typedef uint64 u64;

typedef float f32;
typedef double f64;

#define global static

#define time_elapsed_us(ms) (ms * 1000ULL)
#define time_elapsed_ns(ms) (time_elapsed_us(ms) * 1000ULL)
#define time_elapsed_s(ms) (ms / 1000ULL)
global f64 os_time_elapsed_ms();
global int os_cmd_line_pause();

#if defined(_WIN32) || defined(_WIN64) 
#include "win32_specific.h"
#else
#include "posix_specific.h"
#endif

#define ArrayCount(arr) (sizeof(arr)/sizeof((arr)[0]))

#define AlignPow2(val, align) (((val) + (align) - 1) & ~(((val)-(val)) + (align) - 1)) // NOTE: Value minus Value forces integral promotion

// from <cstdlib.h> because this isn't defined in C for some reason on MSVC. thanks MS :/
#if defined(_MSC_VER)
typedef double max_align_t; // most aligned type
#endif

#define Minimum(a, b) ((a < b) ? (a) : (b))
#define Maximum(a, b) ((a > b) ? (a) : (b))

#define Kilobytes(value) (((u64)value) * 1024LL)
#define Megabytes(value) (Kilobytes((u64)alue) * 1024LL)

#undef Assert
#pragma warning(disable:4127) // mem.c(at Assert): warning C4127: conditional expression is constant
#define Assert(expr) do { if (!(expr)) { *(volatile int*)(0) = (0); } } while (0)

#if defined(_MSC_VER)
#define Breakpoint __debugbreak()
#else 
#define Breakpoint __asm ("int $0x3")
#endif

#undef ZeroMemory
#define ZeroMemory(ptr, size) (memset((ptr), 0, (size)))
#define ZeroStruct(ptr) ZeroMemory(ptr, sizeof(*(ptr)))
#define ZeroArray(ptr, count) ZeroMemory(ptr, count*sizeof((ptr)[0]))

typedef struct buffer buffer;
struct buffer {
    u8* data;
    u64 count;
};

///

// An example linked list node, the macros will operate on any type
typedef struct list_node list_node;
struct list_node {
    list_node* next;
    list_node* prev;
};

#define SLLPushBack_Queue(first, last, elem) SLLPushBack_Queue_(first, last, elem, next)
#define SLLPushBack_Queue_(first, last, elem, member_next) do { \
    if ((first) == 0) {                                         \
        (first) = (last) = (elem);                              \
    } else {                                                    \
        (last)->member_next = (elem);                           \
    }                                                           \
    (last) = (elem);                                            \
    (last)->member_next = 0;                                    \
} while (0) 

#define SLLPop_Queue_(first, last, member_next) do {    \
    if ((first) == (last)) {                            \
        (first) = (last) = 0;                           \
    } else {                                            \
        (first) = (first)->member_next;                 \
    }                                                   \
} while (0)
#define SLLPop_Queue(first, last) SLLPop_Queue_(first, last, next)

#define SLLPush_Stack_(first, elem, next) do {   \
    (elem)->next = (first);                     \
    (first) = (elem);                           \
} while (0) 
#define SLLPush_Stack(first, elem) SLLPush_Stack_(first, elem, next)

#define SLLPop_Stack_(first, next) do {         \
    if ((first)) {                              \
        (first) = (first)->next;                \
    }                                           \
} while (0) 
#define SLLPop_Stack(first) SLLPop_Stack_(first, next)

// Circularly-doubly-linked list using a single "sentinel pointer
#define DLLInit(first) do { (first)->next=(first), (first)->prev=(first); } while(0)
#define DLLInsert_NP(first, elem, next, prev) do {  \
    (elem)->next = (first)->next;                   \
    (elem)->prev = (first);                         \
    (elem)->next->prev = (elem);                    \
    (elem)->prev->next = (elem);                    \
} while (0)

#define DLLPushFront(first, elem) DLLInsert_NP(first, elem, next, prev)
#define DLLPushBack(first, elem) DLLInsert_NP(first, elem, prev, next)

#define DLLRemove(elem) do {                \
    if ((elem)->next) {                     \
        (elem)->next->prev = (elem)->prev;  \
        (elem)->prev->next = (elem)->next;  \
        (elem)->next = (elem)->prev = 0;    \
    }                                       \
} while(0)

/// Length-based strings to avoid null-terminated c strings

typedef struct memory_arena memory_arena;
typedef buffer string; // all implementation deets in shared.h right now !!

typedef struct string_range string_range;
struct string_range {
    u64 start;
    u64 end;
};

typedef struct string_node string_node;
struct string_node {
    string str;
    string_node* next;
};

typedef struct string_list string_list;
struct string_list {
    string_node* first;
    string_node* last;
    u64 total_size;
    u64 count;
};

global u32 string_length(char*);
global string make_string(u8* bytes, u64 size);
#define string_c(cstr) make_string((u8*)(cstr), string_length(cstr))
#define string_literal(cstr) make_string((u8*)(cstr), sizeof(cstr)-1)
#define string_to_range(string) ((string_range ){0, string.count})

global string string_copy(memory_arena* arena, string);

global string make_string_range(u8* first, u8* one_past_last);

global string string_chop(string, u64 Count);
global string string_substring(string, string_range);

global void string_list_push(memory_arena*, string_list*, string);
global void string_list_push_node(string_list*, string_node*);

global string string_concat(memory_arena*, string_list*);
global string_list string_split(memory_arena*, string, u8* split_bytes, u32 split_count);

// TODO: string builder!!

///

typedef struct advent_day advent_day;
struct advent_day {
    u64 d;
};

typedef void aoc_part_func(buffer input);
void aoc_part_stub_func(buffer input) {
    input; Assert(!"Implement me!");
}

typedef struct advent_solution advent_solution;
struct advent_solution {
    advent_day day;
    string day_name;

    aoc_part_func* part1;
    aoc_part_func* part2;

    string day_name_stem; // Substring of executable name, up to but not including the period
};

global inline advent_solution make_solution(u64 day_no, aoc_part_func* part1, aoc_part_func* part2) {
    advent_solution result = {0};
    result.day.d = day_no;
    result.part1 = part1;
    result.part2 = part2;
    return result;
}

global string_list strings_from_args(char* args[], int argc);
global int aoc_run_solutions(advent_solution** solutions, u64 solution_count, string_list args);

#endif
