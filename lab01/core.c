//
//  core.c
//  employee_db
//
//  Created by Jeremy Jacobson on 9/16/14.
//  Copyright (c) 2014 Jeremy Jacobson. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "core.h"
#include "readfile.h"
#include "sort.h"

#define PRINT_EMPLOYEE_HEADER() printf("%-8s%-15s%-15s%-10s\n", "ID", "First Name", "Last Name", "Salary"); \
    printf("---------------------------------------------\n")

/*** Helper functions ***/

/**
 * Trims the new line character from a string
 * given its length, modifying both
 *
 * @param s     pointer to a string
 * @param len   pointer to the length
 */
void trim_newline(char **s, size_t *len) {
    size_t i = *len - 1;
    if ((*s)[i] == '\n') { // check if last character (not \0) is new line
        (*s)[i] = '\0';
        (*len)--;
    }
}

/**
 * Creates a new employee_t
 *
 * @param id            the employee's id
 * @param first_name    the employee's first_name
 * @param last_name     the employee's last_name
 * @param salary        the employee's salary
 * @return the new employee
 */
employee_t *new_employee(unsigned int id, char *first_name, char *last_name, unsigned int salary) {
    employee_t *e = malloc(sizeof(employee_t));
    e->id = id;
    e->first_name = first_name;
    e->last_name = last_name;
    e->salary = salary;
    return e;
}

/**
 * Prints the employee and, conditionally, a header
 *
 * @param e         employee_t pointer
 * @param header    boolean to print the header
 */
void print_employee(employee_t *e, unsigned int header) {
    if (header) {
        PRINT_EMPLOYEE_HEADER();
    }
    
    printf("%-8u%-15s%-15s$%6u\n", e->id, e->first_name, e->last_name, e->salary);
}

/**
 * Prints the db
 *
 * @param db            the database
 * @param db_size       the database size
 */
void print_db(employee_t **db, size_t db_size) {
    PRINT_EMPLOYEE_HEADER();
    
    for (int i = 0; i < db_size; i++) {
        print_employee(db[i], 0);
    }
    
    printf("(%lu records)\n\n", db_size);
}

/**
 * Compares two employees' ids
 *
 * @param a
 * @param b
 * @return a boolean
 */
int compare_employee(employee_t *a, employee_t *b) {
    return a->id < b->id;
}

/**
 * Sorts the db
 *
 * @param db            the database
 * @param db_size       the database size
 */
void sort_db(employee_t ***db, size_t db_size) {
    quicksort((void**)(*db), db_size, 0, (int)db_size - 1, (int(*)(void*, void*))&compare_employee);
}

/**
 * Binary search on the target from start to end
 *
 * @param db            the database
 * @param db_size       the database size
 * @param target    what we're looking for
 * @param start     start index
 * @param end       end index
 * @return index of the target, returns -1 if not found
 */
int binary_search(employee_t **db, int target, int start, int end) {
    if (start > end) return -1;
    
    int mid = (start + end)/2;
    int val = db[mid]->id;
    
    if (val > target) return binary_search(db, target, start, mid - 1);
    
    if (val < target) return binary_search(db, target, mid + 1, end);
    
    return mid;
}

/**
 * Checks a string for validity
 *
 * @param s     the string to be checked
 * @param l     the length of the string
 * @return boolean
 */
int is_valid_str(char *s, size_t l) {
    return l <= 64 && !strstr(s, " ");
}

/*** db functions ***/

/**
 * Finds the index for the employee with the given id
 *
 * @param db            the database
 * @param db_size       the database size
 * @param id    employee's id
 * @return index of employee, returns -1 if not found
 */
int find_by_id(employee_t **db, size_t db_size, int id) {
    return binary_search(db, id, 0, (int)db_size - 1);
}

/**
 * Finds the employee with the given last name
 *
 * @param db            the database
 * @param db_size       the database size
 * @param last_name     the last_name of the employee
 * @return index of employee, returns -1 if not found
 */
