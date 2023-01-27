//
//  Menu.c
//  Student Affairs Management System
//
//  Created by Manely Hamedani on 1/25/23.
//

#include "Menu.h"
#include <stdio.h>
#include "Utility.h"
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "Admin.h"
#include "Student.h"

int waiting_time = 0;
int choice;
static login_parameter login_input;
static register_parameter register_input;
static approve_parameter approve_input;
static self_parameter self_input;
static food_parameter food_input;
static meal_plan_parameter meal_plan_input;
static news_parameter news_input;
static poll_parameter poll_input;
static deactivate_parameter deactivate_input;
static remove_student_parameter remove_input;
static change_student_pass_parameter change_student_pass_input;
static charge_student_account_parameter charge_student_account_input;
static change_pass_parameter change_pass_input;
static datetime_parameter datetime_input;
static statistics_parameter statistics_input;
static student_report_parameter student_report_input;
static system_report_parameter system_report_input;
static reserve_parameter reserve_input;
static take_food_parameter take_food_input;
static charge_account_parameter charge_account_input;
static send_charge_parameter send_charge_input;
static cancel_reserve_parameter cancel_reserve_input;
static daily_reserve_parameter daily_reserve_input;
static agent_parameter agent_input;
static change_self_parameter change_self_input;
static vote_parameter vote_input;
static int week;

static int callback(void *data, int argc, char **argv, char **col_name) {
    if (strcmp(argv[0], "male") == 0) {
        strcpy((char *)data, "boyish");
    }
    else {
        strcpy((char *)data, "girlish");
    }
    return 0;
}

void find_gender(char *type) {
    char *errmsg = NULL;
    char sql[max_size];
    sprintf(sql, "select gender from STUDENTS where student_id = '%s';", current_user.username);
    int rc = sqlite3_exec(db, sql, callback, type, &errmsg);
    if (rc != SQLITE_OK) {
        printf("SQL error:\n%s", errmsg);
        sqlite3_free(&errmsg);
    }
}

void get_week(void) {
    printf("1) Previous week\n"
           "2) Current week\n"
           "3) Next week\n");
    int choice;
    scanf("%d", &choice);
    while (choice != 1 && choice != 2 && choice != 3) {
        puts("Wrong input!");
        scanf("%d", &choice);
    }
    week += choice - 2;
}

int get_vote_input(vote_parameter *input) {
    system("clear");
    printf("Poll list:\n");
    char str[max_size];
    sprintf(str, "POLL where julianday(end_date) - julianday('%s') >= 0", current_date_time.date);
    if (get_data("*", str) == not_found) {
        system("clear");
        puts("Poll list is empty!");
        sleep(waiting_time);
        return not_found;
    }
    puts("");
    printf("Poll_id: ");
    scanf("%d", &(input->poll_id));
    printf("Option: ");
    scanf("%d", &(input->option));
    printf("Enter c to cancel or s to submit.\n");
    char choice;
    scanf(" %c", &choice);
    if (choice == 'c' || choice == 'C') {
        system("clear");
        puts("The process has canceled.");
        sleep(waiting_time);
        return cancel;
    }
    return success;
}

int get_change_self_input(change_self_parameter *input) {
    system("clear");
    printf("Reserved list:\n");
    char str[max_size];
    sprintf(str, "RESERVED_MEAL where student_id = '%s' and taken = 0", current_user.username);
    if (get_data("date, meal", str) == not_found) {
        system("clear");
        puts("Reserved list is empty!");
        sleep(waiting_time);
        return not_found;
    }
    puts("");
    printf("Date(YYYY-MM-DD): ");
    scanf(" %[^'\n']", input->date);
    printf("Meal: ");
    scanf(" %[^'\n']", input->meal);
    system("clear");
    char type[max_size];
    find_gender(type);
    printf("Self list:\n");
    sprintf(str, "SELF inner join MEAL_PLAN on SELF.self_id = MEAL_PLAN.self_id where date = '%s' and MEAL_PLAN.type = '%s' and count > 0 and SELF.type = '%s'", input->date, input->meal, type);
    if (get_data("SELF.self_id, SELF.name", str) == not_found) {
        system("clear");
        puts("Self list is empty!");
        sleep(waiting_time);
        return not_found;
    }
    puts("");
    printf("New_self_id: ");
    scanf(" %[^'\n']", input->new_self);
    printf("Enter c to cancel or s to submit.\n");
    char choice;
    scanf(" %c", &choice);
    if (choice == 'c' || choice == 'C') {
        system("clear");
        puts("The process has canceled.");
        sleep(waiting_time);
        return cancel;
    }
    return success;
}

int get_agent_input(agent_parameter *input) {
    system("clear");
    printf("Reserved list:\n");
    char str[max_size];
    sprintf(str, "RESERVED_MEAL where student_id = '%s' and taken = 0", current_user.username);
    if (get_data("date, meal", str) == not_found) {
        system("clear");
        puts("Reserved list is empty!");
        sleep(waiting_time);
        return not_found;
    }
    puts("");
    printf("Date(YYYY-MM-DD): ");
    scanf(" %[^'\n']", input->date);
    printf("Meal: ");
    scanf(" %[^'\n']", input->meal);
    system("clear");
    printf("Student list:\n");
    sprintf(str, "STUDENTS where student_id != '%s' and activate = 1", current_user.username);
    if (get_data("student_id, name, family", str) == not_found) {
        system("clear");
        puts("Student list is empty!");
        sleep(waiting_time);
        return not_found;
    }
    puts("");
    printf("Student_id: ");
    scanf(" %[^'\n']", input->user_id);
    printf("Enter c to cancel or s to submit.\n");
    char choice;
    scanf(" %c", &choice);
    if (choice == 'c' || choice == 'C') {
        system("clear");
        puts("The process has canceled.");
        sleep(waiting_time);
        return cancel;
    }
    return success;
}

int get_daily_reserve_input(daily_reserve_parameter *input) {
    system("clear");
    printf("Meal: ");
    char meal[max_size];
    scanf(" %[^'\n']", meal);
    char str[max_size];
    char type[max_size];
    find_gender(type);
    sprintf(str, "SELF inner join MEAL_PLAN on SELF.self_id = MEAL_PLAN.self_id inner join food on FOOD.food_id = MEAL_PLAN.food_id where MEAL_PLAN.type = '%s' and SELF.type = '%s'", meal, type);
    system("clear");
    printf("Daily meal plan list:\n");
    if (get_data("SELF.self_id, SELF.name, FOOD.food_id, FOOD.name", str) == not_found) {
        system("clear");
        puts("Daily meal plan list is empty!");
        sleep(waiting_time);
        return not_found;
    }
    puts("");
    printf("Self_id: ");
    scanf(" %[^'\n']", input->self_id);
    printf("Food_id: ");
    scanf(" %[^'\n']", input->food_id);
    printf("Enter c to cancel or s to submit.\n");
    char choice;
    scanf(" %c", &choice);
    if (choice == 'c' || choice == 'C') {
        system("clear");
        puts("The process has canceled.");
        sleep(waiting_time);
        return cancel;
    }
    return success;
}

