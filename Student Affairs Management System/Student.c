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
    sprintf(sql, "insert into RESERVED_MEAL (reserved_meal_id, student_id, self_id, food_id, type, date, daily_reserved) values(%d, '%s', %s, %s, '%s', '%s', %d);", reserved_meal_id, current_user.username, self_id, food_id, meal, date, 0);
    int rc = sqlite3_exec(db, sql, NULL, NULL, &errmsg);
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
    ++reserved_meal_id;
    return success;
}
