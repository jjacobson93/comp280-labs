/* 
 * stats.c
 * 
 * Lab 5
 * Author: Jeremy Jacobson
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "readfile.h"

double *getValues(int *size, int *capacity, char *filename);
double *new_array(int *capacity, double *arr);
double mean(int size, double *arr);
double median(int size, double *arr);
double stddev(int size, double mean, double *arr);
void sort(int size, double *arr);

int main(int argc, char *argv[]) {
	if(argc != 2) {
		printf("argc != 2\n");
		printf ("usage: ./stats filename\n");
		exit(1);
	}
	
	int size = 0;
	int capacity = 20;
	double *arr = getValues(&size, &capacity, argv[1]);
	sort(size, arr);

	printf("\nStatistics:\n");
	printf("--------\n");
	printf("num values:\t%i\n", size);

	double m = mean(size, arr);
	printf("mean:\t\t%f\n", m);

	double med = median(size, arr);
	printf("median:\t\t%f\n", med);

	double sd = stddev(size, m, arr);
	printf("stddev:\t\t%f\n", sd);

	printf("\nUnused array slots: %i\n", capacity - size);
	
	free(arr);
}

/** 
 * Gets the values from a file
 * @param size Pointer to the size of the array
 * @param capacity Pointer to the capacity of the arry
 * @param filename string representing the file name
 * @return The array of double values
 */
double *getValues(int *size, int *capacity, char *filename) {
	if (openFile(filename) != -1) {
		double *arr = malloc(sizeof(double)*(*capacity));
		while (readDouble(&arr[*size]) == 0) {
			if (*size == *capacity - 1) {
				arr = new_array(capacity, arr);
			}
			(*size)++;
		}
		closeFile(filename);
		return arr;
	} else {
		fprintf(stderr, "There was an error opening the file: %s\n", filename);
		return NULL;
	}

	return NULL;
}

/**
 * Allocates a new array of capacity (capacity * 20)
 *
 * @param capacity pointer to capacity
 * @param arr old array to copy values from
 * @return new array
 */
double *new_array(int *capacity, double *arr) {
	int prev_capacity = *capacity;
	*capacity *= 20;
	double *new = malloc(sizeof(double)*(*capacity));
	for (int i = 0; i < prev_capacity; i++) {
		new[i] = arr[i];
	}

	free(arr);
	return new;
}

/**
 * Finds the sum of an array
 *
 * @param size the size of the array
 * @param arr the array
 * @return the sum
 */
double sum(int size, double *arr) {
	double sum = 0;
	for (int i = 0; i < size; i++) {
		sum += arr[i];
	}
	return sum;
}

/**
 * Finds the mean of an array of values
 *
 * @param size the size of the array
 * @param arr the array
 * @return the mean
 */
double mean(int size, double *arr) {
	return sum(size, arr)/(size*1.0);
}

/**
 * Finds the median of an array of values
 *
 * @param size the size of the array
 * @param arr the array
 * @return the median
 */
double median(int size, double *arr) {
	int mid = size/2;
	if (size%2 == 0) {
		return (arr[mid] + arr[mid + 1])/2.0;
	} else {
		return arr[mid];
	}
}

/**
 * Finds the standard deviation of an array of values
 *
 * @param size the size of the array
 * @param mean the mean of the values
 * @param arr the array
 * @return the standard deviation
 */
double stddev(int size, double mean, double *arr) {
	double msd = 0;
	for (int i = 0; i < size; i++) {
		msd += pow(arr[i] - mean, 2.0);
	}
	msd /= (size*1.0 - 1.0);

	return sqrt(msd);
}

/**
 * Sorts an array in place
 *
 * @param size the size of the array
 * @param arr the array
 */
void sort(int size, double *arr) {
	for (int i = 0; i < size; i++) {
		for (int j = i + 1; j < size; j++) {
			if (arr[j] < arr[i]) {
				double tmp = arr[i];
				arr[i] = arr[j];
				arr[j] = tmp;
			}
		}
	}
}


