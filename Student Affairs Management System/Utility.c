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

enum result {
  success, not_found, permission_denied, invalid
};

static int callback(void *data, int argc, char **argv, char **col_name) {
    if (strcmp(argv[0], "1") == 0) {
        *((int *)data) = 1;
    }
    return 0;
}

int is_exists(sqlite3* db, const char *tbl_name, const char *id, const char *type) {
    char *errmsg = NULL;
    char sql[max_size];
    sprintf(sql, "select exists(select * from %s where %s_id = %s);", tbl_name, type, id);
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

int user_login(const char *username, const char *password, sqlite3 *db) {
    if (current_user.user_type != none) {
//        fprintf(stderr, "Cannot login!\n");
        return permission_denied;
    }
    if (is_exists(db, "STUDENTS", username, "student") == 1) {
        strcpy(current_user.username, username);
        strcpy(current_user.password, password);
        current_user.user_type = student;
        return success;
    }
    if (is_exists(db, "ADMINS", username, "admin") == 1) {
        strcpy(current_user.username, username);
        strcpy(current_user.password, password);
        current_user.user_type = admin;
        return success;
    }
    fprintf(stderr, "Incorrect username or password.\n");
    return not_found;
}

int user_logout(const char *username) {
    if (strcmp(username, current_user.username) == 0) {
        strcpy(current_user.username, "\0");
        strcpy(current_user.password, "\0");
        current_user.user_type = none;
        return success;
    }
    fprintf(stderr, "Incorrect username.\n");
    return not_found;
}


int user_register(const char *name, const char *family, const char *user_id, const char *password, const char *national_id, const char *birthdate, const char *gender, const char *type, sqlite3 *db) {
    char sql[max_size];
    char *errmsg = NULL;
    if (is_exists(db, "PENDING", user_id, "user") == 1) {
        return permission_denied;
    }
    if (strcmp(type, "student") == 0) {
        if (is_exists(db, "STUDENTS", user_id, type) == 1) {
            return permission_denied;
        }
    }
    else {
        if (is_exists(db, "ADMINS", user_id, type) == 1) {
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




