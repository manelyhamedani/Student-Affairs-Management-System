//
//  Admin.c
//  Student Affairs Management System
//
//  Created by Manely Hamedani on 1/19/23.
//

#include "Admin.h"
#include "Utility.h"
#include <stdio.h>
#include <string.h>

#define max_size    500

static int callback(void *data, int argc, char **argv, char **col_name) {
    char sql[max_size];
    sprintf(sql, "insert into ADMINS values ('%s'", argv[0]);
    for (int i = 1; i < argc - 1; ++i) {
        strcat(sql, ", '");
        strcat(sql, argv[i]);
        strcat(sql, "'");
    }
    strcat(sql, ");");
    char *errmsg = NULL;
    int rc = sqlite3_exec((sqlite3 *)data, sql, NULL, NULL, &errmsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", errmsg);
        sqlite3_free(errmsg);
        return -1;
    }
    return 0;
}

int approve_admin(sqlite3 *db, const char *admin_id) {
    char *errmsg = NULL;
    char sql[max_size];
    sprintf(sql, "select * from PENDING where user_id = '%s' and type = '%s';", admin_id, "admin");
    int rc = sqlite3_exec(db, sql, callback, db, &errmsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", errmsg);
        sqlite3_free(errmsg);
        return -1;
    }
    return 0;
}


