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

int choice;
static login_parameter login_input;
static register_parameter register_input;
static approve_parameter approve_input;
static self_parameter self_input;

int print_data(void *data, int argc, char **argv, char **col_name) {
    *((int *)data) = success;
    for (int i = 0; i < argc; ++i) {
        printf("%s: %s\n", col_name[i], argv[i]);
    }
    return 0;
}

int get_data(const char *columns, const char *tbl_name) {
    system("clear");
    char *errmsg = NULL;
    char sql[max_size];
    sprintf(sql, "select %s from %s;", columns, tbl_name);
    int exist = not_found;
    int rc = sqlite3_exec(db, sql, print_data, &exist, &errmsg);
    if (rc != SQLITE_OK) {
        printf("SQL error: %s", errmsg);
        sqlite3_free(errmsg);
        return permission_denied;
    }
    if (exist == not_found) {
        return not_found;
    }
    return success;
}

void get_self_input(self_parameter *input) {
    printf("Self_id: ");
    scanf("%s", input->self_id);
    printf("Name: ");
    scanf("%s", input->name);
    printf("Location: ");
    scanf("%s", input->location);
    printf("Capacity: ");
    scanf("%s", input->capacity);
    printf("Type: ");
    scanf("%s", input->type);
    printf("Meal: ");
    scanf("%s", input->meal);
    printf("Lunch-time-start(HHMM): ");
    scanf("%s", input->lunch_time_start);
    printf("Lunch-time-end(HHMM): ");
    scanf("%s", input->lunch_time_end);
    printf("Dinner-time-start(HHMM): ");
    scanf("%s", input->dinner_time_start);
    printf("Dinner-time-end(HHMM): ");
    scanf("%s", input->dinner_time_end);
}

int get_approve_input(approve_parameter *input) {
    printf("Enter user_id or type :stop to end the process.\n");
    printf("User_id: ");
    scanf("%s", input->user_id);
    if (strcmp(input->user_id, ":stop") == 0) {
        return stop;
    }
    return success;
}

void get_register_input(register_parameter *input) {
    system("clear");
    printf("Name: ");
    scanf("%s", input->name);
    printf("Family: ");
    scanf("%s", input->family);
    printf("User_id: ");
    scanf("%s", input->user_id);
    char *temp_pass;
    temp_pass = getpass("Password: ");
    strcpy(input->password, temp_pass);
    printf("National-id-code: ");
    scanf("%s", input->national_id);
    printf("Birthdate: ");
    scanf("%s", input->birthdate);
    printf("Gender: ");
    scanf("%s", input->gender);
    printf("Type: ");
    scanf("%s", input->type);
}

void get_login_input(login_parameter *input) {
    system("clear");
    printf("User_id: ");
    scanf("%s", input->username);
    printf("Password: ");
    scanf("%s", input->password);
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
           "14) Logout\n"
           "15) Exit\n");
    
    
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
           "15) Logout\n"
           "16) Exit\n");
    scanf("%d", &choice);
    int result;
    switch (choice) {
        case 1:
            printf("Pending list:\n");
            result = get_data("user_id, type", "PENDING");
            if (result == not_found) {
                puts("Pending list is empty!");
                sleep(5);
                admin_menu();
            }
            while (get_approve_input(&approve_input) != stop) {
                result = approve(approve_input.user_id);
                if (result == not_found) {
                    printf("User %s not found!\n", approve_input.user_id);
                }
                else {
                    printf("User %s successfully approved.\n", approve_input.user_id);
                }
            }
            admin_menu();
            break;
        case 2:
            get_self_input(&self_input);
            result = define_self(self_input.self_id, self_input.name, self_input.location, self_input.capacity, self_input.type, self_input.meal, self_input.lunch_time_start, self_input.lunch_time_end, self_input.dinner_time_start, self_input.dinner_time_end);
            if (result == permission_denied) {
                printf("Self %s has been defined!\n", self_input.self_id);
                sleep(5);
                admin_menu();
            }
            else {
                printf("Self %s successfully defined.\n", self_input.self_id);
                sleep(5);
                admin_menu();
            }
            break;
        case 3:
            break;
        case 4:
            break;
        case 5:
            break;
        case 6:
            break;
        case 7:
            break;
        case 8:
            break;
        case 9:
            break;
        case 10:
            break;
        case 11:
            break;
        case 12:
            break;
        case 13:
            break;
        case 14:
            break;
        case 15:
            
            main_menu();
            break;
        case 16:
            exit(0);
            break;
        default:
            puts("Invalid input!");
            admin_menu();
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
                puts("You can't login! Another user has logged-in.");
                main_menu();
            }
            if (result == not_found) {
                puts("Incorrect username or password.");
                main_menu();
            }
            if (current_user.user_type == student) {
                printf("User '%s' successfully logged-in.\n", current_user.username);
                student_menu();
            }
            else {
                printf("User '%s' successfully logged-in.\n", current_user.username);
                admin_menu();
            }
            break;
        case 2:
            get_register_input(&register_input);
            result = user_register(register_input.name, register_input.family, register_input.user_id, register_input.password, register_input.national_id, register_input.birthdate, register_input.gender, register_input.type);
            if (result == permission_denied) {
                puts("This username or password already exists.");
                main_menu();
            }
            else {
                puts("Successfully registerd. Please wait for your account to be approved");
                main_menu();
            }
            break;
        case 3:
            exit(0);
            break;
        default:
            puts("Invalid input!");
            main_menu();
    }
}

int print_news(void *data, int argc, char **argv, char **col_name) {
    *((int *)data) = success;
    for (int i = 0; i < argc; i += 4) {
        printf("%s) Title: %s\n\tContent: %s\n", argv[i], argv[i + 1], argv[i + 2]);
    }
    return 0;
}
