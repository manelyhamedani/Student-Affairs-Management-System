//
//  main.c
//  Student Affairs Management System
//
//  Created by Manely Hamedani on 1/19/23.
//

#include <stdio.h>
#include "Utility.h"
#include "Admin.h"
#include "Test Case.h"
#include <string.h>
#include <stdlib.h>
#include "Menu.h"
#include <time.h>

#define max_size_filename   100

typedef struct _date_time date_time;

sqlite3 *db;

int create_my_tables(void) {
    char *admin_tbl = "ADMINS";
    char *admin_def = "ADMIN_ID          TEXT    PRIMARY KEY     NOT NULL, " \
                        "NAME           TEXT, " \
                        "FAMILY         TEXT, " \
                        "PASSWORD       TEXT    UNIQUE          NOT NULL, " \
                        "NATIONAL_ID    TEXT    UNIQUE, " \
                        "BIRTHDATE      TEXT, " \
                        "GENDER         TEXT ";
    char *student_tbl = "STUDENTS";
    char *student_def = "STUDENT_ID        TEXT    PRIMARY KEY     NOT NULL, " \
                        "NAME           TEXT    NOT NULL, " \
                        "FAMILY         TEXT    NOT NULL, " \
                        "PASSWORD       TEXT    UNIQUE          NOT NULL, " \
                        "NATIONAL_ID    TEXT     UNIQUE          NOT NULL, " \
                        "BIRTHDATE      TEXT    NOT NULL, " \
                        "GENDER         TEXT    NOT NULL, " \
                        "BALANCE        REAL, " \
                        "ACTIVATE       TINYINT NOT NULL ";
    char *pending_tbl = "PENDING";
    char *pending_def = "USER_ID        TEXT    PRIMARY KEY     NOT NULL, " \
                        "NAME           TEXT    NOT NULL, " \
                        "FAMILY         TEXT    NOT NULL, " \
                        "PASSWORD       TEXT    UNIQUE          NOT NULL, " \
                        "NATIONAL_ID    TEXT     UNIQUE          NOT NULL, " \
                        "BIRTHDATE      TEXT    NOT NULL, " \
                        "GENDER         TEXT    NOT NULL, " \
                        "TYPE           TEXT    NOT NULL ";
    char *self_tbl = "SElF";
    char *self_def = "SELF_ID           INT     PRIMARY KEY     NOT NULL, " \
                        "NAME           TEXT    NOT NULL, " \
                        "LOCATION       TEXT    NOT NULL, " \
                        "CAPACITY       INT     NOT NULL, " \
                        "TYPE           TEXT    NOT NULL, " \
                        "MEAL           TEXT    NOT NULL, " \
                        "LUNCH_TIME_START     INT   NOT NULL, " \
                        "LUNCH_TIME_END       INT   NOT NULL, " \
                        "DINNER_TIME_START    INT   NOT NULL, " \
                        "DINNER_TIME_END      INT   NOT NULL ";
    char *food_tbl = "FOOD";
    char *food_def = "FOOD_ID           INT     PRIMARY KEY     NOT NULL, " \
                        "NAME           TEXT    NOT NULL, " \
                        "TYPE           TEXT    NOT NULL, " \
                        "PRICE          INT    NOT NULL ";
    char *meal_plan_tbl = "MEAL_PLAN";
    char *meal_plan_def = "MEAL_PLAN_ID INT     PRIMARY KEY     NOT NULL, " \
                          "SELF_ID      INT     REFERENCES SELF     NOT NULL, " \
                          "FOOD_ID      INT     REFERENCES FOOD     NOT NULL, " \
                          "DATE         TEXT    NOT NULL, " \
                          "TYPE         TEXT    NOT NULL, " \
                          "COUNT        INT     NOT NULL ";
    char *resereved_meal_tbl = "RESERVED_MEAL";
    char *resereved_meal_def = "RESERVED_MEAL_ID    INT     PRIMARY KEY     NOT NULL, " \
                                "STUDENT_ID         TEXT    REFERENCES STUDENTS     NOT NULL, " \
                                "SELF_ID            INT     REFERENCES SELF     NOT NULL, " \
                                "FOOD_ID            INT     REFERENCES FOOD     NOT NULL, " \
                                "TYPE               TEXT    NOT NULL, " \
                                "DATE               TEXT    NOT NULL, " \
                                "AGENT              TEXT, " \
                                "DAILY_RESERVED     TINYINT NOT NULL, " \
                                "TAKEN              TINYINT NOT NULL";
    char *taken_meal_tbl = "TAKEN_MEAL";
    char *taken_meal_def = "TAKEN_MEAL_ID           INT     PRIMARY KEY     NOT NULL, " \
                            "STUDENT_ID             TEXT    REFERENCES STUDENTS     NOT NULL, " \
                            "SELF_NAME              TEXT    REFERENCES SELF     NOT NULL, " \
                            "FOOD_NAME              TEXT    REFERENCES FOOD     NOT NULL, " \
                            "DATE                   TEXT    NOT NULL, " \
                            "TIME                   INT     NOT NULL";
    char *student_reports_tbl = "STUDENT_REPORTS";
    char *student_reports_def = "REPORT_ID          INT     PRIMARY KEY     NOT NULL, " \
                                "AFFAIR             TEXT     NOT NULL, " \
                                "STUDENT_ID         TEXT    REFERENCES STUDENTS     NOT NULL, " \
                                "DATE               TEXT    NOT NULL, " \
                                "BALANCE_CHANGE     REAL    NOT NULL ";
    char *system_reports_tbl = "SYSTEM_REPORTS";
    char *system_reports_def =  "REPORT_ID          INT     PRIMARY KEY     NOT NULL, " \
                                "AFFAIR             TEXT    NOT NULL, " \
                                "DATE               TEXT    NOT NULL, " \
                                "AMOUNT             REAL    NOT NULL ";
    char *news_tbl = "NEWS";
    char *news_def = "NEWS_ID    INT    PRIMARY KEY     NOT NULL, " \
                    "TITLE       TEXT   NOT NULL, " \
                    "CONTENT     TEXT   NOT NULL, " \
                    "END_DATE    TEXT   NOT NULL ";
    char *poll_tbl = "POLL";
    char *poll_def = "POLL_ID    INT    PRIMARY KEY     NOT NULL, " \
                    "END_DATE    TEXT   NOT NULL, " \
                    "QUESTION    TEXT   NOT NULL, " \
                    "OPTION_1    TEXT   NOT NULL, " \
                    "COUNT_1     INT    NOT NULL, " \
                    "OPTION_2    TEXT   NOT NULL, " \
                    "COUNT_2     INT    NOT NULL, " \
                    "OPTION_3    TEXT, " \
                    "COUNT_3     INT, " \
                    "OPTION_4    TEXT, " \
                    "COUNT_4     INT ";
    
    if (create_table(db, admin_tbl, admin_def) != 0) {
        return -1;
    }
    if (create_table(db, student_tbl, student_def) != 0) {
        return -1;
    }
    if (create_table(db, pending_tbl, pending_def) != 0) {
        return -1;
    }
    if (create_table(db, self_tbl, self_def) != 0) {
        return -1;
    }
    if (create_table(db, food_tbl, food_def) != 0) {
        return -1;
    }
    if (create_table(db, meal_plan_tbl, meal_plan_def) != 0) {
        return -1;
    }
    if (create_table(db, resereved_meal_tbl, resereved_meal_def) != 0) {
        return -1;
    }
    if (create_table(db, taken_meal_tbl, taken_meal_def) != 0) {
        return -1;
    }
    if (create_table(db, student_reports_tbl, student_reports_def) != 0) {
        return -1;
    }
    if (create_table(db, system_reports_tbl, system_reports_def) != 0) {
        return -1;
    }
    if (create_table(db, news_tbl, news_def) != 0) {
        return -1;
    }
    if (create_table(db, poll_tbl, poll_def) != 0) {
        return -1;
    }
    return 0;
}




