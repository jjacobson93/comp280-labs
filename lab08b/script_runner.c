/*
 * A program to run a shell script: i.e. a set of shell commands.
 *
 * Jeremy Jacobson
 * Copyright (c) 2014
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/errno.h>
#include "parse_args.h"

#define MAX_BUFFER 1024

/**
 * The handler function for SIGCHLD signals
 *
 * @param sig 	the signal
 */
void sighandler(int sig) {
	pid_t pid;

	while ((pid = waitpid(-1, NULL, WNOHANG)) > 0) {}

	if (errno != ECHILD) {
		fprintf(stderr, "Waitpid error\n");
	}

	return;
}

int main(int argc, char **argv) {
	// check number of args
	if (argc < 2) {
		printf("Usage: %s <script_file>\n", argv[0]);
		return 0;
	}

	// open the file
	char *filename = argv[1];
	FILE *input = fopen(filename, "r");

	// check if it's a valid file
	if (input == NULL) {
		printf("Could not open file: %s\n", filename);
		return 1;
	}

	// init a buffer
	char buf[MAX_BUFFER];
	for (int i = 0; i < MAX_BUFFER; ++i) {
		buf[i] = '\0';
	}

	// register SIGCHLD handler
	if (signal(SIGCHLD, sighandler) == SIG_ERR) {
		fprintf(stderr, "There was an error registering the handler\n");
		return 1;
	}

	// read lines from file into buffer
	while (fgets(buf, MAX_BUFFER, input) != NULL) {
		int bg;
		// parse the arguments
		char **args = parseArgumentsDynamic(buf, &bg);

		// fork process
		pid_t pid = fork();
		if (pid == 0) { // child process
			// execute
			int res = execvp(args[0], args);
			if (res) {
				fprintf(stderr, "There was an error running: %s\n", args[0]);
				return 1;
			}
		} else {
			// wait if it's not in the background
			if (!bg) {
				waitpid(pid, NULL, 0);
			}
		}
	}

	return 0;
}
