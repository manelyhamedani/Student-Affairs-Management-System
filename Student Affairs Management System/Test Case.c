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
#include <ctype.h>
#include "Student.h"


#define true                    1
#define max_size_command        30
#define max_size_parameter      500
#define max_size_parameter_name 30



char parameter_name[max_size_parameter_name];
char parameter_line[max_size_parameter];
char symbol;

int get_login_parameter(FILE *input, login_parameter *parameter) {
    fgets(parameter_line, max_size_parameter, input);
    if (sscanf(parameter_line, "%[^:]%c%[^|]%c%[^:]%c%s", parameter_name, &symbol, parameter->username, &symbol, parameter_name, &symbol, parameter->password) != 7) {
        return invalid;
    }
    return success;
}

int get_logout_parameter(FILE *input, logout_parameter *parameter) {
    fgets(parameter_line, max_size_parameter, input);
    if (sscanf(parameter_line, "%[^:]%c%s", parameter_name, &symbol, parameter->username) != 3) {
        return invalid;
    }
    return success;
}

int get_register_parameter(FILE *input, register_parameter *parameter) {
    fgets(parameter_line, max_size_parameter, input);
    if (sscanf(parameter_line, "%[^:]%c%[^|]%c%[^:]%c%[^|]%c%[^:]%c%[^|]%c%[^:]%c%[^|]%c%[^:]%c%[^|]%c%[^:]%c%[^|]%c%[^:]%c%[^|]%c%[^:]%c%s", parameter_name, &symbol, parameter->name, &symbol, parameter_name, &symbol, parameter->family, &symbol, parameter_name, &symbol, parameter->user_id, &symbol, parameter_name, &symbol, parameter->password, &symbol, parameter_name, &symbol, parameter->national_id, &symbol, parameter_name, &symbol, parameter->birthdate, &symbol, parameter_name, &symbol, parameter->gender, &symbol, parameter_name, &symbol, parameter->type) != 31) {
        return invalid;
    }
    char *p = parameter->user_id;
    if (strcmp(parameter->type, "student") == 0) {
        while (*p != '\0') {
            if (isdigit(*p) == 0) {
                return invalid;
            }
            ++p;
        }
    }
    if (strcmp(parameter->gender, "male") && strcmp(parameter->gender, "female")) {
        return invalid;
    }
    if (strcmp(parameter->type, "admin") && strcmp(parameter->type, "student")) {
        return invalid;
    }
//    p = parameter->national_id;
//    while (*p != '\0') {
//        if (isdigit(*p) == 0) {
//            return invalid;
//        }
//        ++p;
//    }
//    for (int i = 0; i < 4; ++i) {
//        if (isdigit(parameter->birthdate[i]) == 0) {
//            return invalid;
//        }
//    }
//    if (parameter->birthdate[4] != '-') {
//        return invalid;
//    }
//    for (int i = 5; i < 7; ++i) {
//        if (isdigit(parameter->birthdate[i]) == 0) {
//            return invalid;
//        }
//    }
//    if (parameter->birthdate[7] != '-') {
//        return invalid;
//    }
//    for (int i = 8; i < 10; ++i) {
//        if (isdigit(parameter->birthdate[i]) == 0) {
//            return invalid;
//        }
//    }
//    if (parameter->birthdate[10] != '\0') {
//        return invalid;
//    }
    return success;
}

int get_approve_parameter(FILE *input, approve_parameter *parameter) {
    int rv = fscanf(input, "%[^:]%c%[^| || '\n' || '\r']%c", parameter_name, &symbol, parameter->user_id, &symbol);
    if (rv != 4) {
        if (feof(input)) {
            return end_of_file;
        }
        return invalid;
    }
    if (symbol == '\n' || symbol == '\r') {
        return end_of_line;
    }
    return success;
}

