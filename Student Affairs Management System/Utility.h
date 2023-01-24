//
//  Utility.h
//  Student Affairs Management System
//
//  Created by Manely Hamedani on 1/19/23.
//

#ifndef Utility_h
#define Utility_h
#include <sqlite3.h>

enum type {
    none, student, admin
};

enum result {
  success, not_found, permission_denied, invalid, end_of_line, end_of_file, insufficient_money
};

extern sqlite3 *db;

extern struct _date_time {
    char date[11];
    char time[5];
} current_date_time;

extern struct _user {
    char username[30];
    char password[20];
    enum type user_type;
} current_user;

int user_login(const char *username, const char *password);
int user_logout(const char *username);
int user_register(const char *name, const char *family, const char *user_id, const char *password, const char *national_id, const char *birthdate, const char *gender, const char *type);
int change_pass(const char *username, const char *old_pass, const char *new_pass);
int is_exists(const char *tbl_name, const char *condition);
int create_table(sqlite3 *db, const char *tbl_name, const char *definition);
int create_db(const char *db_name, sqlite3 **ppDB);

#endif /* Utility_h */
