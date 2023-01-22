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
#endif /* Admin_h */