int get_change_pass_parameter(FILE *input, change_pass_parameter *parameter) {
    fgets(parameter_line, max_size_parameter, input);
    if (sscanf(parameter_line, "%[^:]%c%[^|]%c%[^:]%c%[^|]%c%[^:]%c%s", parameter_name, &symbol, parameter->user_id, &symbol, parameter_name, &symbol, parameter->old_pass, &symbol, parameter_name, &symbol, parameter->new_pass) != 11) {
        return invalid;
    }
    return success;
}

int get_change_student_pass_parameter(FILE *input, change_student_pass_parameter *parameter) {
    fgets(parameter_line, max_size_parameter, input);
    if (sscanf(parameter_line, "%[^:]%c%[^|]%c%[^:]%c%s", parameter_name, &symbol, parameter->user_id, &symbol, parameter_name, &symbol, parameter->new_pass) != 7) {
        return invalid;
    }
    return success;
}

int get_remove_student_parameter(FILE *input, remove_student_parameter *parameter) {
    fgets(parameter_line, max_size_parameter, input);
    if (sscanf(parameter_line, "%[^:]%c%s", parameter_name, &symbol, parameter->user_id) != 3) {
        return invalid;
    }
    return success;
}

int get_deactivate_parameter(FILE *input, deactivate_parameter *parameter) {
    fgets(parameter_line, max_size_parameter, input);
    if (sscanf(parameter_line, "%[^:]%c%s", parameter_name, &symbol, parameter->user_id) != 3) {
        return invalid;
    }
    return success;
}

int get_self_parameter(FILE *input, self_parameter *parameter) {
    fgets(parameter_line, max_size_parameter, input);
    if (sscanf(parameter_line, "%[^:]%c%[^|]%c%[^:]%c%[^|]%c%[^:]%c%[^|]%c%[^:]%c%[^|]%c%[^:]%c%[^|]%c%[^:]%c%[^|]%c%[^:]%c%[^-]%c%[^|]%c%[^:]%c%[^-]%c%s", parameter_name, &symbol, parameter->self_id, &symbol, parameter_name, &symbol, parameter->name, &symbol, parameter_name, &symbol, parameter->location, &symbol, parameter_name, &symbol, parameter->capacity, &symbol, parameter_name, &symbol, parameter->type, &symbol, parameter_name, &symbol, parameter->meal, &symbol, parameter_name, &symbol, parameter->lunch_time_start, &symbol, parameter->lunch_time_end, &symbol, parameter_name, &symbol, parameter->dinner_time_start, &symbol, parameter->dinner_time_end) != 35) {
        return invalid;
    }
    char condition[max_size_parameter];
    sprintf(condition, "where self_id = %s", parameter->self_id);
    if (is_exists("SELF", condition) == 1) {
        return invalid;
    }
    int lunch_start_hour, lunch_start_minute, dinner_start_hour, dinner_start_minute, lunch_end_hour, lunch_end_minute, dinner_end_hour, dinner_end_minute;
    
    lunch_start_hour = (parameter->lunch_time_start[0] - '0') * 10 + (parameter->lunch_time_start[1] - '0');
    lunch_start_minute = (parameter->lunch_time_start[2] - '0') * 10 + (parameter->lunch_time_start[3] - '0');
    
    lunch_end_hour = (parameter->lunch_time_end[0] - '0') * 10 + (parameter->lunch_time_end[1] - '0');
    lunch_end_minute = (parameter->lunch_time_end[2] - '0') * 10 + (parameter->lunch_time_end[3] - '0');
    
    dinner_start_hour = (parameter->dinner_time_start[0] - '0') * 10 + (parameter->dinner_time_start[1] - '0');
    dinner_start_minute = (parameter->dinner_time_start[2] - '0') * 10 + (parameter->dinner_time_start[3] - '0');
    
    dinner_end_hour = (parameter->dinner_time_end[0] - '0') * 10 + (parameter->dinner_time_end[1] - '0');
    dinner_end_minute = (parameter->dinner_time_end[2] - '0') * 10 + (parameter->dinner_time_end[3] - '0');
    
    if (strcmp(parameter->meal, "lunch") == 0) {
        if (dinner_start_hour != 0 || dinner_start_minute != 0 || dinner_end_hour != 0 || dinner_end_minute != 0) {
            return invalid;
        }
        if (!(lunch_start_hour >= 0 && lunch_start_hour < 24)) {
            return invalid;
        }
        if (!(lunch_start_minute >= 0 && lunch_start_minute < 60)) {
            return invalid;
        }
        if (!(lunch_end_hour >= 0 && lunch_end_hour < 24)) {
            return invalid;
        }
        if (!(lunch_end_minute >= 0 && lunch_end_minute < 60)) {
            return invalid;
        }
    }
    else if (strcmp(parameter->meal, "dinner") == 0) {
        if (lunch_start_hour != 0 || lunch_start_minute != 0 || lunch_end_hour != 0 || lunch_end_minute != 0) {
            return invalid;
        }
        if (!(dinner_start_hour >= 0 && dinner_start_hour < 24)) {
            return invalid;
        }
        if (!(dinner_start_minute >= 0 && dinner_start_minute < 60)) {
            return invalid;
        }
        if (!(dinner_end_hour >= 0 && dinner_end_hour < 24)) {
            return invalid;
        }
        if (!(dinner_end_minute >= 0 && dinner_end_minute < 60)) {
            return invalid;
        }
    }
    else if (strcmp(parameter->meal, "both") == 0) {
        if (!(dinner_start_hour >= 0 && dinner_start_hour < 24)) {
            return invalid;
        }
        if (!(dinner_start_minute >= 0 && dinner_start_minute < 60)) {
            return invalid;
        }
        if (!(dinner_end_hour >= 0 && dinner_end_hour < 24)) {
            return invalid;
        }
        if (!(dinner_end_minute >= 0 && dinner_end_minute < 60)) {
            return invalid;
        }
        if (!(lunch_start_hour >= 0 && lunch_start_hour < 24)) {
            return invalid;
        }
        if (!(lunch_start_minute >= 0 && lunch_start_minute < 60)) {
            return invalid;
        }
        if (!(lunch_end_hour >= 0 && lunch_end_hour < 24)) {
            return invalid;
        }
        if (!(lunch_end_minute >= 0 && lunch_end_minute < 60)) {
            return invalid;
        }
        
    }
    else {
        return invalid;
    }
    if (strcmp(parameter->type, "boyish") && strcmp(parameter->type, "girlish")) {
        return invalid;
    }
    return success;
}

