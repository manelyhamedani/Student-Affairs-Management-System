//
//  Utility.c
//  Student Affairs Management System
//
//  Created by Manely Hamedani on 1/19/23.
//

#include "Utility.h"
#include <string.h>
#include <stdio.h>
#include <time.h>


struct _user current_user;
struct _date_time current_date_time;

int ID[7] = {1, 1, 1, 1, 1, 1, 1};
char *student_affairs[] = { "login", "logout", "register", "reserve", "take_food", "charge_account", "send_charge", "cancel_reserve", "daily_reserve", "define_agent", "change_self", "check_news", "vote", "recieve_charge_by_admin", "recieve_charge_by_student", "change_pass", "change_pass_by_admin", "change_datetime" };

char *system_affairs[] = { "food_reserved", "food_taken", "daily_reserved", "dessert_reserved", "dessert_taken", "cancel_reserved" };

struct tm *current_time;
struct tm *last_time;

int print_data(void *data, int argc, char **argv, char **col_name) {
    *((int *)data) = success;
    for (int i = 0; i < argc; ++i) {
        printf("%s: %s\n", col_name[i], argv[i]);
    }
    return 0;
}

int get_data(const char *columns, const char *tbl_name) {
    char *errmsg = NULL;
    char sql[max_size];
    sprintf(sql, "select %s from %s;", columns, tbl_name);
    int exist = not_found;
    int rc = sqlite3_exec(db, sql, print_data, &exist, &errmsg);
    if (rc != SQLITE_OK) {
        printf("SQL error: \n%s", errmsg);
        sqlite3_free(errmsg);
        return sql_err;
    }
    if (exist == not_found) {
        return not_found;
    }
    return success;
}

void get_date_time(void) {
    time_t t;
    time(&t);
    current_time = localtime(&t);
}

void set_date_time(void) {
    int year, month, day;
    int hour, minute;
    sscanf(current_date_time.date, "%d-%d-%d", &year, &month, &day);
    sscanf(current_date_time.time, "%2d%2d", &hour, &minute);
    get_date_time();
    int new_year = current_time->tm_year - last_time->tm_year + year;
    int new_month = current_time->tm_mon - last_time->tm_mon + month;
    int new_day = current_time->tm_mday - last_time->tm_mday + day;
    int new_hour = current_time->tm_hour - last_time->tm_hour + hour;
    int new_minute = current_time->tm_min - last_time->tm_min + minute;
    sprintf(current_date_time.date, "%04d-%02d-%02d", new_year, new_month, new_day);
    sprintf(current_date_time.time, "%02d%02d", new_hour, new_minute);
    last_time = current_time;
}


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
        fprintf(stderr, "SQL error: \n%s\n", errmsg);
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
        student_report(_login, username, 0);
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
        if (current_user.user_type == student) {
            student_report(_logout, username, 0);
        }
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
        student_report(_register, user_id, 0);
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
        fprintf(stderr, "SQL error: \n%s\n", errmsg);
        sqlite3_free(errmsg);
        return sql_err;
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
        student_report(_change_pass, username, 0);
    }
    int rc = sqlite3_exec(db, sql, NULL, NULL, &errmsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: \n%s\n", errmsg);
        sqlite3_free(errmsg);
        return sql_err;
    }
    return success;
}


int create_table(sqlite3 *db, const char *tbl_name, const char *definition) {
    char *errmsg = NULL;
    char sql[max_size];
    sprintf(sql, "create table if not exists %s (%s);", tbl_name, definition);
    int rc = sqlite3_exec(db, sql, NULL, NULL, &errmsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: \n%s\n", errmsg);
        sqlite3_free(errmsg);
        return sql_err;
    }
    return 0;
}

int create_db(const char *db_name, sqlite3 **ppDB) {
    char *errmsg = NULL;
    int rc = sqlite3_open(db_name, ppDB);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: \n%s\n", errmsg);
        sqlite3_free(errmsg);
        return sql_err;
    }
    return 0;
}


void change_datetime(const char *date, const char *time) {
    strcpy(current_date_time.date, date);
    strcpy(current_date_time.time, time);
    last_time = current_time;
    if (current_user.user_type == student) {
        student_report(_change_datetime, current_user.username, 0);
    }
}

void student_report(int affair_id, const char *student_id, double balance_change) {
    char sql[max_size];
    char *errmsg = NULL;
    sprintf(sql, "insert into STUDENT_REPORTS values(%d, '%s', '%s', '%s', %lf);", ID[student_report_id], student_affairs[affair_id], student_id, current_date_time.date, balance_change);
    int rc = sqlite3_exec(db, sql, NULL, NULL, &errmsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: \n%s\n", errmsg);
        sqlite3_free(errmsg);
    }
    ++ID[student_report_id];
}

void system_report(int affair_id, double amount) {
    char sql[max_size];
    char *errmsg = NULL;
    sprintf(sql, "insert into SYSTEM_REPORTS values(%d, '%s', '%s', %lf);", ID[system_report_id], system_affairs[affair_id], current_date_time.date, amount);
    int rc = sqlite3_exec(db, sql, NULL, NULL, &errmsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: \n%s\n", errmsg);
        sqlite3_free(errmsg);
    }
    ++ID[system_report_id];
}

