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
#include <stdlib.h>

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
        fprintf(stderr, "SQL error: \n%s\n", errmsg);
        sqlite3_free(errmsg);
        return -1;
    }
    return 0;
}

int approve(const char *user_id) {
    if (current_user.user_type != admin) {
        return permission_denied;
    }
    char *errmsg = NULL;
    char sql[max_size];
    int exist = not_found;
    sprintf(sql, "select * from PENDING where user_id = '%s';", user_id);
    int rc = sqlite3_exec(db, sql, callback, &exist, &errmsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: \n%s\n", errmsg);
        sqlite3_free(errmsg);
        return sql_err;
    }
    if (exist == success) {
        sprintf(sql, "delete from PENDING where user_id = '%s';", user_id);
        rc = sqlite3_exec(db, sql, NULL, NULL, &errmsg);
        if (rc != SQLITE_OK) {
            fprintf(stderr, "SQL error: \n%s\n", errmsg);
            sqlite3_free(errmsg);
            return sql_err;
        }
        return success;
    }
    char condition[max_size];
    sprintf(condition, "where student_id = '%s' and activate = 0 ", user_id);
    if (is_exists("STUDENTS", condition) != 1) {
        return not_found;
    }
    sprintf(sql, "update STUDENTS set activate = 1 where student_id = '%s';", user_id);
    rc = sqlite3_exec(db, sql, NULL, NULL, &errmsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: \n%s\n", errmsg);
        sqlite3_free(errmsg);
        return sql_err;
    }
    return success;
}

int change_student_pass(const char *user_id, const char *new_pass) {
    if (current_user.user_type != admin) {
        return permission_denied;
    }
    char condition[max_size];
    sprintf(condition, "where student_id = '%s' and activate = 1 ", user_id);
    if (is_exists("STUDENTS", condition) != 1) {
        return not_found;
    }
    char *errmsg = NULL;
    char sql[max_size];
    sprintf(sql, "update STUDENTS set password = '%s' where student_id = '%s';", new_pass, user_id);
    int rc = sqlite3_exec(db, sql, NULL, NULL, &errmsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: \n%s\n", errmsg);
        sqlite3_free(errmsg);
        return sql_err;
    }
    student_report(_change_pass_by_admin, user_id, 0);
    return success;
}

int remove_student(const char *user_id) {
    if (current_user.user_type != admin) {
        return permission_denied;
    }
    char condition[max_size];
    sprintf(condition, "where student_id = '%s' ", user_id);
    if (is_exists("STUDENTS", condition) != 1) {
        return not_found;
    }
    char *errmsg = NULL;
    char sql[max_size];
    sprintf(sql, "delete from STUDENTS where student_id = '%s';", user_id);
    int rc = sqlite3_exec(db, sql, NULL, NULL, &errmsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: \n%s\n", errmsg);
        sqlite3_free(errmsg);
        return sql_err;
    }
    return success;
}

int deactivate(const char *user_id) {
    if (current_user.user_type != admin) {
        return permission_denied;
    }
    char condition[max_size];
    sprintf(condition, "where student_id = '%s' ", user_id);
    if (is_exists("STUDENTS", condition) != 1) {
        return not_found;
    }
    char *errmsg = NULL;
    char sql[max_size];
    sprintf(sql, "update STUDENTS set activate = 0 where student_id = '%s';", user_id);
    int rc = sqlite3_exec(db, sql, NULL, NULL, &errmsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: \n%s\n", errmsg);
        sqlite3_free(errmsg);
        return sql_err;
    }
    return success;
}

int define_self(const char *self_id, const char *name, const char *location, const char *capacity, const char *type, const char *meal, const char *lunch_time_start, const char *lunch_time_end, const char *dinner_time_start, const char *dinner_time_end) {
    if (current_user.user_type != admin) {
        return permission_denied;
    }
    char *errmsg = NULL;
    char sql[max_size];
    sprintf(sql, "insert into SELF values(%s, '%s', '%s', %s, '%s', '%s', %s, %s, %s, %s);", self_id, name, location, capacity, type, meal, lunch_time_start, lunch_time_end, dinner_time_start, dinner_time_end);
    int rc = sqlite3_exec(db, sql, NULL, NULL, &errmsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: \n%s\n", errmsg);
        sqlite3_free(errmsg);
        return sql_err;
    }
    return success;
}

int define_food(const char *food_id, const char *name, const char *type, const char *price) {
    if (current_user.user_type != admin) {
        return permission_denied;
    }
    char *errmsg = NULL;
    char sql[max_size];
    sprintf(sql, "insert into FOOD values(%s, '%s', '%s', %s);", food_id, name, type, price);
    int rc = sqlite3_exec(db, sql, NULL, NULL, &errmsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: \n%s\n", errmsg);
        sqlite3_free(errmsg);
        return sql_err;
    }
    return success;
}