int get_food_parameter(FILE *input, food_parameter *parameter) {
    fgets(parameter_line, max_size_parameter, input);
    if (sscanf(parameter_line, "%[^:]%c%[^|]%c%[^:]%c%[^|]%c%[^:]%c%[^|]%c%[^:]%c%s", parameter_name, &symbol, parameter->food_id, &symbol, parameter_name, &symbol, parameter->name, &symbol, parameter_name, &symbol, parameter->type, &symbol, parameter_name, &symbol, parameter->price) != 15) {
        return invalid;
    }
    if (strcmp(parameter->type, "food") && strcmp(parameter->type, "dessert")) {
        return invalid;
    }
    char condition[max_size_parameter];
    sprintf(condition, "where food_id = %s", parameter->food_id);
    if (is_exists("FOOD", condition) == 1) {
        return invalid;
    }
    return success;
}

int get_meal_plan_parameter(FILE *input, meal_plan_parameter *parameter) {
    fgets(parameter_line, max_size_parameter, input);
    if (sscanf(parameter_line, "%[^:]%c%[^|]%c%[^:]%c%[^|]%c%[^:]%c%[^|]%c%[^:]%c%[^|]%c%[^:]%c%s", parameter_name, &symbol, parameter->self_id, &symbol, parameter_name, &symbol, parameter->date, &symbol, parameter_name, &symbol, parameter->type, &symbol, parameter_name, &symbol, parameter->food_id, &symbol, parameter_name, &symbol, parameter->count) != 19) {
        return invalid;
    }
    if (strcmp(parameter->type, "lunch") && strcmp(parameter->type, "dinner") && strcmp(parameter->type, "both")) {
        return invalid;
    }
    return success;
}

