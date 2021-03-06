/*
 * array_sort.c
 *
 *  Array sort of QM / QMI sort
 *
 *  Created on: 2013/03/23
 *      Author: leo
 */
#include    <math.h>
#include "sort.h"

static int      (*comp)(const void *, const void *);
//static void 	*index[MAX_BIT];    // address of picked up elements
static size_t   length;
static char 	*pivot;
#ifdef  DEBUG
static  size_t  search_pivot;
#endif

static void copy(void *dst, const void *src)
{
#ifdef  DEBUG
    qsort_moved++;
    if (trace_level >= TRACE_MOVE) fprintf(OUT, "copy(dst = %s, src = %s)\n", dump_data(dst), dump_data(src));
#endif
    memcpy(dst, src, length); /* restore an elements  */
}

static void sort(void *base, size_t nmemb) {
    if (nmemb <= 1) return;
#ifdef DEBUG
    qsort_called++;
    dump_array("sort() start in " __FILE__, base, nmemb, length);
#endif
    if (nmemb <= medium_boundary) {
        (*medium_array)(base, nmemb, length, comp);
    }
    else {	// N is large
#define first   ((char *)base)
    	size_t  distance;
    	char    *hole;
		distance = nmemb / 3;    // distance between elements
		char	*p1, *p2, *p3;
		p1 = base + (size_t)(random_number * distance) * length;		// pick up median of random 3 elements
		p3 = (p2 = p1 + distance * length) + distance * length;
#ifdef  DEBUG
		if (trace_level >= TRACE_DUMP) fprintf(OUT,
				"nmemb = %ld\tdistance = %ld\t pickup = (%s, %s, %s)\n", nmemb, distance, p1, p2, p3);
#endif
		hole = (comp(p1, p3) < 0 ?
			   (comp(p2, p1) < 0 ? p1: (comp(p2,  p3) < 0 ? p2 : p3)) :
			   (comp(p2, p3) < 0 ? p3 : (comp(p2, p1) < 0 ? p2 : p1)));
#ifdef DEBUG
    	if (trace_level >= TRACE_DUMP) fprintf(OUT, "pivot = %s\n", dump_data(hole));
#endif
        char *last = first + length * (nmemb - 1);
#ifdef  DEBUG
        if (trace_level >= TRACE_DUMP) fprintf(OUT, "pivot <-- hole = %s <-- last = %s\n", dump_data(hole), dump_data(last));
#endif
        copy(pivot, hole); copy(hole, last);    // pivot <-- hole <-- last
        char    *lo = first,  *hi = (hole = last) - length, *eq = NULL;
        for (; lo < hole; lo += length) {
            if (comp(lo, pivot) >= 0) {
#ifdef  DEBUG
                if (trace_level >= TRACE_DUMP) fprintf(OUT, "move %s --> %s\n", dump_data(lo), dump_data(hole));
#endif
                copy(hole, lo);
                hole = lo;
                for (; hi > hole; hi -= length) {
                    int chk;
                    if ((chk = comp(hi, pivot)) < 0) {
#ifdef  DEBUG
                        if (trace_level >= TRACE_DUMP) fprintf(OUT, "move %s <-- %s\n", dump_data(hole), dump_data(hi));
#endif
                        copy(hole, hi);
                        hole = hi;
                        eq = NULL;
                    }
                    else if (chk > 0) eq = NULL;
                    else if (eq == NULL) eq = hi;
                }
            }
        }
#ifdef  DEBUG
        if (trace_level >= TRACE_DUMP) fprintf(OUT, "restore pivot %s to %s [%ld]\n",
                dump_data(pivot), dump_data(hole), (hole - first) / length);
#endif
        copy(hole, pivot);  // restore
#ifdef DEBUG
        dump_array("sort() partitioned", base, nmemb, length);
#endif
        if (eq == NULL) eq = hole;
#ifdef DEBUG
        else if (trace_level >= TRACE_DUMP) fprintf(OUT,"skip higher %ld elements\n", (eq - hole) / length);
#endif
        size_t  n_lo = (hole - first) / length; // number of element in lower partition
        size_t  n_hi = (last - eq) / length;
#ifdef DEBUG
        dump_rate(n_lo, n_hi);
#endif
        sort(first, n_lo);
        sort(eq + length, n_hi);
    }
#ifdef DEBUG
    dump_array("sort() done.", base, nmemb, length);
#endif
}

void array_sort(void *base, size_t nmemb, size_t size, int (*compare)(const void *, const void *))
{
    if (nmemb > 1) {
        char a[size]; pivot = a; *a = '\0';
        length = size; comp = compare;
#ifdef DEBUG
        search_pivot = 0;
#endif
        sort(base, nmemb);
#ifdef DEBUG
        if (trace_level >= TRACE_DUMP) fprintf(OUT, "search_pivot = %ld\n", search_pivot);
#endif
    }
}
