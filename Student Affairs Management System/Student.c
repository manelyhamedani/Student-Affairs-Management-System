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
#include "Menu.h"
#include <stdlib.h>
#include <ctype.h>
#include <time.h>



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
    else if (isdigit(argv[0][0])) {
        sscanf(argv[0], "%d", (int *)data);
    }
    else {
        sscanf(argv[0], "%s", (char *)data);
    }
    return 0;
}

int check_gender(const char *self_id) {
    char *errmsg = NULL;
    char sql[max_size];
    int gender;
    int type;
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
    return success;
}

int check_stock(const char *food_id, int ratio) {
    char *errmsg = NULL;
    char sql[max_size];
    int is_enough;
    sprintf(sql, "select (%d * (select price from FOOD where food_id = %s)) <= (select balance from STUDENTS where student_id = '%s');", ratio, food_id, current_user.username);
    int rc = sqlite3_exec(db, sql, callback, &is_enough, &errmsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", errmsg);
        sqlite3_free(errmsg);
        return permission_denied;
    }
    if (is_enough == 0) {
        return insufficient_money;
    }
    return success;
}

int reserve(const char *self_id, const char *date, const char *meal, const char *food_id, int ratio, int is_daily) {
    if (current_user.user_type != student) {
        return permission_denied;
    }
    char condition[max_size];
    char *errmsg = NULL;
    char sql[max_size];
    int rc;
    if (is_daily == 0)  {
        int time_check;
        sprintf(condition, "where self_id = %s and food_id = %s and type = '%s' and date = '%s' and count > 0 ", self_id, food_id, meal, date);
        if (is_exists("MEAL_PLAN", condition) != 1) {
            return not_found;
        }
        set_date_time();
        sprintf(sql, "select julianday('%s') - julianday('%s') > 2 and julianday('%s') - julianday('%s') < 14;", date, current_date_time.date, date, current_date_time.date);
        rc = sqlite3_exec(db, sql, callback, &time_check, &errmsg);
        if (rc != SQLITE_OK) {
            fprintf(stderr, "SQL error: %s\n", errmsg);
            sqlite3_free(errmsg);
            return permission_denied;
        }
        if (time_check == 0) {
            return permission_denied;
        }
    }
    if (check_gender(self_id) == permission_denied) {
        return permission_denied;
    }
    if (check_stock(food_id, ratio) == insufficient_money) {
        return insufficient_money;
    }
    sprintf(sql, "insert into RESERVED_MEAL (reserved_meal_id, student_id, self_id, food_id, type, date, daily_reserved, taken) values(%d, '%s', %s, %s, '%s', '%s', %d, 0);", ID[reserved_meal_id], current_user.username, self_id, food_id, meal, date, is_daily);
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
    int price;
    sprintf(sql, "select price from FOOD where food_id = %s;", food_id);
    rc = sqlite3_exec(db, sql, callback, &price, &errmsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", errmsg);
        sqlite3_free(errmsg);
        return permission_denied;
    }
    char food_type[max_size];
    sprintf(sql, "select type from FOOD where food_id = %s;", food_id);
    rc = sqlite3_exec(db, sql, callback, food_type, &errmsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", errmsg);
        sqlite3_free(errmsg);
        return permission_denied;
    }
    if (is_daily) {
        system_report(_daily_reserved, ratio * price);
    }
    else if (strcmp(food_type, "food") == 0) {
        system_report(_food_reserved, ratio * price);
    }
    else {
        system_report(_dessert_reserved, ratio * price);
    }
    sprintf(sql, "update STUDENTS set balance = balance - (%d * %d) where student_id = '%s';", ratio, price, current_user.username);
    rc = sqlite3_exec(db, sql, NULL, NULL, &errmsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", errmsg);
        sqlite3_free(errmsg);
        return permission_denied;
    }
    ++ID[reserved_meal_id];
    if (is_daily) {
        student_report(_daily_reserve, current_user.username, -ratio * price);
    }
    else {
        student_report(_reserve, current_user.username, -ratio * price);
    }
    return success;
}