int find_by_last_name(employee_t **db, size_t db_size, char *last_name) {
    for (int i = 0; i < db_size; i++) {
        if (strcmp(db[i]->last_name, last_name) == 0) {
            return i;
        }
    }
    
    return -1;
}

/**
 * Compares salaries of two employees
 *
 * @param a         first employee
 * @param b         second employee
 * @return 1 if a's salary is greater than b's, else 0
 */
int compare_salaries(employee_t *a, employee_t *b) {
    return a->salary > b->salary;
}

/**
 * Finds all employees by last name
 *
 * @param db            the database
 * @param db_size       the database size
 * @param last_name     the last_name of the employee
 */
void find_all_by_last_name(employee_t **db, size_t db_size, char *last_name) {
    PRINT_EMPLOYEE_HEADER();
    
    for (int i = 0; i < db_size; i++) {
        if (strcmp(db[i]->last_name, last_name) == 0) {
            print_employee(db[i], 0);
        }
    }
}

/**
 * Finds the num highest salaries
 *
 * @param db            the database
 * @param db_size       the database size
 * @param num   the number of salaries
 */
void find_highest_salaries(employee_t **db, size_t db_size, unsigned long num) {
    employee_t **highest = calloc(num, sizeof(employee_t));
    
    for (int i = 0; i < db_size; i++) {
        int idx;
        // find insertion point
        for (idx = 0; idx < num; idx++) {
            if (!highest[idx] || db[i]->salary > highest[idx]->salary) {
                break;
            }
        }
        
        // shift all elements above
        for (int j = (int)num - 1; j >= idx; j--) {
            highest[j+1] = highest[j];
            highest[j] = NULL;
        }
        
        highest[idx] = db[i];
    }
    
    PRINT_EMPLOYEE_HEADER();
    for (int i = 0; i < num; i++) {
        print_employee(highest[i], 0);
    }
    
    printf("\n");
    
    free(highest);
    highest = NULL;
}

/**
 * Inserts employee into the database
 *
 * @param db            the database
 * @param db_size       the database size
 * @param e     the new employee
 * @return the index of the employee
 */
int insert_employee(employee_t ***db, size_t *db_size, employee_t *e) {
    if (*db_size < MAXDBSIZE) {
        int i;
        // find insertion point
        for (i = 0; i < *db_size; i++) {
            if (e->id < (*db)[i]->id) {
                break;
            }
        }
        
        // shift all elements above
        for (int j = (int)*db_size; j >= i; j--) {
            (*db)[j+1] = (*db)[j];
        }
        
        (*db)[i] = e;
        (*db_size)++;
        
        return i;
    } else {
        return -1;
    }
}

/**
 * Removes an employee at the given index
 * 
 * @param db            the database
 * @param db_size       the database size
 * @param idx   index of the employee
 */
void remove_employee(employee_t ***db, size_t *db_size, int idx) {
    free((*db)[idx]);
    (*db)[idx] = NULL;
    
    for (int i = idx; i < (*db_size - 1); i++) {
        (*db)[i] = (*db)[i + 1];
    }
    
    (*db_size)--;
}

/*** Reading functions ***/

/**
 * Gets the filename from the command line
 *
 * @param filename      the filename
 * @param argc          argument count
 * @param argv          the actual arguments
 */
void getFilenameFromCommandLine(char filename[], int argc, const char *argv[]) {
	if (argc != 2) {
		printf("Usage: %s database_file\n", argv[0]);
		// exit function: quits the program immediately...some errors are not
		// recoverable by the program, so exiting with an error message is
		// reasonable error handling option in this case
		exit(1);
	}
	if (strlen(argv[1]) >= MAXFILENAME) {
		printf("Filename, %s, is too long, cp to shorter name and try again\n",
               filename);
		exit(1);
	}
	strcpy(filename, argv[1]);
}

/**
 * Reads a line from the database file
 *
 * @param id            pointer to the id
 * @param first_name    pointer to the first_name
 * @param last_name     pointer to the last_name
 * @param salary        pointer to the salary
 * @return success: 0 otherwise -1 if fail or EOF
 */
