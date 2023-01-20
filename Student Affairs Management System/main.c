//
//  main.c
//  Student Affairs Management System
//
//  Created by Manely Hamedani on 1/19/23.
//

#include <stdio.h>
#include "Utility.h"
#include "Admin.h"

sqlite3 *db;

int main(int argc, const char * argv[]) {
    char *db_name = "samsDB.db";
    if (create_db(db_name, &db) != 0) {
        
    }
    return 0;
}