int take_food(const char *self_id, const char *date, const char *meal) {
    if (current_user.user_type != student) {
        return permission_denied;
    }
    set_date_time();
    if (strcmp(current_date_time.date, date)) {
        return permission_denied;
    }
    char *errmsg = NULL;
    char sql[max_size];
    if (strcmp(meal, "lunch") == 0) {
        sprintf(sql, "select %s >= (select lunch_time_start from SELF where self_id = %s) and %s <= (select lunch_time_end from SELF where self_id = %s);", current_date_time.time, self_id, current_date_time.time, self_id);
    }
    else {
        sprintf(sql, "select %s >= (select dinner_time_start from SELF where self_id = %s) and %s <= (select dinner_time_end from SELF where self_id = %s);", current_date_time.time, self_id, current_date_time.time, self_id);
    }
    int in_time;
    int rc = sqlite3_exec(db, sql, callback, &in_time, &errmsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", errmsg);
        sqlite3_free(errmsg);
        return permission_denied;
    }
    if (in_time == 0) {
        return permission_denied;
    }
    char condition[max_size];
    sprintf(condition, "where self_id = %s and date = '%s' and type = '%s' ", self_id, date, meal);
    if (is_exists("MEAL_PLAN", condition) != 1) {
        return not_found;
    }
    sprintf(condition, "where (student_id = '%s' or agent = '%s') and self_id = %s and date = '%s' and type = '%s' ", current_user.username, current_user.username, self_id, date, meal);
    if (is_exists("RESERVED_MEAL", condition) != 1) {
        return permission_denied;
    }
    int food_id;
    sprintf(sql, "select food_id from RESERVED_MEAL where (student_id = '%s' or agent = '%s') and self_id = %s and date = '%s' and type = '%s';", current_user.username, current_user.username, self_id, date, meal);
    rc = sqlite3_exec(db, sql, callback, &food_id, &errmsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", errmsg);
        sqlite3_free(errmsg);
        return permission_denied;
    }
    char food_name[max_size];
    sprintf(sql, "select name from FOOD where food_id = %d;", food_id);
    rc = sqlite3_exec(db, sql, callback, food_name, &errmsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", errmsg);
        sqlite3_free(errmsg);
        return permission_denied;
    }
    char self_name[max_size];
    sprintf(sql, "select name from SELF where self_id = %s;", self_id);
    rc = sqlite3_exec(db, sql, callback, self_name, &errmsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", errmsg);
        sqlite3_free(errmsg);
        return permission_denied;
    }
    char food_type[max_size];
    sprintf(sql, "select type from FOOD where food_id = %d;", food_id);
    rc = sqlite3_exec(db, sql, callback, food_type, &errmsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", errmsg);
        sqlite3_free(errmsg);
        return permission_denied;
    }
    if (strcmp(food_type, "food") == 0) {
        system_report(_food_taken, 0);
    }
    else {
        system_report(_dessert_taken, 0);
    }
    sprintf(sql, "insert into TAKEN_MEAL values(%d, '%s', '%s', '%s', '%s', %s);", ID[taken_meal_id], current_user.username, self_name, food_name, date, current_date_time.time);
    rc = sqlite3_exec(db, sql, NULL, NULL, &errmsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", errmsg);
        sqlite3_free(errmsg);
        return permission_denied;
    }
    ++ID[taken_meal_id];
    sprintf(sql, "update RESERVED_MEAL set taken = 1 where self_id = %s and date = '%s' and type = '%s' and student_id = '%s';", self_id, date, meal, current_user.username);
    rc = sqlite3_exec(db, sql, NULL, NULL, &errmsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", errmsg);
        sqlite3_free(errmsg);
        return permission_denied;
    }
    student_report(_take_food, current_user.username, 0);
    return success;
}