int get_cancel_reserve_input(cancel_reserve_parameter *input) {
    system("clear");
    printf("Reserved list:\n");
    char str[max_size];
    sprintf(str, "STUDENTS where student_id = '%s' and taken = 0", current_user.username);
    if (get_data("date, type", str) == not_found) {
        system("clear");
        puts("Reserved list is empty!");
        sleep(waiting_time);
        return not_found;
    }
    puts("");
    printf("Date(YYYY-MM-DD): ");
    scanf(" %[^'\n']", input->date);
    printf("Meal: ");
    scanf(" %[^'\n']", input->meal);
    printf("Enter c to cancel or s to submit.\n");
    char choice;
    scanf(" %c", &choice);
    if (choice == 'c' || choice == 'C') {
        system("clear");
        puts("The process has canceled.");
        sleep(waiting_time);
        return cancel;
    }
    return success;
}

int get_send_charge_input(send_charge_parameter *input) {
    system("clear");
    printf("Student list:\n");
    char str[max_size];
    sprintf(str, "STUDENTS where student_id != '%s'", current_user.username);
    if (get_data("student_id, name, family", str) == not_found) {
        system("clear");
        puts("Student list is empty!");
        sleep(waiting_time);
        return not_found;
    }
    puts("");
    printf("Student_id: ");
    scanf(" %[^'\n']", input->user_id);
    printf("Student_name: ");
    scanf(" %[^'\n']", input->name);
    printf("Amount: ");
    scanf(" %[^'\n']", input->amount);
    sprintf(str, "STUDENTS where student_id = '%s'", input->user_id);
    if (get_data("student_id, name, family", str) == not_found) {
        system("clear");
        printf("Student %s not found!", input->user_id);
        sleep(waiting_time);
        return not_found;
    }
    printf("Enter c to cancel or s to submit.\n");
    char choice;
    scanf(" %c", &choice);
    if (choice == 'c' || choice == 'C') {
        system("clear");
        puts("The process has canceled.");
        sleep(waiting_time);
        return cancel;
    }
    return success;
}

int get_charge_account_input(charge_account_parameter *input) {
    system("clear");
    printf("Amount: ");
    scanf(" %[^'\n']", input->amount);
    printf("Card-number: ");
    scanf(" %[^'\n']", input->card_number);
    printf("Pass-code: ");
    scanf(" %[^'\n']", input->pass_code);
    printf("Enter c to cancel or s to submit.\n");
    char choice;
    scanf(" %c", &choice);
    if (choice == 'c' || choice == 'C') {
        system("clear");
        puts("The process has canceled.");
        sleep(waiting_time);
        return cancel;
    }
    return success;
}

int get_take_food_input(take_food_parameter *input) {
    system("clear");
    strcpy(input->date, current_date_time.date);
    printf("Meal: ");
    scanf(" %[^'\n']", input->meal);
    printf("Self list:\n");
    char str[max_size];
    sprintf(str, "SELF inner join SELF on SELF.self_id = RESERVED_MEAL.self_id where date = '%s' and type = '%s' and (student_id = '%s' or agent = '%s' and taken = 0)", input->date, input->meal, current_user.username, current_user.username);
    if (get_data("SELF.self_id, name", str) == not_found) {
        system("clear");
        puts("Self list is empty! You didn't reserve any meal for today.");
        sleep(waiting_time);
        return not_found;
    }
    puts("");
    printf("Self_id: ");
    scanf(" %[^'\n']", input->self_id);
    printf("Enter c to cancel or s to submit.\n");
    char choice;
    scanf(" %c", &choice);
    if (choice == 'c' || choice == 'C') {
        system("clear");
        puts("The process has canceled.");
        sleep(waiting_time);
        return cancel;
    }
    return success;
}

int get_reserve_input(reserve_parameter *input) {
    system("clear");
    char type[max_size];
    find_gender(type);
    char str[max_size];
    sprintf(str, "SELF where type = '%s'", type);
    printf("Self list:\n");
    if (get_data("*", str) == not_found) {
        puts("Self list is empty!");
        sleep(waiting_time);
        return not_found;
    }
    puts("");
    printf("Self_id: ");
    scanf(" %[^'\n']", input->self_id);
    printf("Date(YYYY-MM-DD): ");
    scanf(" %[^'\n']", input->date);
    printf("Meal: ");
    scanf(" %[^'\n']", input->meal);
    system("clear");
    printf("Food list: ");
    sprintf(str, "FOOD inner join MEAL_PLAN on FOOD.food_id = MEAL_PLAN.food_id where date = '%s' and MEAL_PLAN.type = '%s' and self_id = %s", input->date, input->meal, input->self_id);
    if (get_data("FOOD.food_id, name", str) == not_found) {
        puts("Food list is empty!");
        sleep(waiting_time);
        return not_found;
    }
    puts("");
    printf("Food_id: ");
    scanf(" %[^'\n']", input->food_id);
    printf("Enter c to cancel or s to submit.\n");
    char choice;
    scanf(" %c", &choice);
    if (choice == 'c' || choice == 'C') {
        system("clear");
        puts("The process has canceled.");
        sleep(waiting_time);
        return cancel;
    }
    return success;
}

int get_system_report_input(system_report_parameter *input) {
    system("clear");
    printf("Report list:\n"
           "1) Reserved food\n"
           "2) Taken food\n"
           "3) Reserved dessert\n"
           "4) Taken dessert\n"
           "5) Daily reserved\n"
           "6) Canceled reserve\n"
           "7) Total amount\n"
           "Enter 0 to back to menu.\n");
    scanf("%d", &input->affair);
    if (input->affair == 0) {
        system("clear");
        puts("The proccess has canceled.");
        sleep(waiting_time);
        return cancel;
    }
    --(input->affair);
    printf("Start_date(YYYY-MM-DD): ");
    scanf(" %[^'\n']", input->start_date);
    printf("End_date(YYYY-MM-DD): ");
    scanf(" %[^'\n']", input->end_date);
    return success;
}

