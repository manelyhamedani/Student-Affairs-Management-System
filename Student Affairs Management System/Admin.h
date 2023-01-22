//
//  Admin.h
//  Student Affairs Management System
//
//  Created by Manely Hamedani on 1/19/23.
//

#ifndef Admin_h
#define Admin_h
#include <sqlite3.h>

int approve(sqlite3 *db, const char *user_id);

#endif /* Admin_h */