int get_charge_student_account(FILE *input, charge_student_account_parameter *parameter) {
    fgets(parameter_line, max_size_parameter, input);
    if (sscanf(parameter_line, "%[^:]%c%[^|]%c%[^:]%c%s", parameter_name, &symbol, parameter->user_id, &symbol, parameter_name, &symbol, parameter->amount) != 7) {
        return invalid;
    }
    return success;
}

int get_news_parameter(FILE *input, news_parameter *parameter) {
    fgets(parameter_line, max_size_parameter, input);
    if (sscanf(parameter_line, "%[^:]%c%[^|]%c%[^:]%c%[^|]%c%[^:]%c%s", parameter_name, &symbol, parameter->title, &symbol, parameter_name, &symbol, parameter->content, &symbol, parameter_name, &symbol, parameter->end_date) != 11) {
        return invalid;
    }
    return success;
}

int get_poll_parameter(FILE *input, poll_parameter *parameter) {
    fgets(parameter_line, max_size_parameter, input);
    if (sscanf(parameter_line, "%[^:]%c%[^|]%c%[^:]%c%[^|]%c%[^:]%c%[^|]%c%[^:]%c%[^|]%c%[^:]%c%[^|]%c%[^:]%c%s", parameter_name, &symbol, parameter->question, &symbol, parameter_name, &symbol, parameter->option1, &symbol, parameter_name, &symbol, parameter->option2, &symbol, parameter_name, &symbol, parameter->option3, &symbol, parameter_name, &symbol, parameter->option4, &symbol, parameter_name, &symbol, parameter->end_date) != 23) {
        return invalid;
    }
    return success;
}

int get_reserve_parameter(FILE *input, reserve_parameter *parameter) {
    fgets(parameter_line, max_size_parameter, input);
    if (sscanf(parameter_line, "%[^:]%c%[^|]%c%[^:]%c%[^|]%c%[^:]%c%[^|]%c%[^:]%c%s", parameter_name, &symbol, parameter->self_id, &symbol, parameter_name, &symbol, parameter->date, &symbol, parameter_name, &symbol, parameter->meal, &symbol, parameter_name, &symbol, parameter->food_id) != 15) {
        return invalid;
    }
    if (strcmp(parameter->meal, "lunch") && strcmp(parameter->meal, "dinner")) {
        return invalid;
    }
    return success;
}

int get_take_food_parameter(FILE *input, take_food_parameter *parameter) {
    fgets(parameter_line, max_size_parameter, input);
    if (sscanf(parameter_line, "%[^:]%c%[^|]%c%[^:]%c%[^|]%c%[^:]%c%s", parameter_name, &symbol, parameter->self_id, &symbol, parameter_name, &symbol, parameter->date, &symbol, parameter_name, &symbol, parameter->meal) != 11) {
        return invalid;
    }
    if (strcmp(parameter->meal, "lunch") && strcmp(parameter->meal, "dinner")) {
        return invalid;
    }
    return success;
}

int get_charge_account_parameter(FILE *input, charge_account_parameter *parameter) {
    fgets(parameter_line, max_size_parameter, input);
    if (sscanf(parameter_line, "%[^:]%c%[^|]%c%[^:]%c%[^|]%c%[^:]%c%s", parameter_name, &symbol, parameter->amount, &symbol, parameter_name, &symbol, parameter->card_number, &symbol, parameter_name, &symbol, parameter->pass_code) != 11) {
        return invalid;
    }
    return success;
}

