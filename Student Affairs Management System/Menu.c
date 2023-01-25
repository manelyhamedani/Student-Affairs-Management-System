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

int choice;
login_parameter login_input;
register_parameter register_input;
 
void student_menu(void) {
    system("clear");
    printf("1) Reserve food\n2) Take food\n3) Charge account\n4) ");
}

void admin_menu(void) {
    system("clear");
}

void get_register_input(register_parameter *input) {
    printf("Name: ");
    scanf("%s", input->name);
    printf("Family: ");
    scanf("%s", input->family);
    printf("Username: ");
    scanf("%s", input->user_id);
    printf("Password: ");
    scanf("%s", input->password);
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
    printf("Username: ");
    scanf("%s", input->username);
    printf("Password: ");
    scanf("%s", input->password);
}

void main_menu(void) {
    system("clear");
    printf("1) Login\n2) Register\n3) Exit\n");
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
    }
}

int print_news(void *data, int argc, char **argv, char **col_name) {
    *((int *)data) = success;
    for (int i = 0; i < argc; i += 4) {
        printf("%s) Title: %s\n\tContent: %s\n", argv[i], argv[i + 1], argv[i + 2]);
    }
    return 0;
}
