//
//  sort.h
//  employee_db
//
//  Created by Jeremy Jacobson on 9/16/14.
//  Copyright (c) 2014 Jeremy Jacobson. All rights reserved.
//

#ifndef employee_db_sort_h
#define employee_db_sort_h

/**
 * Quicksorts elements given by arr
 *
 * @param arr       pointer to elements
 * @param arr_size  size of arr
 * @param i         start index
 * @param k         end index
 * @param cmp       function pointer to element comparator
 */
void quicksort(void **arr, size_t arr_size, int i, int k, int (*cmp)(void*, void*));

#endif