int get_student_report_input(student_report_parameter *input) {
    system("clear");
    puts("Student list:\n");
    if (get_data("student_id", "STUDENTS") == not_found) {
        puts("Student list is empty!");
        sleep(waiting_time);
        return not_found;
    }
    puts("");
    printf("Student_id: ");
    scanf(" %[^'\n']", input->student_id);
    printf("Start_date(YYYY-MM-DD): ");
    scanf(" %[^'\n']", input->start_date);
    printf("End_date(YYYY-MM-DD): ");
    scanf(" %[^'\n']", input->end_date);
    printf("Enter c to cancel or s to submit.\n");
    char choice;
    scanf(" %c", &choice);
    if (choice == 'c' || choice == 'C') {
        system("clear");
        puts("The process has canceled.");
        sleep(waiting_time);
        return cancel;
    }
    return success;
}

int get_statistics_input(statistics_parameter *input) {
    system("clear");
    printf("Date: ");
    scanf(" %[^'\n']", input->date);
    printf("Meal: ");
    scanf(" %[^'\n']", input->meal);
    printf("Enter 1 for taken meals and 0 for reserved meals: ");
    scanf("%d", &(input->is_taken));
    printf("Enter c to cancel or s to submit.\n");
    char choice;
    scanf(" %c", &choice);
    if (choice == 'c' || choice == 'C') {
        system("clear");
        puts("The process has canceled.");
        sleep(waiting_time);
        return cancel;
    }
    return success;
}

int get_datetime_input(datetime_parameter *input) {
    system("clear");
    printf("Date(YYYY-MM-DD): ");
    scanf(" %[^'\n']", input->date);
    printf("Time(HHMM): ");
    scanf(" %[^'\n']", input->time);
    printf("Enter c to cancel or s to submit.\n");
    char choice;
    scanf(" %c", &choice);
    if (choice == 'c' || choice == 'C') {
        system("clear");
        puts("The process has canceled.");
        sleep(waiting_time);
        return cancel;
    }
    return success;
}

int get_change_pass_input(change_pass_parameter *input) {
    system("clear");
    printf("User_id: ");
    scanf(" %[^'\n']", input->user_id);
    char *temp_pass;
    temp_pass = getpass("Old_pass: ");
    strcpy(input->old_pass, temp_pass);
    temp_pass = getpass("New_pass: ");
    strcpy(input->new_pass, temp_pass);
    printf("Enter c to cancel or s to submit.\n");
    char choice;
    scanf(" %c", &choice);
    if (choice == 'c' || choice == 'C') {
        system("clear");
        puts("The process has canceled.");
        sleep(waiting_time);
        return cancel;
    }
    return success;
}

int get_charge_student_account_input(charge_student_account_parameter *input) {
    system("clear");
    puts("Student list:\n");
    if (get_data("student_id", "STUDENTS") == not_found) {
        puts("Student list is empty!");
        sleep(waiting_time);
        return not_found;
    }
    puts("");
    printf("Student_id: ");
    scanf(" %[^'\n']", input->user_id);
    printf("Amount: ");
    scanf(" %[^'\n']", input->amount);
    printf("Enter c to cancel or s to submit.\n");
    char choice;
    scanf(" %c", &choice);
    if (choice == 'c' || choice == 'C') {
        system("clear");
        puts("The process has canceled.");
        sleep(waiting_time);
        return cancel;
    }
    return success;
}

int get_change_student_pass_input(change_student_pass_parameter *input) {
    system("clear");
    puts("Student list:\n");
    if (get_data("student_id", "STUDENTS") == not_found) {
        puts("Student list is empty!");
        sleep(waiting_time);
        return not_found;
    }
    puts("");
    printf("Student_id: ");
    scanf(" %[^'\n']", input->user_id);
    char *temp_pass;
    temp_pass = getpass("New_pass: ");
    strcpy(input->new_pass, temp_pass);
    printf("Enter c to cancel or s to submit.\n");
    char choice;
    scanf(" %c", &choice);
    if (choice == 'c' || choice == 'C') {
        system("clear");
        puts("The process has canceled.");
        sleep(waiting_time);
        return cancel;
    }
    return success;
}

int get_remove_input(remove_student_parameter *input) {
    system("clear");
    puts("Student list:\n");
    if (get_data("student_id", "STUDENTS") == not_found) {
        puts("Student list is empty!");
        sleep(waiting_time);
        return not_found;
    }
    puts("");
    printf("Student_id: ");
    scanf(" %[^'\n']", input->user_id);
    printf("Enter c to cancel or s to submit.\n");
    char choice;
    scanf(" %c", &choice);
    if (choice == 'c' || choice == 'C') {
        system("clear");
        puts("The process has canceled.");
        sleep(waiting_time);
        return cancel;
    }
    return success;
}

int get_deactivate_input(deactivate_parameter *input) {
    system("clear");
    puts("Active student list:\n");
    if (get_data("student_id", "STUDENTS where activate = 1") == not_found) {
        puts("Active student list is empty!");
        sleep(waiting_time);
        return not_found;
    }
    puts("");
    printf("Student_id: ");
    scanf(" %[^'\n']", input->user_id);
    printf("Enter c to cancel or s to submit.\n");
    char choice;
    scanf(" %c", &choice);
    if (choice == 'c' || choice == 'C') {
        system("clear");
        puts("The process has canceled.");
        sleep(waiting_time);
        return cancel;
    }
    return success;
}

int get_poll_input(poll_parameter *input) {
    system("clear");
    printf("Question: ");
    scanf(" %[^'\n']", input->question);
    printf("Option 1: ");
    scanf(" %[^'\n']", input->option1);
    printf("Option 2: ");
    scanf(" %[^'\n']", input->option2);
    printf("Option 3 (enter to ignore): ");
    scanf(" %[^'\n']", input->option3);
    printf("Option 4 (enter to ignore): ");
    scanf(" %[^'\n']", input->option4);
    printf("End-date(YYYY-MM-DD): ");
    scanf(" %[^'\n']", input->end_date);
    printf("Enter c to cancel or s to submit.\n");
    char choice;
    scanf(" %c", &choice);
    if (choice == 'c' || choice == 'C') {
        system("clear");
        puts("The process has canceled.");
        sleep(waiting_time);
        return cancel;
    }
    return success;
}

int get_news_input(news_parameter *input) {
    system("clear");
    printf("Title: ");
    scanf(" %[^'\n']", input->title);
    printf("Content: ");
    scanf(" %[^'\n']", input->content);
    printf("End-date(YYYY-MM-DD): ");
    scanf(" %[^'\n']", input->end_date);
    printf("Enter c to cancel or s to submit.\n");
    char choice;
    scanf(" %c", &choice);
    if (choice == 'c' || choice == 'C') {
        system("clear");
        puts("The process has canceled.");
        sleep(waiting_time);
        return cancel;
    }
    return success;
}

