/*
 * Lab 7: "Game of Life"
 * Author: Jeremy Jacobson
 * 2014
 */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <string.h>

#define CIRC(x, size) ((x - 1 < 0) ? size - 1 : x)

typedef struct _board {
	int num_rows;
	int num_cols;
	int *repr;
} board_t;

board_t *init_board(FILE *f, int num_rows, int num_cols, int num_pairs) {
	board_t *self = malloc(sizeof(board_t));
	self->num_cols = num_cols;
	self->num_rows = num_rows;
	self->repr = calloc(num_cols*num_rows, sizeof(int));

	for (int x = 0; x < num_pairs; ++x) {
		int i = 0;
		int j = 0;
		fscanf(f, "%d %d\n", &i, &j);

		self->repr[(i*num_cols) + j] = 1;
	}

	return self;
}

void board_free(board_t *self) {
	free(self->repr);
	free(self);
}

int board_get(board_t *self, int row, int col) {
	return self->repr[(row*self->num_cols) + col];
}

int live_or_die(board_t *self, int row, int col) {
	int curr = board_get(self, row, col);
	int top_row = CIRC(row - 1, self->num_rows);
	int bottom_row = (row + 1)%self->num_rows;
	int left_col = CIRC(col - 1, self->num_cols);
	int right_col = (col + 1)%self->num_cols;

	int neighbors = board_get(self, top_row, left_col) +
					board_get(self, top_row, col) +
					board_get(self, top_row, right_col) +
					board_get(self, row, left_col) +
					board_get(self, row, right_col) +
					board_get(self, bottom_row, left_col) +
					board_get(self, bottom_row, col) +
					board_get(self, bottom_row, right_col);

	if (curr == 1 && neighbors < 2)
		return 0; // dies of loneliness
	else if (curr == 1 && neighbors > 3)
		return 0; // dies of overcrowding
	else if (curr == 1 && (neighbors == 2 || neighbors == 3))
		return 1; // stays alive
	else if (curr == 0 && neighbors == 3)
		return 1; // born

	return 0; // stays dead
}

int board_next(board_t *self, int *iteration) {
	if (*iteration > 1) {
		int *new_repr = calloc(self->num_rows*self->num_cols, sizeof(int));
		for (int i = 0; i < self->num_rows; ++i) {
			for (int j = 0; j < self->num_cols; ++j) {
				new_repr[(i*self->num_cols) + j] = live_or_die(self, i, j);
			}
		}

		for (int i = 0; i < self->num_rows*self->num_cols; ++i) {
			self->repr[i] = new_repr[i];
		}

		free(new_repr);
		return 1;
	}

	return 0;
}

void print_board(board_t *self, int iteration) {
	printf("\n");
	printf("Time Step: %d\n", iteration);
	printf("\n");
	for (int i = 0; i < self->num_rows; ++i) {
		for (int j = 0; j < self->num_cols; ++j) {
			int alive = board_get(self, i, j);
			printf("%c ", (alive) ? '@' : '-');
		}
		printf("\n");
	}

	printf("\n");
}

/**
 * 
 * http://unix.stackexchange.com/questions/43075/how-to-change-the-contents-of-a-line-on-the-terminal-as-opposed-to-writing-a-new
 */
void clear(int n) {
	for (int i = 0; i < n; ++i) {
		printf("\033[1A");
		printf("\033[K");
	}
}

int main(int argc, char **argv) {
	if (argc < 3) {
		printf("Usage: ./gol <infile> <print>\n");
		return 0;
	}

	FILE *f = fopen(argv[1], "r");
	int to_print = strtol(argv[2], NULL, 10);

	// Check if we opened the file
	if (f != NULL) { 
		int num_rows = 0;
		int num_cols = 0;
		int num_iterations = 0;
		int num_pairs = 0;
		double total_time = 0.0;
		
		// read numbers
		fscanf(f, "%d\n%d\n%d\n%d", &num_rows, &num_cols, &num_iterations, &num_pairs);

		// set up board
		board_t *board = init_board(f, num_rows, num_cols, num_pairs);

		// set up timing
		struct timeval start_time;
		struct timeval end_time;
		gettimeofday(&start_time, NULL);

		// run iterations
		int i = num_iterations;
		while (board_next(board, &i)) {
			if (to_print) {
				print_board(board, num_iterations - i);
				usleep(200000);
				clear(num_rows + 4);
			}
			i--;
		}

		// get end time
		gettimeofday(&end_time, NULL);
		total_time = ((end_time.tv_sec + (end_time.tv_usec/1000000.0)) - (start_time.tv_sec + (start_time.tv_usec/1000000.0)));

		// print final board
		print_board(board, num_iterations);

		printf("total time for %d iteration%s of %dx%d world is %f sec\n", num_iterations, (num_iterations != 1) ? "s" : "", num_rows, num_cols, total_time);
		fclose(f);
		board_free(board);
	} else {
		printf("There was an error opening '%s'\n", argv[1]);
		return 1;
	}

	return 0;
}