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

#define max_size_filename   100

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
                        "LUNCH_TIME     TEXT, " \
                        "DINNER_TIME    TEXT ";
    char *food_tbl = "FOOD";
    char *food_def = "FOOD_ID           INT     PRIMARY KEY     NOT NULL, " \
                        "NAME           TEXT    NOT NULL, " \
                        "TYPE           TEXT    NOT NULL, " \
                        "PRICE          REAL    NOT NULL ";
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
                                "DAILY_RESERVED     TINYINT NOT NULL ";
    char *taken_meal_tbl = "TAKEN_MEAL";
    char *taken_meal_def = "TAKEN_MEAL_ID           INT     PRIMARY KEY     NOT NULL, " \
                            "STUDENT_ID             TEXT    REFERENCES STUDENTS     NOT NULL, " \
                            "SELF_ID                INT     REFERENCES SELF     NOT NULL, " \
                            "FOOD_ID                INT     REFERENCES FOOD     NOT NULL, " \
                            "DATE                   TEXT    NOT NULL ";
    char *student_reports_tbl = "STUDENT_REPORTS";
    char *student_reports_def = "AFFAIR_ID          INT     PRIMARY KEY     NOT NULL, " \
                                "STUDENT_ID         TEXT    REFERENCES STUDENTS     NOT NULL, " \
                                "DATE               TEXT    NOT NULL, " \
                                "BALANCE_CHANGE     REAL    NOT NULL ";
    char *system_reports_tbl = "SYSTEM_REPORTS";
    char *system_reports_def = "AFFAIR_ID           INT     PRIMARY KEY     NOT NULL, " \
                                "DATE               TEXT    NOT NULL, " \
                                "DATA               REAL    NOT NULL ";
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
                    "COUNT_2     INT    NOT NULL ";
    
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

int main(int argc, const char * argv[]) {
    char *db_name = "samsDB.db";
    if (create_db(db_name, &db) != 0) {
        puts("Cannot create database!");
        return -1;
    }
    if (create_my_tables() != 0) {
        puts("Cannot create tables!");
        return -1;
    }
    FILE *input = fopen("input.txt", "r");
    FILE *output = fopen("output.txt", "w");
    get_command(input, output);
//    char test_case_flag;
//    puts("Do you wanna open test-case file? (y or n)");
//    scanf("%c", &test_case_flag);
//    if (test_case_flag == 'y' || test_case_flag == 'Y') {
//        char input_name[max_size_filename];
//        char output_name[max_size_filename];
//        puts("Enter input file name:");
//        scanf("%s", input_name);
//        puts("Enter output file name:");
//        scanf("%s", output_name);
//        FILE *input = fopen(input_name, "r");
//        if (input == NULL) {
//            puts("Cannot open input file!");
//            return -1;
//        }
//        FILE *output = fopen(output_name, "w");
//        if (output == NULL) {
//            puts("Cannot open output file!");
//            return -1;
//        }
//        get_command(input, output);
//    }
//    else {
//
//    }
    return 0;
}