int get_meal_plan_input(meal_plan_parameter *input) {
    system("clear");
    printf("Self list:\n");
    if (get_data("*", "SELF") == not_found) {
        puts("Self list is empty!");
        sleep(waiting_time);
        return not_found;
    }
    puts("");
    printf("Self_id: ");
    scanf(" %[^'\n']", input->self_id);
    printf("Type(lunch/dinner): ");
    scanf(" %[^'\n']", input->type);
    system("clear");
    printf("Food list:\n");
    if (get_data("*", "FOOD") == not_found) {
        puts("Food list is empty!");
        sleep(waiting_time);
        return not_found;
    }
    puts("");
    printf("Food_id: ");
    scanf(" %[^'\n']", input->food_id);
    printf("Count: ");
    scanf(" %[^'\n']", input->count);
    printf("Date(YYYY-MM-DD): ");
    scanf(" %[^'\n']", input->date);
    printf("Enter c to cancel or s to submit.\n");
    char choice;
    scanf(" %c", &choice);
    if (choice == 'c' || choice == 'C') {
        system("clear");
        puts("The process has canceled.");
        sleep(waiting_time);
        return cancel;
    }
    return success;
}

int get_food_input(food_parameter *input) {
    system("clear");
    printf("Food_id: ");
    scanf(" %[^'\n']", input->food_id);
    printf("Food_name: ");
    scanf(" %[^'\n']", input->name);
    printf("Type (food/dessert): ");
    scanf(" %[^'\n']", input->type);
    printf("Price: ");
    scanf(" %[^'\n']", input->price);
    printf("Enter c to cancel or s to submit.\n");
    char choice;
    scanf(" %c", &choice);
    if (choice == 'c' || choice == 'C') {
        system("clear");
        puts("The process has canceled.");
        sleep(waiting_time);
        return cancel;
    }
    return success;
}

int get_self_input(self_parameter *input) {
    system("clear");
    printf("Self_id: ");
    scanf(" %[^'\n']", input->self_id);
    printf("Name: ");
    scanf(" %[^'\n']", input->name);
    printf("Location: ");
    scanf(" %[^'\n']", input->location);
    printf("Capacity: ");
    scanf(" %[^'\n']", input->capacity);
    printf("Type: ");
    scanf(" %[^'\n']", input->type);
    printf("Meal: ");
    scanf(" %[^'\n']", input->meal);
    printf("Lunch-time-start(HHMM): ");
    scanf(" %[^'\n']", input->lunch_time_start);
    printf("Lunch-time-end(HHMM): ");
    scanf(" %[^'\n']", input->lunch_time_end);
    printf("Dinner-time-start(HHMM): ");
    scanf(" %[^'\n']", input->dinner_time_start);
    printf("Dinner-time-end(HHMM): ");
    scanf(" %[^'\n']", input->dinner_time_end);
    printf("Enter c to cancel or s to submit.\n");
    char choice;
    scanf(" %c", &choice);
    if (choice == 'c' || choice == 'C') {
        system("clear");
        puts("The process has canceled.");
        sleep(waiting_time);
        return cancel;
    }
    return success;
}

int get_approve_input(approve_parameter *input) {
    printf("Enter user_id or type :stop to end the process.\n");
    printf("User_id: ");
    scanf(" %[^'\n']", input->user_id);
    if (strcmp(input->user_id, ":stop") == 0) {
        return stop;
    }
    return success;
}

int get_register_input(register_parameter *input) {
    system("clear");
    printf("Name: ");
    scanf(" %[^'\n']", input->name);
    printf("Family: ");
    scanf(" %[^'\n']", input->family);
    printf("User_id: ");
    scanf(" %[^'\n']", input->user_id);
    char *temp_pass;
    temp_pass = getpass("Password: ");
    strcpy(input->password, temp_pass);
    printf("National-id-code: ");
    scanf(" %[^'\n']", input->national_id);
    printf("Birthdate(YYYY-MM-DD): ");
    scanf(" %[^'\n']", input->birthdate);
    printf("Gender: ");
    scanf(" %[^'\n']", input->gender);
    printf("Type: ");
    scanf(" %[^'\n']", input->type);
    printf("Enter c to cancel or s to submit.\n");
    char choice;
    scanf(" %c", &choice);
    if (choice == 'c' || choice == 'C') {
        system("clear");
        puts("The process has canceled.");
        sleep(waiting_time);
        return cancel;
    }
    return success;
}

void get_login_input(login_parameter *input) {
    system("clear");
    printf("User_id: ");
    scanf(" %[^'\n']", input->username);
    char *temp_pass;
    temp_pass = getpass("Password: ");
    strcpy(input->password, temp_pass);
}

