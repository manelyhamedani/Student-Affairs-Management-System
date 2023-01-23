//
//  Admin.h
//  Student Affairs Management System
//
//  Created by Manely Hamedani on 1/19/23.
//

#ifndef Admin_h
#define Admin_h
#include <sqlite3.h>

int approve(const char *user_id);
int change_student_pass(const char *user_id, const char *new_pass);
int remove_student(const char *user_id);
int deactivate(const char *user_id);
int define_self(const char *self_id, const char *name, const char *location, const char *capacity, const char *type, const char *meal, const char *lunch_time, const char *dinner_time);
int define_food(const char *food_id, const char *name, const char *type, const char *price);
int define_meal_plan(const char *self_id, const char *date, const char *type, const char *food_id, const char *count);
int charge_student_account(const char *user_id, const char *amount);
int add_news(const char *title, const char *content, const char *end_date);
int add_poll(const char *question, const char *option1, const char *option2, const char *option3, const char *option4, const char *end_date);
#endif /* Admin_h */
