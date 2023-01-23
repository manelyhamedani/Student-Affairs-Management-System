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


#define true                    1
#define max_size_command        30
#define max_size_parameter      500
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

typedef struct {
    char user_id[max_size_parameter];
    char new_pass[max_size_parameter];
} change_student_pass_parameter;

typedef struct {
    char user_id[max_size_parameter];
} remove_student_parameter;

typedef struct {
    char user_id[max_size_parameter];
} deactivate_parameter;

typedef struct {
    char self_id[max_size_parameter];
    char name[max_size_parameter];
    char location[max_size_parameter];
    char capacity[max_size_parameter];
    char type[max_size_parameter];
    char meal[max_size_parameter];
    char lunch_time[max_size_parameter];
    char dinner_time[max_size_parameter];
} define_self_parameter;

typedef struct {
    char food_id[max_size_parameter];
    char name[max_size_parameter];
    char type[max_size_parameter];
    char price[max_size_parameter];
} define_food_parameter;


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
    int rv = fscanf(input, "%[^:]%c%[^| || '\n']%c", parameter_name, &symbol, parameter->user_id, &symbol);
    if (rv != 4) {
        if (feof(input)) {
            return end_of_file;
        }
        return invalid;
    }
    if (symbol == '\n') {
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

int get_define_self_parameter(FILE *input, define_self_parameter *parameter) {
    fgets(parameter_line, max_size_parameter, input);
    if (sscanf(parameter_line, "%[^:]%c%[^|]%c%[^:]%c%[^|]%c%[^:]%c%[^|]%c%[^:]%c%[^|]%c%[^:]%c%[^|]%c%[^:]%c%[^|]%c%[^:]%c%[^|]%c%[^:]%c%s", parameter_name, &symbol, parameter->self_id, &symbol, parameter_name, &symbol, parameter->name, &symbol, parameter_name, &symbol, parameter->location, &symbol, parameter_name, &symbol, parameter->capacity, &symbol, parameter_name, &symbol, parameter->type, &symbol, parameter_name, &symbol, parameter->meal, &symbol, parameter_name, &symbol, parameter->lunch_time, &symbol, parameter_name, &symbol, parameter->dinner_time) != 31) {
        return invalid;
    }
    int lunch_hour1, lunch_minute1, dinner_hour1, dinner_minute1, lunch_hour2, lunch_minute2, dinner_hour2, dinner_minute2;
    
    lunch_hour1 = (parameter->lunch_time[0] - '0') * 10 + (parameter->lunch_time[1] - '0');
    lunch_minute1 = (parameter->lunch_time[2] - '0') * 10 + (parameter->lunch_time[3] - '0');
    
    lunch_hour2 = (parameter->lunch_time[5] - '0') * 10 + (parameter->lunch_time[6] - '0');
    lunch_minute2 = (parameter->lunch_time[7] - '0') * 10 + (parameter->lunch_time[8] - '0');
    
    dinner_hour1 = (parameter->dinner_time[0] - '0') * 10 + (parameter->dinner_time[1] - '0');
    dinner_minute1 = (parameter->dinner_time[2] - '0') * 10 + (parameter->dinner_time[3] - '0');
    
    dinner_hour2 = (parameter->dinner_time[5] - '0') * 10 + (parameter->dinner_time[6] - '0');
    dinner_minute2 = (parameter->dinner_time[7] - '0') * 10 + (parameter->dinner_time[8] - '0');
    
    if (strcmp(parameter->meal, "lunch") == 0) {
        if (dinner_hour1 != 0 || dinner_minute1 != 0 || dinner_hour2 != 0 || dinner_minute2 != 0) {
            return invalid;
        }
        if (!(lunch_hour1 >= 0 && lunch_hour1 < 24)) {
            return invalid;
        }
        if (!(lunch_minute1 >= 0 && lunch_minute1 < 60)) {
            return invalid;
        }
        if (!(lunch_hour2 >= 0 && lunch_hour2 < 24)) {
            return invalid;
        }
        if (!(lunch_minute2 >= 0 && lunch_minute2 < 60)) {
            return invalid;
        }
    }
    else if (strcmp(parameter->meal, "dinner") == 0) {
        if (lunch_hour1 != 0 || lunch_minute1 != 0 || lunch_hour2 != 0 || lunch_minute2 != 0) {
            return invalid;
        }
        if (!(dinner_hour1 >= 0 && dinner_hour1 < 24)) {
            return invalid;
        }
        if (!(dinner_minute1 >= 0 && dinner_minute1 < 60)) {
            return invalid;
        }
        if (!(dinner_hour2 >= 0 && dinner_hour2 < 24)) {
            return invalid;
        }
        if (!(dinner_minute2 >= 0 && dinner_minute2 < 60)) {
            return invalid;
        }
    }
    else if (strcmp(parameter->meal, "both") == 0) {
        if (!(dinner_hour1 >= 0 && dinner_hour1 < 24)) {
            return invalid;
        }
        if (!(dinner_minute1 >= 0 && dinner_minute1 < 60)) {
            return invalid;
        }
        if (!(dinner_hour2 >= 0 && dinner_hour2 < 24)) {
            return invalid;
        }
        if (!(dinner_minute2 >= 0 && dinner_minute2 < 60)) {
            return invalid;
        }
        if (!(lunch_hour1 >= 0 && lunch_hour1 < 24)) {
            return invalid;
        }
        if (!(lunch_minute1 >= 0 && lunch_minute1 < 60)) {
            return invalid;
        }
        if (!(lunch_hour2 >= 0 && lunch_hour2 < 24)) {
            return invalid;
        }
        if (!(lunch_minute2 >= 0 && lunch_minute2 < 60)) {
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

int get_define_food_parameter(FILE *input, define_food_parameter *parameter) {
    fgets(parameter_line, max_size_parameter, input);
    if (sscanf(parameter_line, "%[^:]%c%[^|]%c%[^:]%c%[^|]%c%[^:]%c%[^|]%c%[^:]%c%s", parameter_name, &symbol, parameter->food_id, &symbol, parameter_name, &symbol, parameter->name, &symbol, parameter_name, &symbol, parameter->type, &symbol, parameter_name, &symbol, parameter->price) != 15) {
        return invalid;
    }
    if (strcmp(parameter->type, "food") && strcmp(parameter->type, "dessert")) {
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
                else if (result == end_of_line || result == end_of_file) {
                    result = approve(parameter.user_id);
                    if (result == not_found) {
                        fprintf(output, "not-found\n");
                    }
                    else {
                        fprintf(output, "success\n");
                    }
                    break;
                }
                else {
                    result = approve(parameter.user_id);
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
            define_self_parameter parameter;
            result = get_define_self_parameter(input, &parameter);
            if (result == invalid) {
                fprintf(output, "%d#invalid\n", command_id);
            }
            else {
                result = define_self(parameter.self_id, parameter.name, parameter.location, parameter.capacity, parameter.type, parameter.meal, parameter.lunch_time, parameter.dinner_time);
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
            define_food_parameter parameter;
            result = get_define_food_parameter(input, &parameter);
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
        
    }
    
    
}


