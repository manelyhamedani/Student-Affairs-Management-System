//
//  Utility.h
//  Student Affairs Management System
//
//  Created by Manely Hamedani on 1/19/23.
//

#ifndef Utility_h
#define Utility_h
#include <sqlite3.h>

#define max_size    500

enum type {
    none, student, admin
};

enum result {
  success, not_found, permission_denied, invalid, end_of_line, end_of_file, insufficient_money
};

extern sqlite3 *db;
extern struct tm *current_time;
extern struct tm *last_time;

extern struct _date_time {
    char date[11];
    char time[5];
} current_date_time;

extern struct _user {
    char username[30];
    char password[20];
    enum type user_type;
} current_user;


extern int ID[5];

enum ID {
    meal_plan_id, news_id, poll_id, reserved_meal_id, taken_meal_id
};

typedef struct {
    char username[max_size];
    char password[max_size];
} login_parameter;

typedef struct {
    char username[max_size];
} logout_parameter;

typedef struct {
    char name[max_size];
    char family[max_size];
    char user_id[max_size];
    char password[max_size];
    char national_id[max_size];
    char birthdate[max_size];
    char gender[max_size];
    char type[max_size];
} register_parameter;

typedef struct {
    char user_id[max_size];
} approve_parameter;

typedef struct {
    char user_id[max_size];
    char old_pass[max_size];
    char new_pass[max_size];
} change_pass_parameter;

typedef struct {
    char user_id[max_size];
    char new_pass[max_size];
} change_student_pass_parameter;

typedef struct {
    char user_id[max_size];
} remove_student_parameter;

typedef struct {
    char user_id[max_size];
} deactivate_parameter;

typedef struct {
    char self_id[max_size];
    char name[max_size];
    char location[max_size];
    char capacity[max_size];
    char type[max_size];
    char meal[max_size];
    char lunch_time_start[max_size];
    char lunch_time_end[max_size];
    char dinner_time_start[max_size];
    char dinner_time_end[max_size];
} self_parameter;

typedef struct {
    char food_id[max_size];
    char name[max_size];
    char type[max_size];
    char price[max_size];
} food_parameter;

typedef struct {
    char self_id[max_size];
    char date[max_size];
    char type[max_size];
    char food_id[max_size];
    char count[max_size];
} meal_plan_parameter;

typedef struct {
    char user_id[max_size];
    char amount[max_size];
} charge_student_account_parameter;

typedef struct {
    char title[max_size];
    char content[max_size];
    char end_date[max_size];
} news_parameter;

typedef struct {
    char question[max_size];
    char option1[max_size];
    char option2[max_size];
    char option3[max_size];
    char option4[max_size];
    char end_date[max_size];
} poll_parameter;

typedef struct {
    char self_id[max_size];
    char date[max_size];
    char meal[max_size];
    char food_id[max_size];
} reserve_parameter;

typedef struct {
    char self_id[max_size];
    char date[max_size];
    char meal[max_size];
} take_food_parameter;

typedef struct {
    char amount[max_size];
    char card_number[max_size];
    char pass_code[max_size];
} charge_account_parameter;

typedef struct {
    char user_id[max_size];
    char amount[max_size];
    char name[max_size];
} send_charge_parameter;

typedef struct {
    char date[max_size];
    char meal[max_size];
} cancel_reserve_parameter;

typedef struct {
    char self_id[max_size];
    char food_id[max_size];
} daily_reserve_parameter;

typedef struct {
    char date[max_size];
    char meal[max_size];
    char user_id[max_size];
} agent_parameter;

typedef struct {
    char date[max_size];
    char meal[max_size];
    char new_self[max_size];
} change_self_parameter;

typedef struct {
    char date[max_size];
    char time[max_size];
} datetime_parameter;

int user_login(const char *username, const char *password);
int user_logout(const char *username);
int user_register(const char *name, const char *family, const char *user_id, const char *password, const char *national_id, const char *birthdate, const char *gender, const char *type);
int change_pass(const char *username, const char *old_pass, const char *new_pass);
int is_exists(const char *tbl_name, const char *condition);
int create_table(sqlite3 *db, const char *tbl_name, const char *definition);
int create_db(const char *db_name, sqlite3 **ppDB);
void change_datetime(const char *date, const char *time);
void get_date_time(void);
void set_date_time(void);
#endif /* Utility_h */
