//
//  Student.h
//  Student Affairs Management System
//
//  Created by Manely Hamedani on 1/19/23.
//

#ifndef Student_h
#define Student_h


int reserve(const char *self_id, const char *date, const char *meal, const char *food_id, int ratio, int is_daily);
int take_food(const char *self_id, const char *date, const char *meal);
int charge_account(const char *amount);
int send_charge(const char *user_id, const char *amount, const char *name);
int cancel_reserve(const char *date, const char *meal);
int daily_reserve(const char *self_id, const char *food_id);
int define_agent(const char *date, const char *meal, const char *user_id);
int change_self(const char *date, const char *meal, const char *new_self);
int check_news(void);
int vote(int poll_id, int option, int is_testcase);
#endif /* Student_h */
