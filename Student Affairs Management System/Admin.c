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
    *((int *)data) = success;
    char sql[max_size];
    int student_flag = 0;
    if (strcmp(argv[argc - 1], "admin") == 0) {
        sprintf(sql, "insert into ADMINS values ('%s'", argv[0]);
    }
    else {
        sprintf(sql, "insert into STUDENTS values ('%s'", argv[0]);
        student_flag = 1;
    }
    for (int i = 1; i < argc - 1; ++i) {
        strcat(sql, ", '");
        strcat(sql, argv[i]);
        strcat(sql, "'");
    }
    if (student_flag) {
        strcat(sql, ", 0.0, 1");
    }
    strcat(sql, ");");
    char *errmsg = NULL;
    int rc = sqlite3_exec(db, sql, NULL, NULL, &errmsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", errmsg);
        sqlite3_free(errmsg);
        return -1;
    }
    return 0;
}

int approve(const char *user_id) {
    char *errmsg = NULL;
    char sql[max_size];
    int exist = not_found;
    sprintf(sql, "select * from PENDING where user_id = '%s';", user_id);
    int rc = sqlite3_exec(db, sql, callback, &exist, &errmsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", errmsg);
        sqlite3_free(errmsg);
        return -1;
    }
    if (exist == not_found) {
        return not_found;
    }
    sprintf(sql, "delete from PENDING where user_id = '%s';", user_id);
    rc = sqlite3_exec(db, sql, NULL, NULL, &errmsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", errmsg);
        sqlite3_free(errmsg);
        return -1;
    }
    return success;
}

int change_student_pass(const char *user_id, const char *new_pass) {
    if (current_user.user_type != admin) {
        return permission_denied;
    }
    if (is_exists("STUDENTS", user_id, NULL, "student") != 1) {
        return not_found;
    }
    char *errmsg = NULL;
    char sql[max_size];
    sprintf(sql, "update STUDENTS set password = '%s' where student_id = '%s';", new_pass, user_id);
    int rc = sqlite3_exec(db, sql, NULL, NULL, &errmsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", errmsg);
        sqlite3_free(errmsg);
        return -1;
    }
    return success;
}

int remove_student(const char *user_id) {
    if (current_user.user_type != admin) {
        return permission_denied;
    }
    if (is_exists("STUDENTS", user_id, NULL, "student") != 1) {
        return not_found;
    }
    char *errmsg = NULL;
    char sql[max_size];
    sprintf(sql, "delete from STUDENTS where student_id = '%s';", user_id);
    int rc = sqlite3_exec(db, sql, NULL, NULL, &errmsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", errmsg);
        sqlite3_free(errmsg);
        return -1;
    }
    return success;
}