int read_line(int *id, char **first_name, char **last_name, unsigned int *salary) {
    int ret;
    
    // read id
    ret = readInt(id);
    if (ret) return ret;
   
    // read first_name
    ret = readString(*first_name);
    if (ret) return ret;

    // read last_name
    ret = readString(*last_name);
    if (ret) return ret;

    // read salary
    ret = readInt((int*)salary);
    if (ret) return ret;
    
    return ret;
}

/**
 * Reads the database from the given filename
 *
 * @param db            the database
 * @param db_size       the database size
 * @param filename      the filename string
 * @return 0
 */
int read_database(employee_t ***db, size_t *db_size, char filename[]) {
    int file_result = openFile(filename);
    if (file_result) {
        fprintf(stderr, "Could not read file: %s\n", filename);
        return file_result;
    }
    
    *db = malloc(sizeof(employee_t)*MAXDBSIZE);
    int curr = 0;
    
    int reading = 1;
    int ret = 0;
    while (reading) {
        // employee vars
        employee_t *new_e;
        int id;
        char *first_name = malloc(sizeof(char)*MAXNAME), *last_name = malloc(sizeof(char)*MAXNAME);
        unsigned int salary;
        
        ret = read_line(&id, &first_name, &last_name, &salary);
        if (ret == -1) break;
        
        new_e = new_employee(id, first_name, last_name, salary);
        
        (*db)[curr++] = new_e;
    }
    
    *db_size = curr;
    
    sort_db(db, *db_size);
    
    closeFile();
    return 0;
}

/**
 * The run loop
 *
 * @param db            the database
 * @param db_size       the database size
 * @return 0 if success -1 if fail
 */