void student_menu(void) {
    system("clear");
    printf("1)  Reserve food\n"
           "2)  Take food\n"
           "3)  Charge account\n"
           "4)  Send charge\n"
           "5)  Cancel reserve\n"
           "6)  Daily reserve\n"
           "7)  Define agent\n"
           "8)  Change self\n"
           "9)  Check news\n"
           "10) Vote\n"
           "11) Register\n"
           "12) Change password\n"
           "13) Change date-time\n"
           "14) Reserved meal plan\n"
           "15) Financial report\n"
           "16) Taken meal report\n"
           "17) Logout\n"
           "18) Exit\n");
    scanf("%d", &choice);
    int result;
    switch (choice) {
        case 1:
            if (get_reserve_input(&reserve_input) != success) {
                student_menu();
                break;
            }
            result = reserve(reserve_input.self_id, reserve_input.date, reserve_input.meal, reserve_input.food_id, 1, 0);
            while (result == insufficient_money) {
                system("clear");
                puts("Insufficient money! Charge your account.");
                sleep(waiting_time);
                if (get_charge_account_input(&charge_account_input) == cancel) {
                    student_menu();
                    break;
                }
                if (charge_account(charge_account_input.amount) == sql_err) {
                    system("clear");
                    puts("SQL ERROR HAPPENED!");
                    sleep(waiting_time);
                    student_menu();
                    break;
                }
                else {
                    system("clear");
                    puts("Your account successfully charged.");
                    result = reserve(reserve_input.self_id, reserve_input.date, reserve_input.meal, reserve_input.food_id, 1, 0);
                }
            }
            if (result == sql_err) {
                system("clear");
                puts("SQL ERROR HAPPENED!");
                sleep(waiting_time);
                student_menu();
                break;
            }
            else if (result == permission_denied) {
                system("clear");
                puts("Can't reserve meal at this time!");
                sleep(waiting_time);
                student_menu();
                break;
            }
            else if (result == not_found) {
                system("clear");
                puts("Meal plan not found!");
                sleep(waiting_time);
                student_menu();
                break;
            }
            else if (result == success) {
                system("clear");
                puts("Successfully reserved.");
                sleep(waiting_time);
                student_menu();
                break;
            }
            break;
        case 2:
            if (get_take_food_input(&take_food_input) != success) {
                student_menu();
                break;
            }
            result = take_food(take_food_input.self_id, take_food_input.date, take_food_input.meal);
            if (result == sql_err) {
                system("clear");
                puts("SQL ERROR HAPPENED!");
                sleep(waiting_time);
                student_menu();
                break;
            }
            else if (result == permission_denied) {
                system("clear");
                puts("You can't take meal at this time.");
                sleep(waiting_time);
                student_menu();
                break;
            }
            else if (result == not_found) {
                system("clear");
                puts("Reserved meal not found!");
                sleep(waiting_time);
                student_menu();
                break;
            }
            else {
                system("clear");
                puts("You took your meal successfully.");
                sleep(waiting_time);
                student_menu();
                break;
            }
            break;
        case 3:
            if (get_charge_account_input(&charge_account_input) != success) {
                student_menu();
                break;
            }
            if (charge_account(charge_account_input.amount) == sql_err) {
                system("clear");
                puts("SQL ERROR HAPPENED!");
                sleep(waiting_time);
                student_menu();
                break;
            }
            else {
                system("clear");
                puts("Your account successfully charged.");
                sleep(waiting_time);
                student_menu();
                break;
            }
            break;
        case 4:
            if (get_send_charge_input(&send_charge_input) != success) {
                student_menu();
                break;
            }
            result = send_charge(send_charge_input.user_id, send_charge_input.amount, send_charge_input.name);
            if (result == sql_err) {
                system("clear");
                puts("SQL ERROR HAPPENED!");
                sleep(waiting_time);
                student_menu();
                break;
            }
            else if (result == permission_denied) {
                system("clear");
                puts("You have NOT enough money!");
                sleep(waiting_time);
                student_menu();
                break;
            }
            else {
                system("clear");
                puts("Charge has successfully sent.");
                sleep(waiting_time);
                student_menu();
                break;
            }
            break;
        case 5:
            if (get_cancel_reserve_input(&cancel_reserve_input) != success) {
                student_menu();
                break;
            }
            result = cancel_reserve(cancel_reserve_input.date, cancel_reserve_input.meal);
            if (result == sql_err) {
                system("clear");
                puts("SQL ERROR HAPPENED!");
                sleep(waiting_time);
                student_menu();
                break;
            }
            else if (result == permission_denied) {
                system("clear");
                puts("You can't cancel reserved meal at this time.");
                sleep(waiting_time);
                student_menu();
                break;
            }
            else if (result == not_found) {
                system("clear");
                puts("Reserved meal not found!");
                sleep(waiting_time);
                student_menu();
                break;
            }
            else {
                system("clear");
                puts("Reserved meal has canceled successfully.");
                sleep(waiting_time);
                student_menu();
                break;
            }
            break;
        case 6:
            if (get_daily_reserve_input(&daily_reserve_input) != success) {
                student_menu();
                break;
            }
            result = daily_reserve(daily_reserve_input.self_id, daily_reserve_input.food_id);
            if (result == sql_err) {
                system("clear");
                puts("SQL ERROR HAPPENED!");
                sleep(waiting_time);
                student_menu();
                break;
            }
            else if (result == permission_denied) {
                system("clear");
                puts("Can't reserve meal at this time!");
                sleep(waiting_time);
                student_menu();
                break;
            }
            else if (result == not_found) {
                system("clear");
                puts("Meal plan not found!");
                sleep(waiting_time);
                student_menu();
                break;
            }
            else if (result == success) {
                system("clear");
                puts("Successfully reserved.");
                sleep(waiting_time);
                student_menu();
                break;
            }
            while (result == insufficient_money) {
                system("clear");
                puts("Insufficient money! Charge your account.");
                if (get_charge_account_input(&charge_account_input) == cancel) {
                    student_menu();
                    break;
                }
                if (charge_account(charge_account_input.amount) == sql_err) {
                    system("clear");
                    puts("SQL ERROR HAPPENED!");
                    sleep(waiting_time);
                    student_menu();
                    break;
                }
                else {
                    system("clear");
                    puts("Your account successfully charged.");
                }
                result = daily_reserve(daily_reserve_input.self_id, daily_reserve_input.food_id);
            }
            break;
        case 7:
            if (get_agent_input(&agent_input) != success) {
                student_menu();
                break;
            }
            result = define_agent(agent_input.date, agent_input.meal, agent_input.user_id);
            if (result == sql_err) {
                system("clear");
                puts("SQL ERROR HAPPENED!");
                sleep(waiting_time);
                student_menu();
                break;
            }
            else if (result == not_found) {
                system("clear");
                puts("Agent or reserved meal not found!");
                sleep(waiting_time);
                student_menu();
                break;
            }
            else {
                system("clear");
                puts("Agent has successfully defined.");
                sleep(waiting_time);
                student_menu();
                break;
            }
            break;
        case 8:
            if (get_change_self_input(&change_self_input) != success) {
                student_menu();
                break;
            }
            result = change_self(change_self_input.date, change_self_input.meal, change_self_input.new_self);
            if (result == sql_err) {
                system("clear");
                puts("SQL ERROR HAPPENED!");
                sleep(waiting_time);
                student_menu();
                break;
            }
            else if (result == not_found) {
                system("clear");
                puts("Reserved meal or new self not found!");
                sleep(waiting_time);
                student_menu();
                break;
            }
            else if (result == permission_denied) {
                system("clear");
                puts("You can't change self at this time.");
                sleep(waiting_time);
                student_menu();
                break;
            }
            else {
                system("clear");
                puts("Self has changed successfully.");
                sleep(waiting_time);
                student_menu();
                break;
            }
            break;
        case 9:
            if (check_news(0) == not_found) {
                system("clear");
                puts("There is no news at this time.");
                sleep(waiting_time);
                student_menu();
                break;
            }
            else {
                puts("Press any key to back.");
                char k;
                scanf(" %c", &k);
                student_menu();
                break;
            }
            break;
        case 10:
            if (get_vote_input(&vote_input) != success) {
                student_menu();
                break;
            }
            result = vote(vote_input.poll_id, vote_input.option, 0);
            if (result == sql_err) {
                system("clear");
                puts("SQL ERROR HAPPENED!");
                sleep(waiting_time);
                student_menu();
                break;
            }
            else if (result == not_found) {
                system("clear");
                puts("Poll not found!");
                sleep(waiting_time);
                student_menu();
                break;
            }
            else {
                system("clear");
                puts("Your vote has successfully recorded.");
                sleep(waiting_time);
                student_menu();
                break;
            }
            break;
        case 11:
            if (get_register_input(&register_input) != success) {
                student_menu();
                break;
            }
            result = user_register(register_input.name, register_input.family, register_input.user_id, register_input.password, register_input.national_id, register_input.birthdate, register_input.gender, register_input.type);
            if (result == permission_denied) {
                system("clear");
                puts("This username or password already exists.");
                sleep(waiting_time);
                student_menu();
                break;
            }
            else if (result == sql_err) {
                system("clear");
                puts("SQL ERROR HAPPENED!");
                sleep(waiting_time);
                student_menu();
                break;
            }
            else {
                system("clear");
                puts("Successfully registerd. Please wait for your account to be approved.");
                sleep(waiting_time);
                student_menu();
                break;
            }
            break;
        case 12:
            if (get_change_pass_input(&change_pass_input) != success) {
                student_menu();
                break;
            }
            result = change_pass(change_pass_input.user_id, change_pass_input.old_pass, change_pass_input.new_pass);
            if (result == permission_denied) {
                system("clear");
                puts("Wrong password.");
                sleep(waiting_time);
                student_menu();
                break;
            }
            else if (result == not_found) {
                system("clear");
                puts("Wrong username.");
                sleep(waiting_time);
                student_menu();
                break;
            }
            else if (result == sql_err) {
                system("clear");
                puts("SQL ERROR HAPPENED!");
                sleep(waiting_time);
                student_menu();
                break;
            }
            else {
                system("clear");
                puts("Password successfully changed.");
                sleep(waiting_time);
                student_menu();
                break;
            }
            break;
        case 13:
            if (get_datetime_input(&datetime_input) != success) {
                student_menu();
                break;
            }
            change_datetime(datetime_input.date, datetime_input.time);
            puts("Date and time successfully changes.");
            sleep(waiting_time);
            student_menu();
            break;
        case 14:
            get_week();
            result = get_reserved_meal_plan(week);
            if (result == not_found) {
                system("clear");
                puts("No result!");
                sleep(waiting_time);
                student_menu();
                break;
            }
            else if (result == sql_err) {
                system("clear");
                puts("SQL ERROR HAPPENED!");
                sleep(waiting_time);
                student_menu();
                break;
            }
            else {
                puts("Press any key to back.");
                char k;
                scanf(" %c", &k);
                student_menu();
                break;
            }
            break;
        case 15:
            result = get_financial_report();
            if (result == not_found) {
                system("clear");
                puts("No result!");
                sleep(waiting_time);
                student_menu();
                break;
            }
            else if (result == sql_err) {
                system("clear");
                puts("SQL ERROR HAPPENED!");
                sleep(waiting_time);
                student_menu();
                break;
            }
            else {
                puts("Press any key to back.");
                char k;
                scanf(" %c", &k);
                student_menu();
                break;
            }
            break;
        case 16:
            result = get_taken_meal_report();
            if (result == not_found) {
                system("clear");
                puts("No result!");
                sleep(waiting_time);
                student_menu();
                break;
            }
            else if (result == sql_err) {
                system("clear");
                puts("SQL ERROR HAPPENED!");
                sleep(waiting_time);
                student_menu();
                break;
            }
            else {
                puts("Press any key to back.");
                char k;
                scanf(" %c", &k);
                student_menu();
                break;
            }
            break;
        case 17:
            puts("Are you sure to logout?(y or n)");
            char choice;
            scanf(" %c", &choice);
            if (choice == 'n' || choice == 'N') {
                system("clear");
                puts("The process has canceled.");
                sleep(waiting_time);
                student_menu();
                break;
            }
            user_logout(current_user.username);
            system("clear");
            puts("Successfully logout.");
            sleep(waiting_time);
            main_menu();
            break;
        case 18:
            puts("Are you sure to exit?(y or n)");
            scanf(" %c", &choice);
            if (choice == 'n' || choice == 'N') {
                system("clear");
                puts("The process has canceled.");
                sleep(waiting_time);
                student_menu();
                break;
            }
            exit(0);
            break;
        default:
            system("clear");
            puts("Invalid input!");
            sleep(waiting_time);
            student_menu();
            break;
    }
    
}