int get_send_charge_parameter(FILE *input, send_charge_parameter *parameter) {
    fgets(parameter_line, max_size_parameter, input);
    if (sscanf(parameter_line, "%[^:]%c%[^|]%c%[^:]%c%[^|]%c%[^:]%c%s", parameter_name, &symbol, parameter->user_id, &symbol, parameter_name, &symbol, parameter->amount, &symbol, parameter_name, &symbol, parameter->name) != 11) {
        return invalid;
    }
    return success;
}

int get_cancel_reserve_parameter(FILE *input, cancel_reserve_parameter *parameter) {
    fgets(parameter_line, max_size_parameter, input);
    if (sscanf(parameter_line, "%[^:]%c%[^|]%c%[^:]%c%s", parameter_name, &symbol, parameter->date, &symbol, parameter_name, &symbol, parameter->meal) != 7) {
        return invalid;
    }
    if (strcmp(parameter->meal, "lunch") && strcmp(parameter->meal, "dinner")) {
        return invalid;
    }
    return success;
}

int get_daily_reserve_parameter(FILE *input, daily_reserve_parameter *parameter) {
    fgets(parameter_line, max_size_parameter, input);
    if (sscanf(parameter_line, "%[^:]%c%[^|]%c%[^:]%c%s", parameter_name, &symbol, parameter->self_id, &symbol, parameter_name, &symbol, parameter->food_id) != 7) {
        return invalid;
    }
    return success;
}

int get_agent_parameter(FILE *input, agent_parameter *parameter) {
    fgets(parameter_line, max_size_parameter, input);
    if (sscanf(parameter_line, "%[^:]%c%[^|]%c%[^:]%c%[^|]%c%[^:]%c%s", parameter_name, &symbol, parameter->date, &symbol, parameter_name, &symbol, parameter->meal, &symbol, parameter_name, &symbol, parameter->user_id) != 11) {
        return invalid;
    }
    return success;
}

int get_change_self_parameter(FILE *input, change_self_parameter *parameter) {
    fgets(parameter_line, max_size_parameter, input);
    if (sscanf(parameter_line, "%[^:]%c%[^|]%c%[^:]%c%[^|]%c%[^:]%c%s", parameter_name, &symbol, parameter->date, &symbol, parameter_name, &symbol, parameter->meal, &symbol, parameter_name, &symbol, parameter->new_self) != 11) {
        return invalid;
    }
    if (strcmp(parameter->meal, "lunch") && strcmp(parameter->meal, "dinner")) {
        return invalid;
    }
    return success;
}

