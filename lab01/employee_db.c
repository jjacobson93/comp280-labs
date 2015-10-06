//
//  employee_db.c
//  employee_db
//
//  Created by Jeremy Jacobson on 9/14/14.
//  Copyright (c) 2014 Jeremy Jacobson. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "readfile.h"
#include "core.h"

int main(int argc, const char * argv[])
{
	char filename[MAXFILENAME];
    int db_result;
    employee_t **db = NULL;
    size_t db_size = 0;

	// this initializes the filename string from the command line arguments
	getFilenameFromCommandLine(filename, argc, argv);
    
	// Read database
    db_result = read_database(&db, &db_size, filename);
    if (db_result) return db_result;
    
    return run_loop(&db, &db_size);
}