int charge_account(const char *amount) {
    if (current_user.user_type != student) {
        return permission_denied;
    }
    char *errmsg = NULL;
    char sql[max_size];
    sprintf(sql, "update STUDENTS set balance = balance + %s where student_id = '%s';", amount, current_user.username);
    int rc = sqlite3_exec(db, sql, NULL, NULL, &errmsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", errmsg);
        sqlite3_free(errmsg);
        return permission_denied;
    }
    student_report(_charge_account, current_user.username, atof(amount));
    return success;
}

int send_charge(const char *user_id, const char *amount, const char *name) {
    if (current_user.user_type != student) {
        return permission_denied;
    }
    char condition[max_size];
    sprintf(condition, "where name = '%s' collate nocase and student_id = '%s' ", name, user_id);
    if (is_exists("STUDENTS", condition) != 1) {
        return not_found;
    }
    char *errmsg = NULL;
    char sql[max_size];
    int is_enough;
    sprintf(sql, "select (select balance from STUDENTS where student_id = '%s') >= %s;", current_user.username, amount);
    int rc = sqlite3_exec(db, sql, callback, &is_enough, &errmsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", errmsg);
        sqlite3_free(errmsg);
        return permission_denied;
    }
    if (is_enough == 0) {
        return permission_denied;
    }
    sprintf(sql, "update STUDENTS set balance = balance - %s where student_id = '%s'; update STUDENTS set balance = balance + %s where student_id = '%s';", amount, current_user.username, amount, user_id);
    rc = sqlite3_exec(db, sql, callback, &is_enough, &errmsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", errmsg);
        sqlite3_free(errmsg);
        return permission_denied;
    }
    student_report(_send_charge, current_user.username, -atof(amount));
    student_report(_recieve_charge_by_student, user_id, atof(amount));
    return success;
}

int cancel_reserve(const char *date, const char *meal) {
    if (current_user.user_type != student) {
        return permission_denied;
    }
    char *errmsg = NULL;
    char sql[max_size];
    int self_id = -1;
    sprintf(sql, "select self_id from RESERVED_MEAL where date = '%s' and type = '%s' and student_id = '%s';", date, meal, current_user.username);
    int rc = sqlite3_exec(db, sql, callback, &self_id, &errmsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", errmsg);
        sqlite3_free(errmsg);
        return permission_denied;
    }
    if (self_id == -1) {
        return not_found;
    }
    int time_check;
    set_date_time();
    sprintf(sql, "select (select %s_time_start from SELF where self_id = %d) - %s >= 0100;", meal, self_id, current_date_time.time);
    rc = sqlite3_exec(db, sql, callback, &time_check, &errmsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", errmsg);
        sqlite3_free(errmsg);
        return permission_denied;
    }
    if (time_check == 0) {
        return permission_denied;
    }
    int food_id;
    sprintf(sql, "select food_id from RESERVED_MEAL where date = '%s' and type = '%s' and student_id = '%s';", date, meal, current_user.username);
    rc = sqlite3_exec(db, sql, callback, &food_id, &errmsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", errmsg);
        sqlite3_free(errmsg);
        return permission_denied;
    }
    sprintf(sql, "delete from RESERVED_MEAL where date = '%s' and type = '%s' and student_id = '%s';", date, meal, current_user.username);
    rc = sqlite3_exec(db, sql, NULL, NULL, &errmsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", errmsg);
        sqlite3_free(errmsg);
        return permission_denied;
    }
    int price;
    sprintf(sql, "select price from FOOD where food_id = %d;", food_id);
    rc = sqlite3_exec(db, sql, callback, &price, &errmsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", errmsg);
        sqlite3_free(errmsg);
        return permission_denied;
    }
    sprintf(sql, "update STUDENTS set balance = balance + (0.9 * %d) where student_id = '%s';", price, current_user.username);
    rc = sqlite3_exec(db, sql, NULL, NULL, &errmsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", errmsg);
        sqlite3_free(errmsg);
        return permission_denied;
    }
    sprintf(sql, "update MEAL_PLAN set count = count + 1 where self_id = %d and food_id = %d and type = '%s' and date = '%s';", self_id, food_id, meal, date);
    rc = sqlite3_exec(db, sql, NULL, NULL, &errmsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", errmsg);
        sqlite3_free(errmsg);
        return permission_denied;
    }
    student_report(_cancel_reserve, current_user.username, 0.9 * price);
    system_report(_cancel_reserved, -0.9 * price);
    return success;
}

