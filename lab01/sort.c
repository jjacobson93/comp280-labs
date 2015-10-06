//
//  sort.c
//  employee_db
//
//  Created by Jeremy Jacobson on 9/16/14.
//  Copyright (c) 2014 Jeremy Jacobson. All rights reserved.
//

#include <stdio.h>
#include "sort.h"

/**
 * Swaps elements from arr of size arr_size at index i and j
 *
 * @param arr       pointer to elements
 * @param arr_size  size of arr
 * @param i         index of first element
 * @param j         index of second element
 */
void swap(void **arr, size_t arr_size, int i, int j) {
    void *tmp;
    tmp = arr[i];
    arr[i] = arr[j];
    arr[j] = tmp;
}

/**
 * Partitions arr from left to right
 *
 * @param arr       pointer to elements
 * @param arr_size  size of arr
 * @param left      left index
 * @param right     right index
 * @param cmp       function pointer to element comparator
 * @return store index
 */
int partition(void **arr, size_t arr_size, int left, int right, int (*cmp)(void*, void*)) {
    int pivot, store_idx, i;
    void *val;
    
    pivot = (left + right)/2;
    val = arr[pivot];
    swap(arr, arr_size, pivot, right);
    
    store_idx = left;
    for (i = left; i < right; i++) {
        if (cmp(arr[i], val)) {
            swap(arr, arr_size, i, store_idx);
            store_idx++;
        }
    }
    
    swap(arr, arr_size, store_idx, right);
    
    return store_idx;
}

/**
 * Quicksorts elements given by arr
 *
 * @param arr       pointer to elements
 * @param arr_size  size of arr
 * @param i         start index
 * @param k         end index
 * @param cmp       function pointer to element comparator
 */
void quicksort(void **arr, size_t arr_size, int i, int k, int (*cmp)(void*, void*)) {
    int p;
    if (i < k) {
        p = partition(arr, arr_size, i, k, cmp);
        quicksort(arr, arr_size, i, p - 1, cmp);
        quicksort(arr, arr_size, p + 1, k, cmp);
    }
}