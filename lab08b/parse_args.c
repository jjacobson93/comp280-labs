/*
 * Implementation file for parse_args library.
 *
 * Jeremy Jacobson
 * Copyright (c) 2014
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "parse_args.h"

/* 
 * parseArguments - Parse the command line and build the argv array.
 * @param cmdline The command line string entered at the shell prompt
 *             (const means that the function will not modify the cmdline string)
 * @param argv  An array of size MAXARGS of char *
 *              parseArguments will initialize its contents from the passed
 *              cmdline string.
 * @returns    Non-zero if the command line includes &, to
 *             run in the background, or zero if not 
 */
int parseArguments(const char *cmdline, char **argv) {
	int is_bg = 0;

	// copy cmdline
	char cmdline_copy[MAXLINE];
	strcpy(cmdline_copy, cmdline);

	// tokenize
	int i = 0;
	char *token = strtok(cmdline_copy, " \t\n");
	while (token != NULL) {
		if (strcmp(token, "&") != 0) {
			argv[i++] = token;
		} else {
			is_bg = 1;
			break;
		}

		token = strtok(NULL, " \t\n");

		if (i == MAXARGS - 1) {
			break;
		}
	}

	while (i < MAXARGS) {
		argv[i++] = NULL;
	}

	return is_bg;
}

/*
 * parseArgumentsDynamic - parse the passed command line into an argv array
 *
 * @param cmdline The command line string entered at the shell prompt
 *             (const means that this function cannot modify cmdline)
 * @param bg   Sets the value pointed to by bg 1 if command line is run in 
 *             background, 0 otherwise (a pass-by-reference parameter)
 *
 * @return   A dynamically allocated array of strings, each element
 *             stores a string corresponding to a command line argument
 *             (the caller is responsible for freeing the returned
 *             argv list).
 */
char **parseArgumentsDynamic(const char *cmdline, int *bg) {
	// copy cmdline
	int len = strlen(cmdline);
	char *cmdline_copy = malloc(sizeof(char)*len);
	strcpy(cmdline_copy, cmdline);

	// determnine size of argv
	int size = 0;
	char *token = strtok(cmdline_copy, " \t\n");
	while (token != NULL) {
		size++;
		if (strcmp(token, "&") == 0) {
			break;
		}

		token = strtok(NULL, " \t\n");
	}

	// make a new copy
	free(cmdline_copy);
	cmdline_copy = malloc(sizeof(char)*len);
	strcpy(cmdline_copy, cmdline);

	// malloc argv
	char **argv = malloc(sizeof(char*)*(size + 1));

	// tokenize
	int i = 0;
	token = strtok(cmdline_copy, " \t\n");
	while (token != NULL) {
		argv[i] = malloc(sizeof(char)*strlen(token));
		strcpy(argv[i++], token);

		token = strtok(NULL, " \t\n");
	}

	if (size > 0 && argv[size - 1] && strcmp(argv[size - 1], "&") == 0) {
		argv[size - 1] = NULL;
		*bg = 1;
	} else {
		*bg = 0;
	}

	argv[size] = NULL;

	return argv;
}
