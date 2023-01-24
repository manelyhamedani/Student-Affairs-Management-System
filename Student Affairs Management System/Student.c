//
//  Student.c
//  Student Affairs Management System
//
//  Created by Manely Hamedani on 1/19/23.
//

#include "Student.h"
#include "Utility.h"
#include <stdio.h>
#include <string.h>

#define max_size    500

int reserved_meal_id = 1;
int taken_meal_id = 1;

enum gender {
    male, female
};

static int callback(void *data, int argc, char **argv, char **col_name) {
    if (strcmp(argv[0], "boyish") == 0) {
        *((int *)data) = male;
    }
    else if ( strcmp(argv[0], "girlish") == 0){
        *((int *)data) = female;
    }
    else if ( strcmp(argv[0], "male") == 0){
        *((int *)data) = male;
    }
    else if (strcmp(argv[0], "female") == 0){
        *((int *)data) = female;
    }
    else {
        sscanf(argv[0], "%d", (int *)data);
    }
    return 0;
}

int reserve(const char *self_id, const char *date, const char *meal, const char *food_id) {
    if (current_user.user_type != student) {
        return permission_denied;
    }
    char condition[max_size];
    sprintf(condition, "where self_id = %s and food_id = %s and type = '%s' and date = '%s' and count > 0 ", self_id, food_id, meal, date);
    if (is_exists("MEAL_PLAN", condition) != 1) {
        return not_found;
    }
    char *errmsg = NULL;
    char sql[max_size];
    int type;
    int gender;
    sprintf(sql, "select type from SELF where self_id = %s;", self_id);
    int rc = sqlite3_exec(db, sql, callback, &type, &errmsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", errmsg);
        sqlite3_free(errmsg);
        return permission_denied;
    }
    sprintf(sql, "select gender from STUDENTS where student_id = '%s';", current_user.username);
    rc = sqlite3_exec(db, sql, callback, &gender, &errmsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", errmsg);
        sqlite3_free(errmsg);
        return permission_denied;
    }
    if (type != gender) {
        return permission_denied;
    }
    int is_enough;
    sprintf(sql, "select (select price from FOOD where food_id = %s) <= (select balance from STUDENTS where student_id = '%s');", food_id, current_user.username);
    rc = sqlite3_exec(db, sql, callback, &is_enough, &errmsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", errmsg);
        sqlite3_free(errmsg);
        return permission_denied;
    }
    if (is_enough == 0) {
        return insufficient_money;
    }
    sprintf(sql, "insert into RESERVED_MEAL (reserved_meal_id, student_id, self_id, food_id, type, date, daily_reserved) values(%d, '%s', %s, %s, '%s', '%s', %d);", reserved_meal_id, current_user.username, self_id, food_id, meal, date, 0);
    rc = sqlite3_exec(db, sql, NULL, NULL, &errmsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", errmsg);
        sqlite3_free(errmsg);
        return permission_denied;
    }
    sprintf(sql, "update MEAL_PLAN set count = count - 1 where self_id = %s and food_id = %s and type = '%s' and date = '%s';", self_id, food_id, meal, date);
    rc = sqlite3_exec(db, sql, NULL, NULL, &errmsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", errmsg);
        sqlite3_free(errmsg);
        return permission_denied;
    }
    sprintf(sql, "update STUDENTS set balance = balance - (select price from FOOD where food_id = %s) where student_id = '%s';", food_id, current_user.username);
    rc = sqlite3_exec(db, sql, NULL, NULL, &errmsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", errmsg);
        sqlite3_free(errmsg);
        return permission_denied;
    }
    ++reserved_meal_id;
    return success;
}

int take_food(const char *self_id, const char *date, const char *meal) {
    if (current_user.user_type != student) {
        return permission_denied;
    }
    char condition[max_size];
    sprintf(condition, "where self_id = %s and date = '%s' and type = '%s' ", self_id, date, meal);
    if (is_exists("MEAL_PLAN", condition) != 1) {
        return not_found;
    }
    sprintf(condition, "where student_id = '%s' and self_id = %s and date = '%s' and type = '%s' ", current_user.username, self_id, date, meal);
    if (is_exists("RESERVED_MEAL", condition) != 1) {
        return permission_denied;
    }
    char *errmsg = NULL;
    char sql[max_size];
    int food_id;
    sprintf(sql, "select food_id from RESERVED_MEAL where student_id = '%s' and self_id = %s and date = '%s' and type = '%s' ", current_user.username, self_id, date, meal);
    int rc = sqlite3_exec(db, sql, callback, &food_id, &errmsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", errmsg);
        sqlite3_free(errmsg);
        return permission_denied;
    }
    sprintf(sql, "insert into TAKEN_MEAL values(%d, '%s', %s, %d, '%s');", taken_meal_id, current_user.username, self_id, food_id, date);
    rc = sqlite3_exec(db, sql, NULL, NULL, &errmsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", errmsg);
        sqlite3_free(errmsg);
        return permission_denied;
    }
    ++taken_meal_id;
    return success;
}

