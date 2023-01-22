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
#include "Admin.h"


#define true                    1
#define max_size_command        30
#define max_size_parameter      100
#define max_size_parameter_name 30

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

typedef struct {
    char user_id[max_size_parameter];
} approve_parameter;

typedef struct {
    char user_id[max_size_parameter];
    char old_pass[max_size_parameter];
    char new_pass[max_size_parameter];
} change_pass_parameter;


int get_login_parameter(FILE *input, login_parameter *parameter) {
    char parameter_name[max_size_parameter_name];
    char colon;
    char separator;
    char end;
    fscanf(input, "%[^:]%c", parameter_name, &colon);
    if (strcmp(parameter_name, "user")) {
        return invalid;
    }
    fscanf(input, "%[^|]", parameter->username);
    fscanf(input, "%c%[^:]%c", &separator, parameter_name, &colon);
    if (strcmp(parameter_name, "password")) {
        return invalid;
    }
    fscanf(input, "%s%c", parameter->password, &end);
    if (feof(input)) {
        return eof;
    }
    if (end != '\n') {
        return invalid;
    }
    return success;
}

int get_logout_parameter(FILE *input, logout_parameter *parameter) {
    char parameter_name[max_size_parameter_name];
    char colon;
    char end;
    fscanf(input, "%[^:]%c", parameter_name, &colon);
    if (strcmp(parameter_name, "user")) {
        return invalid;
    }
    fscanf(input, "%s%c", parameter->username, &end);
    if (feof(input)) {
        return eof;
    }
    if (end != '\n') {
        return invalid;
    }
    return success;
}

int get_register_parameter(FILE *input, register_parameter *parameter) {
    char parameter_name[max_size_parameter_name];
    char colon;
    char separator;
    char end;
    fscanf(input, "%[^:]%c", parameter_name, &colon);
    if (strcmp(parameter_name, "name")) {
        return invalid;
    }
    fscanf(input, "%[^|]%c", parameter->name, &separator);
    fscanf(input, "%[^:]%c", parameter_name, &colon);
    if (strcmp(parameter_name, "family")) {
        return invalid;
    }
    fscanf(input, "%[^|]%c", parameter->family, &separator);
    fscanf(input, "%[^:]%c", parameter_name, &colon);
    if (strcmp(parameter_name, "user-id")) {
        return invalid;
    }
    fscanf(input, "%[^|]%c", parameter->user_id, &separator);
    fscanf(input, "%[^:]%c", parameter_name, &colon);
    if (strcmp(parameter_name, "password")) {
        return invalid;
    }
    fscanf(input, "%[^|]%c", parameter->password, &separator);
    fscanf(input, "%[^:]%c", parameter_name, &colon);
    if (strcmp(parameter_name, "national-id-code")) {
        return invalid;
    }
    fscanf(input, "%[^|]%c", parameter->national_id, &separator);
    fscanf(input, "%[^:]%c", parameter_name, &colon);
    if (strcmp(parameter_name, "birthdate")) {
        return invalid;
    }
    fscanf(input, "%[^|]%c", parameter->birthdate, &separator);
    fscanf(input, "%[^:]%c", parameter_name, &colon);
    if (strcmp(parameter_name, "gender")) {
        return invalid;
    }
    fscanf(input, "%[^|]%c", parameter->gender, &separator);
    fscanf(input, "%[^:]%c", parameter_name, &colon);
    if (strcmp(parameter_name, "type")) {
        return invalid;
    }
    fscanf(input, "%s%c", parameter->type, &end);
    if (feof(input)) {
        return eof;
    }
    if (end != '\n') {
        return invalid;
    }
    return success;
}

int get_approve_parameter(FILE *input, approve_parameter *parameter) {
    char parameter_name[max_size_parameter_name];
    char colon;
    char separator;
    fscanf(input, "%[^:]%c", parameter_name, &colon);
    if (strcmp(parameter_name, "user")) {
        return invalid;
    }
    fscanf(input, "%[^|]%c", parameter->user_id, &separator);
    if (feof(input)) {
        // delete '\n' from the end of the last parameter
        sscanf(parameter->user_id, "%s", parameter->user_id);
        return eof;
    }
    if (separator == '\n') {
        return eol;
    }
    else if (separator != '|') {
        return invalid;
    }
    return success;
}

int get_change_pass_parameter(FILE *input, change_pass_parameter *parameter) {
    char parameter_name[max_size_parameter_name];
    char colon;
    char separator;
    fscanf(input, "%[^:]%c", parameter_name, &colon);
    if (strcmp(parameter_name, "user")) {
        return invalid;
    }
    fscanf(input, "%[^|]%c", parameter->user_id, &separator);
    fscanf(input, "%[^:]%c", parameter_name, &colon);
    if (strcmp(parameter_name, "old-pass")) {
        return invalid;
    }
    fscanf(input, "%[^|]%c", parameter->old_pass, &separator);
    fscanf(input, "%[^:]%c", parameter_name, &colon);
    if (strcmp(parameter_name, "new-pass")) {
        return invalid;
    }
    fscanf(input, "%s", parameter->new_pass);
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
                else if (result == eof){
                    fprintf(output, "%d#success\n", command_id);
                    break;
                }
                else {
                    fprintf(output, "%d#success\n", command_id);
                }
            }
            continue;
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
                else if (result == eof){
                    fprintf(output, "%d#success\n", command_id);
                    break;
                }
                else {
                    fprintf(output, "%d#success\n", command_id);
                }
            }
            continue;
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
                else if (result == eof){
                    fprintf(output, "%d#success\n", command_id);
                    break;
                }
                else {
                    fprintf(output, "%d#success\n", command_id);
                }
            }
            if (current_user.user_type == admin) {
                approve(db, parameter.user_id);
            }
            continue;
        }
        
        if (strcmp(command, "approve") == 0) {
            fprintf(output, "%d#", command_id);
            if (current_user.user_type == student) {
                fprintf(output, "permission-denied\n");
                char garbage[max_size_parameter];
                fgets(garbage, max_size_parameter, input);
                continue;
            }
            approve_parameter parameter;
            while (true) {
                result = get_approve_parameter(input, &parameter);
                if (result == invalid) {
                    fprintf(output, "invalid");
                    if (feof(input)) {
                        fprintf(output, "\n");
                        break;
                    }
                    else {
                        fprintf(output, "|");
                    }
                }
                else if (result == eof) {
                    result = approve(db, parameter.user_id);
                    if (result == not_found) {
                        fprintf(output, "not-found\n");
                    }
                    else {
                        fprintf(output, "success\n");
                    }
                    break;
                }
                else {
                    result = approve(db, parameter.user_id);
                    if (result == not_found) {
                        fprintf(output, "not-found|");
                    }
                    else {
                        fprintf(output, "success|");
                    }
                }
            }
            continue;
        }
        
        if (strcmp(command, "change-pass") == 0) {
            change_pass_parameter parameter;
            result = get_change_pass_parameter(input, &parameter);
            if (result == invalid) {
                fprintf(output, "%d#invalid\n", command_id);
            }
            else {
                result = change_pass(parameter.user_id, parameter.old_pass, parameter.new_pass);
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
            continue;
        }
        
        
        
        
        
        
    }
    
    
}


