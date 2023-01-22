//
//  Test Case.c
//  Student Affairs Management System
//
//  Created by Manely Hamedani on 1/20/23.
//

#include "Test Case.h"
#include <string.h>
#include "Utility.h"
#include <sqlite3.h>

extern sqlite3 *db;

#define true                    1
#define max_size_command        30
#define max_size_parameter      100
#define max_size_paramater_name 30

typedef struct {
    char username[max_size_parameter];
    char password[max_size_parameter];
} login_parameter;

typedef struct {
    char username[max_size_parameter];
} logout_parameter;

typedef struct {
    char name[max_size_parameter];
    char family[max_size_parameter];
    char user_id[max_size_parameter];
    char password[max_size_parameter];
    char national_id[max_size_parameter];
    char birthdate[max_size_parameter];
    char gender[max_size_parameter];
    char type[max_size_parameter];
} register_parameter;

enum result {
  success, not_found, permission_denied, invalid
};


int get_login_parameter(FILE *input, login_parameter *parameter) {
    char paramater_name[max_size_paramater_name];
    char colon;
    char separator;
    char end;
    fscanf(input, "%[^:]%c", paramater_name, &colon);
    if (strcmp(paramater_name, "user")) {
        return invalid;
    }
    if (colon != ':') {
        return invalid;
    }
    fscanf(input, "%[^|]", parameter->username);
    fscanf(input, "%c%[^:]%c", &separator, paramater_name, &colon);
    if (separator != '|') {
        return invalid;
    }
    if (strcmp(paramater_name, "password")) {
        return invalid;
    }
    if (colon != ':') {
        return invalid;
    }
    fscanf(input, "%s%c", parameter->password, &end);
    if (end != '\n') {
        return invalid;
    }
    return success;
}

int get_logout_parameter(FILE *input, logout_parameter *parameter) {
    char paramater_name[max_size_paramater_name];
    char colon;
    char end;
    fscanf(input, "%[^:]%c", paramater_name, &colon);
    if (strcmp(paramater_name, "user")) {
        return invalid;
    }
    if (colon != ':') {
        return invalid;
    }
    fscanf(input, "%s%c", parameter->username, &end);
    if (end != '\n') {
        return invalid;
    }
    return success;
}

int get_register_parameter(FILE *input, register_parameter *parameter) {
    char paramater_name[max_size_paramater_name];
    char colon;
    char separator;
    char end;
    fscanf(input, "%[^:]%c", paramater_name, &colon);
    if (strcmp(paramater_name, "name")) {
        return invalid;
    }
    if (colon != ':') {
        return invalid;
    }
    fscanf(input, "%[^|]%c", parameter->name, &separator);
    if (separator != '|') {
        return invalid;
    }
    fscanf(input, "%[^:]%c", paramater_name, &colon);
    if (strcmp(paramater_name, "family")) {
        return invalid;
    }
    if (colon != ':') {
        return invalid;
    }
    fscanf(input, "%[^|]%c", parameter->family, &separator);
    if (separator != '|') {
        return invalid;
    }
    fscanf(input, "%[^:]%c", paramater_name, &colon);
    if (strcmp(paramater_name, "user-id")) {
        return invalid;
    }
    if (colon != ':') {
        return invalid;
    }
    fscanf(input, "%[^|]%c", parameter->user_id, &separator);
    if (separator != '|') {
        return invalid;
    }
    fscanf(input, "%[^:]%c", paramater_name, &colon);
    if (strcmp(paramater_name, "password")) {
        return invalid;
    }
    if (colon != ':') {
        return invalid;
    }
    fscanf(input, "%[^|]%c", parameter->password, &separator);
    if (separator != '|') {
        return invalid;
    }
    fscanf(input, "%[^:]%c", paramater_name, &colon);
    if (strcmp(paramater_name, "national-id-code")) {
        return invalid;
    }
    if (colon != ':') {
        return invalid;
    }
    fscanf(input, "%[^|]%c", parameter->national_id, &separator);
    if (separator != '|') {
        return invalid;
    }
    fscanf(input, "%[^:]%c", paramater_name, &colon);
    if (strcmp(paramater_name, "birthdate")) {
        return invalid;
    }
    if (colon != ':') {
        return invalid;
    }
    fscanf(input, "%[^|]%c", parameter->birthdate, &separator);
    if (separator != '|') {
        return invalid;
    }
    fscanf(input, "%[^:]%c", paramater_name, &colon);
    if (strcmp(paramater_name, "gender")) {
        return invalid;
    }
    if (colon != ':') {
        return invalid;
    }
    fscanf(input, "%[^|]%c", parameter->gender, &separator);
    if (separator != '|') {
        return invalid;
    }
    fscanf(input, "%[^:]%c", paramater_name, &colon);
    if (strcmp(paramater_name, "type")) {
        return invalid;
    }
    if (colon != ':') {
        return invalid;
    }
    fscanf(input, "%[^|]%c", parameter->type, &end);
    if (end != '\n') {
        return invalid;
    }
    return success;
}

void get_command(FILE *input, FILE *output) {
    char command[max_size_command];
    int command_id;
    int result;
    char separator;
    while (true) {
        fscanf(input, "%d#%[^#]%c", &command_id, command, &separator);
        if (feof(input)) {
            break;
        }
        
        if (strcmp(command, "login") == 0) {
            login_parameter parameter;
            result = get_login_parameter(input, &parameter);
            if (result == invalid) {
                fprintf(output, "%d#invalid\n", command_id);
            }
            else {
                result = user_login(parameter.username, parameter.password, db);
                if (result == not_found) {
                    fprintf(output, "%d#not-found\n", command_id);
                }
                else if (result == permission_denied) {
                    fprintf(output, "%d#permission-denied\n", command_id);
                }
                else {
                    fprintf(output, "%d#success\n", command_id);
                }
            }
        }
        
        if (strcmp(command, "logout") == 0) {
            logout_parameter parameter;
            result = get_logout_parameter(input, &parameter);
            if (result == invalid) {
                fprintf(output, "%d#invalid\n", command_id);
            }
            else {
                result = user_logout(parameter.username);
                if (result == not_found) {
                    fprintf(output, "%d#not-found\n", command_id);
                }
                else {
                    fprintf(output, "%d#success\n", command_id);
                }
            }
        }
        
        if (strcmp(command, "register") == 0) {
            register_parameter parameter;
            result = get_register_parameter(input, &parameter);
            if (result == invalid) {
                fprintf(output, "%d#invalid\n", command_id);
            }
            else {
                result = user_register(parameter.name, parameter.family, parameter.user_id, parameter.password, parameter.national_id, parameter.birthdate, parameter.gender, parameter.type, db);
                if (result == permission_denied) {
                    fprintf(output, "%d#permission-denied\n", command_id);
                }
                else {
                    fprintf(output, "%d#success\n", command_id);
                }
            }
        }
        
        
        
        
    }
    
    
}