int daily_reserve(const char *self_id, const char *food_id) {
    if (current_user.user_type != student) {
        return permission_denied;
    }
    char *errmsg = NULL;
    char sql[max_size];
    int lunch_time_start, lunch_time_end;
    int dinner_time_start, dinner_time_end;
    sprintf(sql, "select lunch_time_start from SELF where self_id = %s;", self_id);
    int rc = sqlite3_exec(db, sql, callback, &lunch_time_start, &errmsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", errmsg);
        sqlite3_free(errmsg);
        return permission_denied;
    }
    sprintf(sql, "select lunch_time_end from SELF where self_id = %s;", self_id);
    rc = sqlite3_exec(db, sql, callback, &lunch_time_end, &errmsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", errmsg);
        sqlite3_free(errmsg);
        return permission_denied;
    }
    sprintf(sql, "select dinner_time_start from SELF where self_id = %s;", self_id);
    rc = sqlite3_exec(db, sql, callback, &dinner_time_start, &errmsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", errmsg);
        sqlite3_free(errmsg);
        return permission_denied;
    }
    sprintf(sql, "select dinner_time_end from SELF where self_id = %s;", self_id);
    rc = sqlite3_exec(db, sql, callback, &dinner_time_end, &errmsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", errmsg);
        sqlite3_free(errmsg);
        return permission_denied;
    }
    char condition[max_size];
    set_date_time();
    sprintf(condition, "where self_id = %s and food_id = %s and date = '%s' and count > 0 and %s - %d >= 0 and %d - %s >= 30 ", self_id, food_id, current_date_time.date, current_date_time.time, lunch_time_start, lunch_time_end, current_date_time.time);
    if (is_exists("MEAL_PLAN", condition) == 1) {
        return reserve(self_id, current_date_time.date, "lunch", food_id, 2, 1);
    }
    sprintf(condition, "where self_id = %s and food_id = %s and date = '%s' and count > 0 and %s - %d >= 0 and %d - %s >= 30 ", self_id, food_id, current_date_time.date, current_date_time.time, dinner_time_start, dinner_time_end, current_date_time.time);
    if (is_exists("MEAL_PLAN", condition) == 1) {
        return reserve(self_id, current_date_time.date, "dinner", food_id, 2, 1);
    }
    return not_found;
}

int define_agent(const char *date, const char *meal, const char *user_id) {
    if (current_user.user_type != student) {
        return permission_denied;
    }
    char condition[max_size];
    sprintf(condition, "where student_id = '%s' ", user_id);
    if (is_exists("STUDENTS", condition) != 1) {
        return not_found;
    }
    sprintf(condition, "where student_id = '%s' and date = '%s' and type = '%s' and taken = 0 ", current_user.username, date, meal);
    if (is_exists("RESERVED_MEAL", condition) != 1) {
        return not_found;
    }
    char *errmsg = NULL;
    char sql[max_size];
    sprintf(sql, "update RESERVED_MEAL set agent = '%s' where student_id = '%s' and date = '%s' and type = '%s';", user_id, current_user.username, date, meal);
    int rc = sqlite3_exec(db, sql, NULL, NULL, &errmsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", errmsg);
        sqlite3_free(errmsg);
        return permission_denied;
    }
    student_report(_define_agent, current_user.username, 0);
    return success;
}