int run_loop(employee_t ***db, size_t *db_size) {
    state_t current_state = START;
    char *line = NULL;
    char *endptr;
    size_t linelen = 0;
    size_t linecap = 0;
    unsigned long opt = 0;
    unsigned long id = 0;
    unsigned long salary = 0;
    unsigned long num = 0;
    int idx = -1;
    employee_t *new_e = NULL;
    
    while (1) {
		switch (current_state) {
			case START:
				printf("1. Print the database\n");
				printf("2. Lookup by ID\n");
				printf("3. Lookup by last name\n");
				printf("4. Add an employee\n");
				printf("5. Quit\n");
                printf("6. Remove an employee\n");
                printf("7. Update an employee\n");
                printf("8. Print employees with highest salaries\n");
                printf("9. Find all with last name\n");
				printf(">>> ");
				break;
            case LOOKUP_BY_ID:
                printf("Enter ID >>> ");
                break;
            case LOOKUP_BY_LASTNAME:
                printf("Enter last name >>> ");
                break;
            case ADD_EMPLOYEE_ID:
                printf("ID (6 digits): ");
                break;
            case ADD_EMPLOYEE_FIRSTNAME:
                printf("First name: ");
                break;
            case ADD_EMPLOYEE_LASTNAME:
                printf("Last name: ");
                break;
            case ADD_EMPLOYEE_SALARY:
                printf("Salary (30,000 to 150,000): ");
                break;
            case ADD_EMPLOYEE_VERIFY:
                printf("\nAre you sure you would like to add this employee?\n");
                print_employee(new_e, 1);
                printf("\nY/N? ");
                break;
            case REMOVE_EMPLOYEE_ID:
                printf("Remove employee: Enter ID >>> ");
                break;
            case REMOVE_EMPLOYEE_VERIFY:
                printf("\nAre you sure you would like to remove employee with ID %lu?\n", id);
                print_employee((*db)[idx], 1);
                printf("\nY/N? ");
                break;
            case UPDATE_EMPLOYEE_FIND:
                printf("Update employee: Enter ID >>> ");
                break;
            case UPDATE_EMPLOYEE_CHOOSE:
                printf("Choose field to update:\n");
                printf("1. ID\n");
                printf("2. First name\n");
                printf("3. Last name\n");
                printf("4. Salary\n");
                printf("5. Back\n");
                printf(">>> ");
                break;
            case UPDATE_EMPLOYEE_ID:
                printf("Enter new ID >>> ");
                break;
            case UPDATE_EMPLOYEE_FIRSTNAME:
                printf("Enter new first name >>> ");
                break;
            case UPDATE_EMPLOYEE_LASTNAME:
                printf("Enter new last name >>> ");
                break;
            case UPDATE_EMPLOYEE_SALARY:
                printf("Enter new salary >>> ");
                break;
            case HIGHEST_SALARIES:
                printf("Enter number of salaries >>> ");
                break;
            case FIND_ALL_LASTNAME:
                printf("Enter last name >>> ");
                break;
                
		}
        
        // Read option
        if ((linelen = getline(&line, &linecap, stdin)) != -1) {
            trim_newline(&line, &linelen);
            if (*line != '\0') { // Line is not null
                switch (current_state) {
                        
                    // Initial menu
                    case START:
                        opt = strtoul(line, &endptr, 10);
                        if (*endptr == '\0') {
                            // Go to the correct state
                            switch (opt) {
                                case 1:
                                    print_db(*db, *db_size);
                                    continue;
                                    break;
                                case 2:
                                    current_state = LOOKUP_BY_ID;
                                    continue;
                                    break;
                                case 3:
                                    current_state = LOOKUP_BY_LASTNAME;
                                    continue;
                                    break;
                                case 4:
                                    current_state = ADD_EMPLOYEE_ID;
                                    printf("New employee\n");
                                    printf("==============\n");
                                    continue;
                                    break;
                                case 5:
                                    return 0;
                                case 6:
                                    current_state = REMOVE_EMPLOYEE_ID;
                                    continue;
                                    break;
                                case 7:
                                    current_state = UPDATE_EMPLOYEE_FIND;
                                    continue;
                                    break;
                                case 8:
                                    current_state = HIGHEST_SALARIES;
                                    continue;
                                    break;
                                case 9:
                                    current_state = FIND_ALL_LASTNAME;
                                    continue;
                                    break;
                            }
                        }
                        break;
                        
                    // Lookup by ID
                    case LOOKUP_BY_ID:
                        id = strtoul(line, &endptr, 10);
                        if (*endptr == '\0') {
                            idx = find_by_id(*db, *db_size, (unsigned int)id);
                            if (idx != -1) {
                                print_employee((*db)[idx], 1);
                                printf("\n");
                            } else {
                                printf("Employee with ID %lu does not exist\n\n", id);
                            }
                        } else {
                            printf("%s is not a valid ID\n\n", line);
                        }
                        
                        current_state = START;
                        continue;
                        break;
                    
                    // Lookup by last name
                    case LOOKUP_BY_LASTNAME:
                        idx = find_by_last_name(*db, *db_size, line);
                        if (idx != -1) {
                            print_employee((*db)[idx], 1);
                            printf("\n");
                        } else {
                            printf("Employee with last name %s does not exist\n\n", line);
                        }
                        
                        current_state = START;
                        continue;
                        break;
                    
                    // Add employee id
                    case ADD_EMPLOYEE_ID:
                        id = strtoul(line, &endptr, 10);
                        if (*endptr == '\0' && id >= 100000 && id <= 999999) {
                            idx = find_by_id(*db, *db_size, (unsigned int)id);
                            if (idx == -1) {
                                new_e = new_employee((unsigned int)id, NULL, NULL, 0);
                                current_state = ADD_EMPLOYEE_FIRSTNAME;
                            } else {
                                printf("Employee with ID %lu already exists. Please enter a different ID.\n\n", id);
                            }
                        } else {
                            printf("%s is not a valid ID. Please enter a value between 100000 and 999999 inclusive.\n\n", line);
                        }
                        
                        continue;
                        break;
                        
                    // Add employee first name
                    case ADD_EMPLOYEE_FIRSTNAME:
                        if (*line != '\0' && is_valid_str(line, linelen)) {
                            new_e->first_name = malloc(linelen*sizeof(char));
                            strcpy(new_e->first_name, line);
                            current_state = ADD_EMPLOYEE_LASTNAME;
                        } else {
                            printf("%s is not a valid first name. It must not contain spaces and must be 64 characters or less.\n\n", line);
                        }
                        
                        continue;
                        break;
                    
                    // Add employee last name
                    case ADD_EMPLOYEE_LASTNAME:
                        if (*line != '\0' && is_valid_str(line, linelen)) {
                            new_e->last_name = malloc(linelen*sizeof(char));
                            strcpy(new_e->last_name, line);
                            current_state = ADD_EMPLOYEE_SALARY;
                        } else {
                            printf("%s is not a valid last name. It must not contain spaces and must be 64 characters or less.\n\n", line);
                        }
                        
                        continue;
                        break;
                        
                    // Add employee salary
                    case ADD_EMPLOYEE_SALARY:
                        salary = strtoul(line, &endptr, 10);
                        if (*endptr == '\0' && salary >= 30000 && salary <= 150000) {
                            new_e->salary = (unsigned int)salary;
                            current_state = ADD_EMPLOYEE_VERIFY;
                        } else {
                            printf("%s is not a valid salary. Please enter a value between 30000 and 150000 inclusive.\n\n", line);
                        }
                        
                        continue;
                        break;
                        
                    // Add employee verify
                    case ADD_EMPLOYEE_VERIFY:
                        if (strcmp("Y", line) == 0 || strcmp("y", line) == 0) {
                            int idx = insert_employee(db, db_size, new_e);
                            if (idx > -1) {
                                printf("Inserted at %i\n\n", idx);
                            } else {
                                printf("Database is at it's size limit of 1024 records.\n\n");
                                free(new_e);
                                new_e = NULL;
                            }
                            
                            current_state = START;
                        } else if (strcmp("N", line) == 0 || strcmp("n", line) == 0) {
                            free(new_e);
                            new_e = NULL;
                            printf("Employee not added\n\n");
                            current_state = START;
                        } else {
                            printf("Please enter a Y or N\n\n");
                        }
                        continue;
                        break;
                    
                    // Remove employee (id)
                    case REMOVE_EMPLOYEE_ID:
                        id = strtoul(line, &endptr, 10);
                        if (*endptr == '\0' && id >= 100000 && id <= 999999) {
                            idx = find_by_id(*db, *db_size, (unsigned int)id);
                            if (idx != -1) {
                                current_state = REMOVE_EMPLOYEE_VERIFY;
                            } else {
                                printf("Employee with ID %lu does not exist.\n\n", id);
                                current_state = START;
                            }
                        } else {
                            printf("%s is not a valid ID.\n\n", line);
                            current_state = START;
                        }
                        
                        continue;
                        break;
                        
                    // Remove employee verify
                    case REMOVE_EMPLOYEE_VERIFY:
                        if (strcmp("Y", line) == 0 || strcmp("y", line) == 0) {
                            remove_employee(db, db_size, idx);
                            printf("Employee removed\n\n");
                            current_state = START;
                        } else if (strcmp("N", line) == 0 || strcmp("n", line) == 0) {
                            new_e = NULL;
                            printf("Employee not removed\n\n");
                            current_state = START;
                        } else {
                            printf("Please enter a Y or N\n\n");
                        }
                        continue;
                        break;
                        
                    // Update employee (find by id)
                    case UPDATE_EMPLOYEE_FIND:
                        id = strtoul(line, &endptr, 10);
                        if (*endptr == '\0' && id >= 100000 && id <= 999999) {
                            idx = find_by_id(*db, *db_size, (unsigned int)id);
                            if (idx != -1) {
                                printf("Updating:\n");
                                print_employee((*db)[idx], 1);
                                current_state = UPDATE_EMPLOYEE_CHOOSE;
                            } else {
                                printf("Employee with ID %lu does not exist.\n\n", id);
                                current_state = START;
                            }
                        } else {
                            printf("%s is not a valid ID.\n\n", line);
                            current_state = START;
                        }
                        
                        continue;
                        break;
                    
                    // Update choice
                    case UPDATE_EMPLOYEE_CHOOSE:
                        opt = strtoul(line, &endptr, 10);
                        if (*endptr == '\0') {
                            // Go to the correct state
                            switch (opt) {
                                case 1:
                                    current_state = UPDATE_EMPLOYEE_ID;
                                    break;
                                case 2:
                                    current_state = UPDATE_EMPLOYEE_FIRSTNAME;
                                    break;
                                case 3:
                                    current_state = UPDATE_EMPLOYEE_LASTNAME;
                                    break;
                                case 4:
                                    current_state = UPDATE_EMPLOYEE_SALARY;
                                    break;
                                case 5:
                                    current_state = START;
                                    break;
                            }
                        }
                        
                        continue;
                        break;
                    
                    // Update employee's id
                    case UPDATE_EMPLOYEE_ID:
                        id = strtoul(line, &endptr, 10);
                        if (*endptr == '\0' && id >= 100000 && id <= 999999) {
                            int found = find_by_id(*db, *db_size, (unsigned int)id);
                            if (found == -1) {
                                (*db)[idx]->id = (unsigned int)id;
                                sort_db(db, *db_size);
                                printf("Updated ID to %lu\n\n", id);
                                current_state = UPDATE_EMPLOYEE_CHOOSE;
                            } else {
                                printf("Employee with ID %lu already exists. Please enter a different ID.\n\n", id);
                            }
                        } else {
                            printf("%s is not a valid ID. Please enter a value between 100000 and 999999 inclusive.\n\n", line);
                        }
                        
                        continue;
                        break;
                    
                    // Update employee's first name
                    case UPDATE_EMPLOYEE_FIRSTNAME:
                        if (*line != '\0' && is_valid_str(line, linelen)) {
                            free((*db)[idx]->first_name);
                            (*db)[idx]->first_name = malloc(linelen*sizeof(char));
                            strcpy((*db)[idx]->first_name, line);
                            printf("Updated first name to %s\n", line);
                            current_state = UPDATE_EMPLOYEE_CHOOSE;
                        } else {
                            printf("%s is not a valid first name. It must not contain spaces and must be 64 characters or less.\n\n", line);
                        }
                        
                        continue;
                        break;
                    
                    // Update employee's last name
                    case UPDATE_EMPLOYEE_LASTNAME:
                        if (*line != '\0' && is_valid_str(line, linelen)) {
                            free((*db)[idx]->last_name);
                            (*db)[idx]->last_name = malloc(linelen*sizeof(char));
                            strcpy((*db)[idx]->last_name, line);
                            printf("Updated last name to %s\n", line);
                            current_state = UPDATE_EMPLOYEE_CHOOSE;
                        } else {
                            printf("%s is not a valid last name. It must not contain spaces and must be 64 characters or less.\n\n", line);
                        }
                        
                        continue;
                        break;
                        
                    // Update employee's salary
                    case UPDATE_EMPLOYEE_SALARY:
                        salary = strtoul(line, &endptr, 10);
                        if (*endptr == '\0' && salary >= 30000 && salary <= 150000) {
                            (*db)[idx]->salary = (unsigned int)salary;
                            printf("Updated salary to %lu\n", salary);
                            current_state = UPDATE_EMPLOYEE_CHOOSE;
                        } else {
                            printf("%s is not a valid salary. Please enter a value between 30000 and 150000 inclusive.\n\n", line);
                        }

                        continue;
                        break;
                        
                    // Find highest salaries
                    case HIGHEST_SALARIES:
                        num = strtoul(line, &endptr, 10);
                        if (*endptr == '\0' && num > 0 && num <= *db_size) {
                            find_highest_salaries(*db, *db_size, num);
                            current_state = START;
                        } else {
                            printf("%s is not a valid number. Must be > 0 and <= %lu\n\n", line, *db_size);
                        }
                        
                        continue;
                        break;
                        
                    // Find all with last name
                    case FIND_ALL_LASTNAME:
                        find_all_by_last_name(*db, *db_size, line);
                        printf("\n");
                        current_state = START;
                        continue;
                        break;
                }
            }
        }
        
        printf("Please enter a valid option.\n\n");
	}
    
    return 0;
}