/*
 * TODO: add top-level comment
 *
 */

#include <stdio.h>
#include <stdlib.h>

// You will implement this function in swap.s
/*
 * Rotates the values pointed at by the parameters
 * @param x A pointer to an integer
 * @param y A pointer to another integer
 */
void rotate(int *x, int *y, int *z);

int main(int argc, char **argv) {
	if(argc != 4) {
		printf("usage: ./swap-test a b c\n");
		exit(1);
	}

	// Convert command line args from text to integers
	int a = atoi(argv[1]);
	int b = atoi(argv[2]);
	int c = atoi(argv[3]);

	printf("Before rotate: a = %d, b = %d, c = %d \n", a, b, c);
	rotate(&a, &b, &c);
	printf("After first rotate: a = %d, b = %d, c = %d \n", a, b, c);
	rotate(&a, &b, &c);
	printf("After second rotate: a = %d, b = %d, c = %d \n", a, b, c);

	return 0;
}