void admin_menu(void) {
    system("clear");
    printf("1)  Approve\n"
           "2)  Define self\n"
           "3)  Define food\n"
           "4)  Define meal plan\n"
           "5)  Add news\n"
           "6)  Add poll\n"
           "7)  Activate\n"
           "8)  Deactivate\n"
           "9)  Remove student\n"
           "10) Change student password\n"
           "11) Charge student account\n"
           "12) Register\n"
           "13) Change password\n"
           "14) Change date-time\n"
           "15) Statistics\n"
           "16) Student reports\n"
           "17) System reports\n"
           "18) Logout\n"
           "19) Exit\n");
    scanf("%d", &choice);
    int result;
    switch (choice) {
        case 1:
            system("clear");
            printf("Pending list:\n");
            result = get_data("user_id, type", "PENDING");
            if (result == not_found) {
                system("clear");
                puts("Pending list is empty!");
                sleep(waiting_time);
                admin_menu();
                break;
            }
            puts("");
            while (get_approve_input(&approve_input) != stop) {
                result = approve(approve_input.user_id);
                if (result == not_found) {
                    printf("User %s not found!\n", approve_input.user_id);
                }
                else if (result == sql_err){
                    system("clear");
                    puts("SQL ERROR HAPPENED!");
                    sleep(waiting_time);
                    admin_menu();
                    break;
                }
                else {
                    printf("User %s successfully approved.\n", approve_input.user_id);
                }
            }
            admin_menu();
            break;
        case 2:
            if (get_self_input(&self_input) != success) {
                admin_menu();
                break;
            }
            result = define_self(self_input.self_id, self_input.name, self_input.location, self_input.capacity, self_input.type, self_input.meal, self_input.lunch_time_start, self_input.lunch_time_end, self_input.dinner_time_start, self_input.dinner_time_end);
            if (result == permission_denied) {
                system("clear");
                printf("Self %s has been defined!\n", self_input.self_id);
                sleep(waiting_time);
                admin_menu();
                break;
            }
            else if (result == sql_err) {
                system("clear");
                puts("SQL ERROR HAPPENED!");
                sleep(waiting_time);
                admin_menu();
                break;
            }
            else {
                system("clear");
                printf("Self %s successfully defined.\n", self_input.self_id);
                sleep(waiting_time);
                admin_menu();
                break;
            }
            break;
        case 3:
            if (get_food_input(&food_input) != success) {
                admin_menu();
                break;
            }
            result = define_food(food_input.food_id, food_input.name, food_input.type, food_input.price);
            if (result == permission_denied) {
                system("clear");
                printf("Food %s has been defined!\n", food_input.food_id);
                sleep(waiting_time);
                admin_menu();
                break;
            }
            else if (result == sql_err){
                system("clear");
                puts("SQL ERROR HAPPENED!");
                sleep(waiting_time);
                admin_menu();
                break;
            }
            else {
                system("clear");
                printf("Food %s successfully defined.\n", food_input.food_id);
                sleep(waiting_time);
                admin_menu();
                break;
            }
            break;
        case 4:
            if (get_meal_plan_input(&meal_plan_input) != success) {
                admin_menu();
                break;
            }
            result = define_meal_plan(meal_plan_input.self_id, meal_plan_input.date, meal_plan_input.type, meal_plan_input.food_id, meal_plan_input.count);
            if (result == not_found) {
                system("clear");
                printf("Food or self for %s has not found!\n", meal_plan_input.type);
                sleep(waiting_time);
                admin_menu();
                break;
            }
            else if (result == sql_err) {
                system("clear");
                puts("SQL ERROR HAPPENED!");
                sleep(waiting_time);
                admin_menu();
                break;
            }
            else {
                system("clear");
                puts("Meal plan successfully defined.");
                sleep(waiting_time);
                admin_menu();
                break;
            }
            break;
        case 5:
            if (get_news_input(&news_input) != success) {
                admin_menu();
                break;
            }
            result = add_news(news_input.title, news_input.content, news_input.end_date);
            if (result == sql_err) {
                system("clear");
                puts("SQL ERROR HAPPENED!");
                sleep(waiting_time);
                admin_menu();
                break;
            }
            else {
                system("clear");
                puts("News successfully added.");
                sleep(waiting_time);
                admin_menu();
                break;
            }
            break;
        case 6:
            if (get_poll_input(&poll_input) != success) {
                admin_menu();
                break;
            }
            result = add_poll(poll_input.question, poll_input.option1, poll_input.option2, poll_input.option3, poll_input.option4, poll_input.end_date);
            if (result == sql_err) {
                system("clear");
                puts("SQL ERROR HAPPENED!");
                sleep(waiting_time);
                admin_menu();
                break;
            }
            else {
                system("clear");
                puts("Poll successfully added.");
                sleep(waiting_time);
                admin_menu();
                break;
            }
            break;
        case 7:
            system("clear");
            printf("Deactivated student list:\n");
            result = get_data("student_id", "STUDENTS where activate = 0");
            if (result == not_found) {
                system("clear");
                puts("Deactivated student list is empty!");
                sleep(waiting_time);
                admin_menu();
                break;
            }
            puts("");
            while (get_approve_input(&approve_input) != stop) {
                result = approve(approve_input.user_id);
                if (result == not_found) {
                    printf("User %s not found!\n", approve_input.user_id);
                }
                else if (result == sql_err) {
                    system("clear");
                    puts("SQL ERROR HAPPENED!");
                    sleep(waiting_time);
                    admin_menu();
                    break;
                }
                else {
                    printf("User %s successfully activated.\n", approve_input.user_id);
                }
            }
            admin_menu();
            break;
        case 8:
            if (get_deactivate_input(&deactivate_input) != success) {
                admin_menu();
                break;
            }
            result = deactivate(deactivate_input.user_id);
            if (result == not_found) {
                system("clear");
                printf("User %s not found!\n", deactivate_input.user_id);
                sleep(waiting_time);
                admin_menu();
                break;
            }
            else if (result == sql_err) {
                system("clear");
                puts("SQL ERROR HAPPENED!");
                sleep(waiting_time);
                admin_menu();
                break;
            }
            else {
                system("clear");
                printf("User %s successfully deactivated.\n", deactivate_input.user_id);
                sleep(waiting_time);
                admin_menu();
                break;
            }
            break;
        case 9:
            if (get_remove_input(&remove_input) != success) {
                admin_menu();
                break;
            }
            result = remove_student(remove_input.user_id);
            if (result == sql_err) {
                system("clear");
                puts("SQL ERROR HAPPENED!");
                sleep(waiting_time);
                admin_menu();
                break;
            }
            else {
                system("clear");
                printf("User %s successfully removed.\n", remove_input.user_id);
                sleep(waiting_time);
                admin_menu();
                break;
            }
            break;
        case 10:
            if (get_change_student_pass_input(&change_student_pass_input) != success) {
                admin_menu();
                break;
            }
            result = change_student_pass(change_student_pass_input.user_id, change_student_pass_input.new_pass);
            if (result == not_found) {
                system("clear");
                printf("User %s not found!\n", change_student_pass_input.user_id);
                sleep(waiting_time);
                admin_menu();
                break;
            }
            else if (result == sql_err) {
                system("clear");
                puts("SQL ERROR HAPPENED!");
                sleep(waiting_time);
                admin_menu();
                break;
            }
            else {
                system("clear");
                puts("User pass successfully changed.");
                sleep(waiting_time);
                admin_menu();
                break;
            }
            break;
        case 11:
            if (get_charge_student_account_input(&charge_student_account_input) != success) {
                admin_menu();
                break;
            }
            result = charge_student_account(charge_student_account_input.user_id, charge_student_account_input.amount);
            if (result == not_found) {
                system("clear");
                printf("User %s not found!\n", charge_student_account_input.user_id);
                sleep(waiting_time);
                admin_menu();
                break;
            }
            else if (result == sql_err) {
                system("clear");
                puts("SQL ERROR HAPPENED!");
                sleep(waiting_time);
                admin_menu();
                break;
            }
            else {
                system("clear");
                puts("User account successfully charged.");
                sleep(waiting_time);
                admin_menu();
                break;
            }
            break;
        case 12:
            if (get_register_input(&register_input) != success) {
                admin_menu();
                break;
            }
            result = user_register(register_input.name, register_input.family, register_input.user_id, register_input.password, register_input.national_id, register_input.birthdate, register_input.gender, register_input.type);
            if (result == permission_denied) {
                system("clear");
                puts("This username or password already exists.");
                sleep(waiting_time);
                admin_menu();
                break;
            }
            else if (result == sql_err) {
                system("clear");
                puts("SQL ERROR HAPPENED!");
                sleep(waiting_time);
                admin_menu();
                break;
            }
            else {
                system("clear");
                printf("User %s successfully registerd.\n", register_input.user_id);
                sleep(waiting_time);
                admin_menu();
                break;
            }
            break;
        case 13:
            if (get_change_pass_input(&change_pass_input) != success) {
                admin_menu();
                break;
            }
            result = change_pass(change_pass_input.user_id, change_pass_input.old_pass, change_pass_input.new_pass);
            if (result == permission_denied) {
                system("clear");
                puts("Wrong password.");
                sleep(waiting_time);
                admin_menu();
                break;
            }
            else if (result == not_found) {
                system("clear");
                puts("Wrong username.");
                sleep(waiting_time);
                admin_menu();
                break;
            }
            else if (result == sql_err) {
                system("clear");
                puts("SQL ERROR HAPPENED!");
                sleep(waiting_time);
                admin_menu();
                break;
            }
            else {
                system("clear");
                puts("Password successfully changed.");
                sleep(waiting_time);
                admin_menu();
                break;
            }
            break;
        case 14:
            if (get_datetime_input(&datetime_input) != success) {
                admin_menu();
                break;
            }
            change_datetime(datetime_input.date, datetime_input.time);
            puts("Date and time successfully changes.");
            sleep(waiting_time);
            admin_menu();
            break;
        case 15:
            if (get_statistics_input(&statistics_input) != success) {
                admin_menu();
                break;
            }
            result = statistics(statistics_input.date, statistics_input.meal, statistics_input.is_taken);
            if (result == not_found) {
                system("clear");
                puts("No result.");
                sleep(waiting_time);
                admin_menu();
                break;
            }
            else if (result == sql_err) {
                system("clear");
                puts("SQL ERROR HAPPENED!");
                sleep(waiting_time);
                admin_menu();
                break;
            }
            else {
                puts("Press any key to back to menu.");
                char k;
                scanf(" %c", &k);
                admin_menu();
                break;
            }
            break;
        case 16:
            if (get_student_report_input(&student_report_input) != success) {
                admin_menu();
                break;
            }
            result = get_student_report(student_report_input.start_date, student_report_input.end_date, student_report_input.student_id);
            if (result == not_found) {
                system("clear");
                puts("No result.");
                sleep(waiting_time);
                admin_menu();
                break;
            }
            else if (result == sql_err) {
                system("clear");
                puts("SQL ERROR HAPPENED!");
                sleep(waiting_time);
                admin_menu();
                break;
            }
            else {
                puts("Press any key to back to menu.");
                char k;
                scanf(" %c", &k);
                admin_menu();
                break;
            }
            break;
        case 17:
            if (get_system_report_input(&system_report_input) != success) {
                admin_menu();
                break;
            }
            result = get_system_report(system_report_input.start_date, system_report_input.end_date, system_report_input.affair);
            if (result == not_found) {
                system("clear");
                puts("No result.");
                sleep(waiting_time);
                admin_menu();
                break;
            }
            else if (result == sql_err) {
                system("clear");
                puts("SQL ERROR HAPPENED!");
                sleep(waiting_time);
                admin_menu();
                break;
            }
            else {
                puts("Press any key to back to menu.");
                char k;
                scanf(" %c", &k);
                admin_menu();
                break;
            }
            break;
        case 18:
            puts("Are you sure to logout?(y or n)");
            char choice;
            scanf(" %c", &choice);
            if (choice == 'n' || choice == 'N') {
                system("clear");
                puts("The process has canceled.");
                sleep(waiting_time);
                student_menu();
                break;
            }
            user_logout(current_user.username);
            system("clear");
            puts("Successfully logout.");
            sleep(waiting_time);
            main_menu();
            break;
        case 19:
            puts("Are you sure to exit?(y or n)");
            scanf(" %c", &choice);
            if (choice == 'n' || choice == 'N') {
                system("clear");
                puts("The process has canceled.");
                sleep(waiting_time);
                student_menu();
                break;
            }
            exit(0);
            break;
        default:
            system("clear");
            puts("Invalid input!");
            sleep(waiting_time);
            admin_menu();
            break;
    }
}