int get_datetime_parameter(FILE *input, datetime_parameter *parameter) {
    fgets(parameter_line, max_size_parameter, input);
    if (sscanf(parameter_line, "%[^:]%c%[^|]%c%[^:]%c%s", parameter_name, &symbol, parameter->date, &symbol, parameter_name, &symbol, parameter->time) != 7) {
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
        fscanf(input, "%d#%[^# || '\n']%c", &command_id, command, &separator);
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
                result = user_login(parameter.username, parameter.password);
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
                else if (result == permission_denied) {
                    fprintf(output, "%d#permission-denied\n", command_id);
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
                result = user_register(parameter.name, parameter.family, parameter.user_id, parameter.password, parameter.national_id, parameter.birthdate, parameter.gender, parameter.type);
                if (result == permission_denied) {
                    fprintf(output, "%d#permission-denied\n", command_id);
                }
                else {
                    fprintf(output, "%d#success\n", command_id);
                }
            }
            if (current_user.user_type == admin) {
                approve(parameter.user_id);
            }
            continue;
        }
        
        if (strcmp(command, "approve") == 0) {
            fprintf(output, "%d#", command_id);
            char garbage[max_size_parameter];
            if (current_user.user_type == student) {
                fprintf(output, "permission-denied\n");
                fgets(garbage, max_size_parameter, input);
                continue;
            }
            approve_parameter parameter;
            while (true) {
                result = get_approve_parameter(input, &parameter);
                if (result == invalid) {
                    fprintf(output, "invalid\n");
                    fgets(garbage, max_size_parameter, input);
                    break;
                }
                else if (result == end_of_line || result == end_of_file) {
                    result = approve(parameter.user_id);
                    if (result == not_found) {
                        fprintf(output, "not-found\n");
                    }
                    else if (result == permission_denied) {
                        fprintf(output, "permission-denied\n");
                    }
                    else {
                        fprintf(output, "success\n");
                    }
                    break;
                }
                result = approve(parameter.user_id);
                if (result == not_found) {
                    fprintf(output, "not-found\n");
                    break;
                }
                else if (result == permission_denied) {
                    fprintf(output, "permission-denied\n");
                    break;
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
        
        if (strcmp(command, "change-student-pass") == 0) {
            change_student_pass_parameter parameter;
            result = get_change_student_pass_parameter(input, &parameter);
            if (result == invalid) {
                fprintf(output, "%d#invalid\n", command_id);
            }
            else {
                result = change_student_pass(parameter.user_id, parameter.new_pass);
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
        
        if (strcmp(command, "remove-student") == 0) {
            remove_student_parameter parameter;
            result = get_remove_student_parameter(input, &parameter);
            if (result == invalid) {
                fprintf(output, "%d#invalid\n", command_id);
            }
            else {
                result = remove_student(parameter.user_id);
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
        
        if (strcmp(command, "deactivate") == 0) {
            deactivate_parameter parameter;
            result = get_deactivate_parameter(input, &parameter);
            if (result == invalid) {
                fprintf(output, "%d#invalid\n", command_id);
            }
            else {
                result = deactivate(parameter.user_id);
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
        
        if (strcmp(command, "define-self") == 0) {
            self_parameter parameter;
            result = get_self_parameter(input, &parameter);
            if (result == invalid) {
                fprintf(output, "%d#invalid\n", command_id);
            }
            else {
                result = define_self(parameter.self_id, parameter.name, parameter.location, parameter.capacity, parameter.type, parameter.meal, parameter.lunch_time_start, parameter.lunch_time_end, parameter.dinner_time_start, parameter.dinner_time_end);
                if (result == permission_denied) {
                    fprintf(output, "%d#permission-denied\n", command_id);
                }
                else {
                    fprintf(output, "%d#success\n", command_id);
                }
            }
            continue;
        }
        
        if (strcmp(command, "define-food") == 0) {
            food_parameter parameter;
            result = get_food_parameter(input, &parameter);
            if (result == invalid) {
                fprintf(output, "%d#invalid\n", command_id);
            }
            else {
                result = define_food(parameter.food_id, parameter.name, parameter.type, parameter.price);
                if (result == permission_denied) {
                    fprintf(output, "%d#permission-denied\n", command_id);
                }
                else {
                    fprintf(output, "%d#success\n", command_id);
                }
            }
            continue;
        }
        
        if (strcmp(command, "define-meal-plan") == 0) {
            meal_plan_parameter parameter;
            result = get_meal_plan_parameter(input, &parameter);
            if (result == invalid) {
                fprintf(output, "%d#invalid\n", command_id);
            }
            else {
                result = define_meal_plan(parameter.self_id, parameter.date, parameter.type, parameter.food_id, parameter.count);
                if (result == permission_denied) {
                    fprintf(output, "%d#permission-denied\n", command_id);
                }
                else if (result == not_found) {
                    fprintf(output, "%d#not-found\n", command_id);
                }
                else {
                    fprintf(output, "%d#success\n", command_id);
                }
            }
            continue;
        }
        
        if (strcmp(command, "charge-student-account") == 0) {
            charge_student_account_parameter parameter;
            result = get_charge_student_account(input, &parameter);
            if (result == invalid) {
                fprintf(output, "%d#invalid\n", command_id);
            }
            else {
                result = charge_student_account(parameter.user_id, parameter.amount);
                if (result == permission_denied) {
                    fprintf(output, "%d#permission-denied\n", command_id);
                }
                else if (result == not_found) {
                    fprintf(output, "%d#not-found\n", command_id);
                }
                else {
                    fprintf(output, "%d#success\n", command_id);
                }
            }
            continue;
        }
        
        if (strcmp(command, "add-news") == 0) {
            news_parameter parameter;
            result = get_news_parameter(input, &parameter);
            if (result == invalid) {
                fprintf(output, "%d#invalid\n", command_id);
            }
            else {
                result = add_news(parameter.title, parameter.content, parameter.end_date);
                if (result == permission_denied) {
                    fprintf(output, "%d#permission-denied\n", command_id);
                }
                else {
                    fprintf(output, "%d#success\n", command_id);
                }
            }
            continue;
        }
        
        if (strcmp(command, "add-poll") == 0) {
            poll_parameter parameter;
            result = get_poll_parameter(input, &parameter);
            if (result == invalid) {
                fprintf(output, "%d#invalid\n", command_id);
            }
            else {
                result = add_poll(parameter.question, parameter.option1, parameter.option2, parameter.option3, parameter.option4, parameter.end_date);
                if (result == permission_denied) {
                    fprintf(output, "%d#permission-denied\n", command_id);
                }
                else {
                    fprintf(output, "%d#success\n", command_id);
                }
            }
            continue;
        }
        
        if (strcmp(command, "reserve") == 0) {
            reserve_parameter parameter;
            result = get_reserve_parameter(input, &parameter);
            if (result == invalid) {
                fprintf(output, "%d#invalid\n", command_id);
            }
            else {
                result = reserve(parameter.self_id, parameter.date, parameter.meal, parameter.food_id, 1, 0);
                if (result == permission_denied) {
                    fprintf(output, "%d#permission-denied\n", command_id);
                }
                else if (result == not_found) {
                    fprintf(output, "%d#not-found\n", command_id);
                }
                else if (result == insufficient_money) {
                    fprintf(output, "%d#permission-denied\n", command_id);
                }
                else {
                    fprintf(output, "%d#success\n", command_id);
                }
            }
            continue;
        }
        
        if (strcmp(command, "take-food") == 0) {
            take_food_parameter parameter;
            result = get_take_food_parameter(input, &parameter);
            if (result == invalid) {
                fprintf(output, "%d#invalid\n", command_id);
            }
            else {
                result = take_food(parameter.self_id, parameter.date, parameter.meal);
                if (result == permission_denied) {
                    fprintf(output, "%d#permission-denied\n", command_id);
                }
                else if (result == not_found) {
                    fprintf(output, "%d#not-found\n", command_id);
                }
                else {
                    fprintf(output, "%d#success\n", command_id);
                }
            }
            continue;
        }
        
        if (strcmp(command, "charge-account") == 0) {
            charge_account_parameter parameter;
            result = get_charge_account_parameter(input, &parameter);
            if (result == invalid) {
                fprintf(output, "%d#invalid\n", command_id);
            }
            else {
                result = charge_account(parameter.amount);
                if (result == permission_denied) {
                    fprintf(output, "%d#permission-denied\n", command_id);
                }
                else if (result == not_found) {
                    fprintf(output, "%d#not-found\n", command_id);
                }
                else {
                    fprintf(output, "%d#success\n", command_id);
                }
            }
            continue;
        }
        
        if (strcmp(command, "send-charge") == 0) {
            send_charge_parameter parameter;
            result = get_send_charge_parameter(input, &parameter);
            if (result == invalid) {
                fprintf(output, "%d#invalid\n", command_id);
            }
            else {
                result = send_charge(parameter.user_id, parameter.amount, parameter.name);
                if (result == permission_denied) {
                    fprintf(output, "%d#permission-denied\n", command_id);
                }
                else if (result == not_found) {
                    fprintf(output, "%d#not-found\n", command_id);
                }
                else {
                    fprintf(output, "%d#success\n", command_id);
                }
            }
            continue;
        }
        
        if (strcmp(command, "cancel-reserve") == 0) {
            cancel_reserve_parameter parameter;
            result = get_cancel_reserve_parameter(input, &parameter);
            if (result == invalid) {
                fprintf(output, "%d#invalid\n", command_id);
            }
            else {
                result = cancel_reserve(parameter.date, parameter.meal);
                if (result == permission_denied) {
                    fprintf(output, "%d#permission-denied\n", command_id);
                }
                else if (result == not_found) {
                    fprintf(output, "%d#not-found\n", command_id);
                }
                else {
                    fprintf(output, "%d#success\n", command_id);
                }
            }
            continue;
        }
        
        if (strcmp(command, "daily-reserve") == 0) {
            daily_reserve_parameter parameter;
            result = get_daily_reserve_parameter(input, &parameter);
            if (result == invalid) {
                fprintf(output, "%d#invalid\n", command_id);
            }
            else {
                result = daily_reserve(parameter.self_id, parameter.food_id);
                if (result == permission_denied) {
                    fprintf(output, "%d#permission-denied\n", command_id);
                }
                else if (result == not_found) {
                    fprintf(output, "%d#not-found\n", command_id);
                }
                else if (result == insufficient_money) {
                    fprintf(output, "%d#permission-denied\n", command_id);
                }
                else {
                    fprintf(output, "%d#success\n", command_id);
                }
            }
            continue;
        }
        
        if (strcmp(command, "define-agent") == 0) {
            agent_parameter parameter;
            result = get_agent_parameter(input, &parameter);
            if (result == invalid) {
                fprintf(output, "%d#invalid\n", command_id);
            }
            else {
                result = define_agent(parameter.date, parameter.meal, parameter.user_id);
                if (result == permission_denied) {
                    fprintf(output, "%d#permission-denied\n", command_id);
                }
                else if (result == not_found) {
                    fprintf(output, "%d#not-found\n", command_id);
                }
                else {
                    fprintf(output, "%d#success\n", command_id);
                }
            }
            continue;
        }
        
        if (strcmp(command, "change-self") == 0) {
            change_self_parameter parameter;
            result = get_change_self_parameter(input, &parameter);
            if (result == invalid) {
                fprintf(output, "%d#invalid\n", command_id);
            }
            else {
                result = change_self(parameter.date, parameter.meal, parameter.new_self);
                if (result == permission_denied) {
                    fprintf(output, "%d#permission-denied\n", command_id);
                }
                else if (result == not_found) {
                    fprintf(output, "%d#not-found\n", command_id);
                }
                else {
                    fprintf(output, "%d#success\n", command_id);
                }
            }
            continue;
        }
        
        if (strcmp(command, "check-news") == 0) {
            result = check_news(1);
            if (result == permission_denied) {
                fprintf(output, "%d#permission-denied\n", command_id);
            }
            else if (result == not_found) {
                fprintf(output, "%d#not-found\n", command_id);
            }
            else {
                fprintf(output, "%d#success\n", command_id);
            }
            continue;
        }
        
        if (strcmp(command, "vote") == 0) {
            result = vote(0, 0, 1);
            if (result == permission_denied) {
                fprintf(output, "%d#permission-denied\n", command_id);
            }
            else if (result == not_found) {
                fprintf(output, "%d#not-found\n", command_id);
            }
            else {
                fprintf(output, "%d#success\n", command_id);
            }
            continue;
        }
        
        
        if (strcmp(command, "change-datetime") == 0) {
            datetime_parameter parameter;
            result = get_datetime_parameter(input, &parameter);
            if (result == invalid) {
                fprintf(output, "%d#invalid\n", command_id);
            }
            else {
                change_datetime(parameter.date, parameter.time);
                fprintf(output, "%d#success\n", command_id);
            }
        }
        
        else {
            char garbage[max_size];
            fgets(garbage, max_size, input);
            fprintf(output, "%d#invalid\n", command_id);
        }
        
    }
    
    
}


