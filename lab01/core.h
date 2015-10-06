//
//  core.h
//  employee_db
//
//  Created by Jeremy Jacobson on 9/16/14.
//  Copyright (c) 2014 Jeremy Jacobson. All rights reserved.
//

#ifndef employee_db_core_h
#define employee_db_core_h

#define MAXFILENAME  128
#define MAXNAME       64
#define MAXDBSIZE   1024

typedef enum {
	START,
    LOOKUP_BY_ID,
    LOOKUP_BY_LASTNAME,
    ADD_EMPLOYEE_ID,
    ADD_EMPLOYEE_FIRSTNAME,
    ADD_EMPLOYEE_LASTNAME,
    ADD_EMPLOYEE_SALARY,
    ADD_EMPLOYEE_VERIFY,
    REMOVE_EMPLOYEE_ID,
    REMOVE_EMPLOYEE_VERIFY,
    UPDATE_EMPLOYEE_FIND,
    UPDATE_EMPLOYEE_CHOOSE,
    UPDATE_EMPLOYEE_ID,
    UPDATE_EMPLOYEE_FIRSTNAME,
    UPDATE_EMPLOYEE_LASTNAME,
    UPDATE_EMPLOYEE_SALARY,
    HIGHEST_SALARIES,
    FIND_ALL_LASTNAME
} state_t;

typedef struct employee {
    unsigned int id;
    char *first_name;
    char *last_name;
    unsigned int salary;
} employee_t;

/**
 * Gets the filename from the command line
 *
 * @param filename      the filename
 * @param argc          argument count
 * @param argv          the actual arguments
 */
void getFilenameFromCommandLine(char filename[], int argc, const char *argv[]);

/**
 * Reads the database from the given filename
 *
 * @param db            the database
 * @param db_size       the database size
 * @param filename      the filename string
 * @return 0
 */
int read_database(employee_t ***db, size_t *db_size, char filename[]);

/**
 * The run loop
 *
 * @param db            the database
 * @param db_size       the database size
 * @return 0 if success -1 if fail
 */
int run_loop(employee_t ***db, size_t *db_size);

#endif
