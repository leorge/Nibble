/*
 * isort.h
 *
 *  Created on: 2012/10/01
 *      Author: leo
 */

#ifndef SORT_H_
#define SORT_H_
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define OUT     stdout

typedef enum {  // trace lelve
    TRACE_NONE,
    TRACE_COUNT,    // count up
    TRACE_DUMP,     // dump behavior
    TRACE_MOVE,     // dump memcpy()
    TRACE_COMPARE,  // dump strcmp()
    TRACE_DEBUG
} Trace;

#define bool    int
#define FALSE   0
#define TRUE    (! FALSE)
#define MAX_BIT     (8 * sizeof(size_t))    // bit size of nmemb

#define DIGIT_WIDTH 4       // nibble

#define MAX_SIZE    (1 << DIGIT_WIDTH)
#define MASK_DIGIT  (MAX_SIZE - 1)

typedef unsigned long long INSERT_INDEX;    // 4 bits/element * 16 elements = 64 bits = bit width of long long

typedef unsigned char       byte;

/* for glibc */
#ifndef __compar_d_fn_t
typedef int (*__compar_d_fn_t) (const void *, const void *, void *);
#endif

/***** variables *****/

extern double   random_number;      // main.c
extern size_t   medium_boundary;    // main.c
extern double   usec, esd;          // timer.c
extern long     *microsec;          // timer.c
extern int      peak;               // timer.c

#ifdef DEBUG
extern long qsort_moved, qsort_called;  // main.c
extern Trace    trace_level;            // main.c
#endif

/***** Functions *****/
extern void (*medium_array)();

// array sorting
void    array_sort      (void *base, size_t nmemb, size_t size, int (*compare)(const void *, const void *));
void    insert_binary   (void *base, size_t nmemb, size_t size, int (*compare)(const void *, const void *));
void    insert_sort   (void *base, size_t nmemb, size_t size, int (*compare)(const void *, const void *));

// index sorting
void    imerge_sort     (void *base, size_t nmemb, size_t size, int (*compare)(const void *, const void *));
void    mi_ibin         (void *base, size_t nmemb, size_t size, int (*compare)(const void *, const void *));
void    mi_inibble      (void *base, size_t nmemb, size_t size, int (*compare)(const void *, const void *));
void    mi_inblbin      (void *base, size_t nmemb, size_t size, int (*compare)(const void *, const void *));
void    mi_isort        (void *base, size_t nmemb, size_t size, int (*compare)(const void *, const void *));

// pointer sorting
void    pqsort          (void *base[], size_t nmemb, int (*compare)(const void *, const void *));
void    merge_pointer   (void *base[], size_t nmemb, int (*compare)(const void *, const void *));
void    mi_pbin         (void *base[], size_t nmemb, int (*compare)(const void *, const void *));
void    mi_pnblbin      (void *base[], size_t nmemb, int (*compare)(const void *, const void *));
void    mi_pnibble      (void *base[], size_t nmemb, int (*compare)(const void *, const void *));
void    mi_psort        (void *base[], size_t nmemb, int (*compare)(const void *, const void *));

// others
const char  *dump_data(const void *data);
void    **make_index(void *array1d, size_t nmemb, size_t size);
void    unindex(void *array1d, void *idxtbl[], size_t nmemb, size_t size);

// timer
bool    begin_timer(int count);
void    start_timer();
long    stop_timer();
int     end_timer(int skip);

// for debug
#ifdef DEBUG
void    dump_array(const char *msg, const void *head, size_t nmemb, size_t size);
void    dump_pointer(char *msg, void *head[], size_t length);
void    dump_rate(size_t n_lo, size_t n_hi);
#endif

#endif /* SORT_H_ */