int define_meal_plan(const char *self_id, const char *date, const char *type, const char *food_id, const char *count) {
    if (current_user.user_type != admin) {
        return permission_denied;
    }
    char condition[max_size];
    sprintf(condition, "where self_id = %s and (meal = '%s' or meal = 'both') ", self_id, type);
    if (is_exists("SELF", condition) != 1) {
        return not_found;
    }
    sprintf(condition, "where food_id = %s ", food_id);
    if (is_exists("FOOD", condition) != 1) {
        return not_found;
    }
    char *errmsg = NULL;
    char sql[max_size];
    sprintf(sql, "insert into MEAL_PLAN values(%d, %s, %s, '%s', '%s', %s);", ID[meal_plan_id], self_id, food_id, date, type, count);
    int rc = sqlite3_exec(db, sql, NULL, NULL, &errmsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: \n%s\n", errmsg);
        sqlite3_free(errmsg);
        return sql_err;
    }
    ++ID[meal_plan_id];
    return success;
}

int charge_student_account(const char *user_id, const char *amount) {
    if (current_user.user_type != admin) {
        return permission_denied;
    }
    char condition[max_size];
    sprintf(condition, "where student_id = '%s' ", user_id);
    if (is_exists("STUDENTS", condition) != 1) {
        return not_found;
    }
    char *errmsg = NULL;
    char sql[max_size];
    sprintf(sql, "update STUDENTS set balance = balance + %s where student_id = '%s';", amount, user_id);
    int rc = sqlite3_exec(db, sql, NULL, NULL, &errmsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: \n%s\n", errmsg);
        sqlite3_free(errmsg);
        return sql_err;
    }
    student_report(_recieve_charge_by_admin, user_id, atof(amount));
    return success;
}

int add_news(const char *title, const char *content, const char *end_date) {
    if (current_user.user_type != admin) {
        return permission_denied;
    }
    char *errmsg = NULL;
    char sql[max_size];
    sprintf(sql, "insert into NEWS values(%d, '%s', '%s', '%s');", ID[news_id], title, content, end_date);
    int rc = sqlite3_exec(db, sql, NULL, NULL, &errmsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: \n%s\n", errmsg);
        sqlite3_free(errmsg);
        return sql_err;
    }
    ++ID[news_id];
    return success;
}

int add_poll(const char *question, const char *option1, const char *option2, const char *option3, const char *option4, const char *end_date) {
    if (current_user.user_type != admin) {
        return permission_denied;
    }
    char *errmsg = NULL;
    char sql[max_size];
    sprintf(sql, "insert into POLL values(%d, '%s', '%s', '%s', %d, '%s', %d, '%s', %d, '%s', %d);", ID[poll_id], end_date, question, option1, 0, option2, 0, option3, 0, option4, 0);
    int rc = sqlite3_exec(db, sql, NULL, NULL, &errmsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: \n%s\n", errmsg);
        sqlite3_free(errmsg);
        return sql_err;
    }
    ++ID[poll_id];
    return success;
}

int statistics(const char *date, const char *meal, int is_taken) {
    char str[max_size];
    sprintf(str, "RESERVED_MEAL where date = '%s' and type = '%s' and taken = %d group by self_id", date, meal, is_taken);
    int result;
    if ((result = get_data("self_id, count()", str)) != success) {
        return result;
    }
    printf("\nTotal ");
    sprintf(str, "RESERVED_MEAL where date = '%s' and type = '%s' and taken = %d", date, meal, is_taken);
    result = get_data("count()", str);
    return result;
}

int get_student_report(const char *start_date, const char *end_date, const char *student_id) {
    char str[max_size];
    sprintf(str, "STUDENT_REPORTS where student_id = '%s' and julianday(date) >= julianday('%s') and julianday(date) <= julianday('%s')", student_id, start_date, end_date);
    return get_data("*", str);
}

int get_system_report(const char *start_date, const char *end_date, int affair) {
    char str[max_size];
    if (affair == _sum_amount) {
        sprintf(str, "SYSTEM_REPORTS where julianday(date) >= julianday('%s') and julianday(date) <= julianday('%s')", start_date, end_date);
        return get_data("sum(amount)", str);
    }
    sprintf(str, "SYSTEM_REPORTS where affair = '%s' and julianday(date) >= julianday('%s') and julianday(date) <= julianday('%s')", system_affairs[affair], start_date, end_date);
    return get_data("count(), sum(amount)", str);
}







