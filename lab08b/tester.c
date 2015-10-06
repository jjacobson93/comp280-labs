/*
 * Tester program for COMP 280 Lab 8B (parse_args library)
 *
 * Add your top-level comment here.
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h> 
#include "parse_args.h"

// static: means these functions cannot be called from outside this file
static void printCommandArgs(char *cmdline, char **argv );
static void printBG(int bg) ;

int main() { 

	int bg;
	char cmdline[MAXLINE];
	char *argv[MAXARGS], **args;


	printf("Enter quit to stop\n");
	while(1) {
		// (1) print the shell prompt
		printf("enter a cmd line: ");  
		fflush(stdout);

		// (2) read in the next command
		if ((fgets(cmdline, MAXLINE, stdin) == NULL) && ferror(stdin))
			perror("fgets error");
		if (feof(stdin)) { /* End of file (ctrl-d) */
			fflush(stdout);
			exit(0);
		}

		bg = parseArguments(cmdline, argv);

		if((argv[0]) && (strcmp(argv[0], "quit") == 0) ){
			break;
		}
		printCommandArgs(cmdline, argv);
		printBG(bg);

		bg = 0;
		args = NULL;

		// TODO: uncomment this call to test parse_cmd_dynamic
		args = parseArgumentsDynamic(cmdline, &bg);
		if(args) {
			printCommandArgs(cmdline, args);
			printBG(bg);
			int i = 0;
			while (args[i] != NULL) {
				free(args[i++]);
			}
			free(args);
		}
	}
	return 0;
}

// This function prints out a message based on the value of bg
// indicating if the command is run in the background or not
void printBG(int bg) {
	if(bg) { 
		printf("run in the background is true\n");
	} else { 
		printf("run in the background is false\n");
	}
}

// This function prints out the cmdline and argv list
//   cmdline: the command line string
//   argv: the argv list of command line argumetns string
//
void printCommandArgs(char *cmdline, char **argv ) {
	printf("\nCommand line: %s\n", cmdline);

	unsigned i=0;
	while (argv[i] != NULL) {
		// note: I'm printing out each argv string between # chars
		//       so that I can see if I have left any space or other 
		//       invisible characters in an argv[i] string (I shouldn't)
		printf("%3d  #%s#\n", i, argv[i]);
		i++;
	}
}