void main_menu(void) {
    system("clear");
    printf("1) Login\n"
           "2) Register\n"
           "3) Exit\n");
    scanf("%d", &choice);
    int result;
    switch (choice) {
        case 1:
            get_login_input(&login_input);
            result = user_login(login_input.username, login_input.password);
            if (result == permission_denied) {
                system("clear");
                puts("You can't login! Another user has logged-in.");
                sleep(waiting_time);
                main_menu();
                break;
            }
            if (result == not_found) {
                system("clear");
                puts("Incorrect username or password.");
                sleep(waiting_time);
                main_menu();
                break;
            }
            else if (result == sql_err) {
                system("clear");
                puts("SQL ERROR HAPPENED!");
                sleep(waiting_time);
                main_menu();
                break;
            }
            if (current_user.user_type == student) {
                system("clear");
                printf("User '%s' successfully logged-in.\n", current_user.username);
                sleep(waiting_time);
                student_menu();
                break;
            }
            else {
                system("clear");
                printf("User '%s' successfully logged-in.\n", current_user.username);
                sleep(waiting_time);
                admin_menu();
                break;
            }
            break;
        case 2:
            if (get_register_input(&register_input) != success) {
                main_menu();
                break;
            }
            result = user_register(register_input.name, register_input.family, register_input.user_id, register_input.password, register_input.national_id, register_input.birthdate, register_input.gender, register_input.type);
            if (result == permission_denied) {
                system("clear");
                puts("This username or password already exists.");
                sleep(waiting_time);
                main_menu();
                break;
            }
            else if (result == sql_err) {
                system("clear");
                puts("SQL ERROR HAPPENED!");
                sleep(waiting_time);
                main_menu();
                break;
            }
            else {
                system("clear");
                puts("Successfully registerd. Please wait for your account to be approved.");
                sleep(waiting_time);
                main_menu();
                break;
            }
            break;
        case 3:
            puts("Are you sure to exit?(y or n)");
            char choice;
            scanf(" %c", &choice);
            if (choice == 'n' || choice == 'N') {
                system("clear");
                puts("The process has canceled.");
                sleep(waiting_time);
                student_menu();
                break;
            }
            exit(0);
            break;
        default:
            system("clear");
            puts("Invalid input!");
            sleep(waiting_time);
            main_menu();
            break;
    }
}