int change_self(const char *date, const char *meal, const char *new_self) {
    if (current_user.user_type != student) {
        return permission_denied;
    }
    char *errmsg = NULL;
    char sql[max_size];
    char condition[max_size];
    sprintf(condition, "where self_id = %s and date = '%s' and type = '%s' ", new_self, date, meal);
    if (is_exists("MEAL_PLAN", condition) != 1) {
        return not_found;
    }
    sprintf(condition, "where date = '%s' and type = '%s' and student_id = '%s' ", date, meal, current_user.username);
    if (is_exists("RESERVED_MEAL", condition) != 1) {
        return not_found;
    }
    int self_id;
    sprintf(sql, "select self_id from RESERVED_MEAL where date = '%s' and type = '%s' and student_id = '%s';", date, meal, current_user.username);
    int rc = sqlite3_exec(db, sql, callback, &self_id, &errmsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", errmsg);
        sqlite3_free(errmsg);
        return permission_denied;
    }
    set_date_time();
    int time_check;
    sprintf(sql, "select (select %s_time_start from SELF where self_id = %d) - %s >= 0300;", meal, self_id, current_date_time.time);
    rc = sqlite3_exec(db, sql, callback, &time_check, &errmsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", errmsg);
        sqlite3_free(errmsg);
        return permission_denied;
    }
    if (time_check == 0) {
        return permission_denied;
    }
    sprintf(sql, "update RESERVED_MEAL set self_id = %s where student_id = '%s' and date = '%s' and type = '%s';", new_self, current_user.username, date, meal);
    rc = sqlite3_exec(db, sql, NULL, NULL, &errmsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", errmsg);
        sqlite3_free(errmsg);
        return permission_denied;
    }
    student_report(_change_self, current_user.username, 0);
    return success;
}



int check_news(int is_testcase) {
    if (current_user.user_type != student) {
        return permission_denied;
    }
    char str[max_size];
    int result;
    set_date_time();
    if (is_testcase == 0) {
        sprintf(str, "NEWS where julianday(end_date) - julianday('%s') >= 0", current_date_time.date);
        result = get_data("title, content", str);
        if (result != success) {
            return result;
        }
        student_report(_check_news, current_user.username, 0);
        return success;
    }
    sprintf(str, "where julianday(end_date) - julianday('%s') >= 0 ", current_date_time.date);
    if (is_exists("NEWS", str) != 1) {
        return not_found;
    }
    student_report(_check_news, current_user.username, 0);
    return success;
}

int vote(int poll_id, int option, int is_testcase) {
    if (current_user.user_type != student) {
        return permission_denied;
    }
    char *errmsg = NULL;
    char sql[max_size];
    char condition[max_size];
    set_date_time();
    if (is_testcase) {
        sprintf(condition, "where julianday(end_date) - julianday('%s') >= 0 ", current_date_time.date);
        if (is_exists("POLL", condition) != 1) {
            return not_found;
        }
        return success;
    }
    sprintf(condition, "where julianday(end_date) - julianday('%s') >= 0 and poll_id = %d and option_%d != 'null' ", current_date_time.date, poll_id, option);
    if (is_exists("POLL", condition) != 1) {
        return not_found;
    }
    sprintf(sql, "update POLL set count_%d = count_%d + 1 where poll_id = %d;", option, option, poll_id);
    int rc = sqlite3_exec(db, sql, NULL, NULL, &errmsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", errmsg);
        sqlite3_free(errmsg);
        return permission_denied;
    }
    student_report(_vote, current_user.username, 0);
    return success;
}

int get_reserved_meal_plan(int week) {
    set_date_time();
    char str[max_size];
    int day_start = current_time->tm_wday + week * 7;
    int day_end = day_start + 7;
    sprintf(str, "RESERVED_MEAL where date >= date('%s', '%d day') and date <= date('%s', '%d day')", current_date_time.date, day_start, current_date_time.date, day_end);
    return get_data("*", str);
}

int get_financial_report(void) {
    char *str = "STUDENT_REPORTS where balance_change != 0";
    return get_data("*", str);
}

int get_taken_meal_report(void) {
    char str[max_size];
    sprintf(str, "TAKEN_MEAL where student_id = '%s'", current_user.username);
    return get_data("*", str);
}