static int set_id_callback(void *data, int argc, char **argv, char **col_name) {
    static int counter = 0;
    if (argv[0] == NULL) {
        return 0;
    }
    sscanf(argv[0], "%d", &(((int *)data)[counter]));
    ++(((int *)data)[counter]);
    ++counter;
    return 0;
}

void set_id(void) {
    char *errmsg = NULL;
    char *sql = "select max(meal_plan_id) from MEAL_PLAN;" \
                "select max(news_id) from NEWS;" \
                "select max(poll_id) from POLL;" \
                "select max(reserved_meal_id) from RESERVED_MEAL;" \
                "select max(taken_meal_id) from TAKEN_MEAL;" \
                "select max(report_id) from STUDENT_REPORTS;" \
                "select max(report_id) from SYSTEM_REPORTS;";  
    int rc = sqlite3_exec(db, sql, set_id_callback, ID, &errmsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", errmsg);
        sqlite3_free(errmsg);
    }
}

void set_system_datetime(void) {
    get_date_time();
    sprintf(current_date_time.date, "%04d-%02d-%02d", current_time->tm_year + 1900, current_time->tm_mon + 1, current_time->tm_mday);
    sprintf(current_date_time.time, "%02d%02d", current_time->tm_hour, current_time->tm_min);
    last_time = current_time;
}

int main(int argc, const char * argv[]) {
    char *db_name = "samsDB.db";
    char *errmsg = NULL;
    int rc = sqlite3_open(db_name, &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", errmsg);
        sqlite3_free(errmsg);
        return -1;
    }
//    if (create_db(db_name, &db) != 0) {
//        puts("Cannot create database!");
//        return -1;
//    }
//    if (create_my_tables() != 0) {
//        puts("Cannot create tables!");
//        return -1;
//    }
    current_user.user_type = none;
    set_system_datetime();
    set_id();
    
//    get_command(input, output);
    
    char test_case_flag;
    puts("Do you wanna open test-case file? (y or n)");
    scanf("%c", &test_case_flag);
    if (test_case_flag == 'y' || test_case_flag == 'Y') {
        FILE *input = fopen("input.txt", "r");
        FILE *output = fopen("output.txt", "w");
        if (input == NULL) {
            puts("Cannot open input file!");
            return -1;
        }
        if (output == NULL) {
            puts("Cannot open output file!");
            return -1;
        }
        get_command(input, output);
        fclose(input);
        fclose(output);
    }
    else {
        main_menu();
    }
    return 0;
}
