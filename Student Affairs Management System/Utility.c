//
//  Utility.c
//  Student Affairs Management System
//
//  Created by Manely Hamedani on 1/19/23.
//

#include "Utility.h"
#include <string.h>
#include <stdio.h>

struct _user current_user;
#define max_size    500

static int callback(void *data, int argc, char **argv, char **col_name) {
    if (strcmp(argv[0], "1") == 0) {
        *((int *)data) = 1;
    }
    return 0;
}

int is_exists(const char *tbl_name, const char *condition) {
    char *errmsg = NULL;
    char sql[max_size];
    sprintf(sql, "select exists(select * from %s ", tbl_name);
    strcat(sql, condition);
    strcat(sql, ");");
    int exist = 0;
    int rc = sqlite3_exec(db, sql, callback, &exist, &errmsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", errmsg);
        sqlite3_free(errmsg);
        return -1;
    }
    if (exist == 1) {
        return 1;
    }
    return 0;
}

int user_login(const char *username, const char *password) {
    if (current_user.user_type != none) {
        return permission_denied;
    }
    char condition[max_size];
    sprintf(condition, "where student_id = '%s' and password = '%s' and activate = 1 ", username, password);
    if (is_exists("STUDENTS", condition) == 1) {
        strcpy(current_user.username, username);
        strcpy(current_user.password, password);
        current_user.user_type = student;
        return success;
    }
    sprintf(condition, "where admin_id = '%s' and password = '%s' ", username, password);
    if (is_exists("ADMINS", condition) == 1) {
        strcpy(current_user.username, username);
        strcpy(current_user.password, password);
        current_user.user_type = admin;
        return success;
    }
    return not_found;
}

int user_logout(const char *username) {
    if (strcmp(username, current_user.username) == 0) {
        strcpy(current_user.username, "\0");
        strcpy(current_user.password, "\0");
        current_user.user_type = none;
        return success;
    }
    return not_found;
}

int user_register(const char *name, const char *family, const char *user_id, const char *password, const char *national_id, const char *birthdate, const char *gender, const char *type) {
    char sql[max_size];
    char *errmsg = NULL;
    char condition[max_size];
    sprintf(condition, "where user_id = '%s' and password = '%s' ", user_id, password);
    if (is_exists("PENDING", condition) == 1) {
        return permission_denied;
    }
    if (strcmp(type, "student") == 0) {
        sprintf(condition, "where student_id = '%s' and password = '%s' ", user_id, password);
        if (is_exists("STUDENTS", condition) == 1) {
            return permission_denied;
        }
    }
    else {
        sprintf(condition, "where admin_id = '%s' and password = '%s' ", user_id, password);
        if (is_exists("ADMINS", condition) == 1) {
            return permission_denied;
        }
    }
    sprintf(sql, "insert into PENDING values ('%s', '%s', '%s', '%s', '%s', '%s', '%s','%s');", user_id, name, family, password, national_id, birthdate, gender, type);
    int rc = sqlite3_exec(db, sql, NULL, NULL, &errmsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", errmsg);
        sqlite3_free(errmsg);
        return -1;
    }
    return success;
}

int change_pass(const char *username, const char *old_pass, const char *new_pass) {
    if (strcmp(current_user.username, username)) {
        return not_found;
    }
    if (strcmp(current_user.password, old_pass)) {
        return permission_denied;
    }
    char *errmsg = NULL;
    char sql[max_size];
    if (current_user.user_type == admin) {
        sprintf(sql, "update ADMINS set password = '%s' where admin_id = '%s';", new_pass, username);
    }
    else {
        sprintf(sql, "update STUDENTS set password = '%s' where student_id = '%s';", new_pass, username);
    }
    int rc = sqlite3_exec(db, sql, NULL, NULL, &errmsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", errmsg);
        sqlite3_free(errmsg);
        return -1;
    }
    return success;
}


int create_table(sqlite3 *db, const char *tbl_name, const char *definition) {
    char *errmsg = NULL;
    char sql[max_size];
    sprintf(sql, "create table if not exists %s (%s);", tbl_name, definition);
    int rc = sqlite3_exec(db, sql, NULL, NULL, &errmsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", errmsg);
        sqlite3_free(errmsg);
        return -1;
    }
    return 0;
}

int create_db(const char *db_name, sqlite3 **ppDB) {
    char *errmsg = NULL;
    int rc = sqlite3_open(db_name, ppDB);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", errmsg);
        sqlite3_free(errmsg);
        return -1;
    }
    return 0;
}




