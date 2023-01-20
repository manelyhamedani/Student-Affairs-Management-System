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

extern struct _user {
    char username[30];
    char password[20];
    enum type user_type;
} current_user;

int user_login(const char *username, const char *password, sqlite3 *db);
int user_logout(const char *username);
int user_register(const char *name, const char *family, const char *user_id, const char *password, const char *national_id, const char *birthdate, const char *gender, const char *type, sqlite3 *db);
int create_table(sqlite3 *db, const char *tbl_name, const char *definition);
int create_db(const char *db_name, sqlite3 **ppDB);

#endif /* Utility_h */
