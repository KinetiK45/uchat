#include "../inc/uchat.h"

t_userinfo get_userinfo_by_id(int user_id){
    t_userinfo result;
    result.username = NULL;
    result.profile_img = NULL;
    
    sqlite3 *db = open_db();
    
    sqlite3_stmt *stmt = NULL;
    
    char *command = "SELECT USERNAME, PROFILE_IMG FROM users WHERE USER_ID = ?1";
    
    int rv = sqlite3_prepare_v2(db, command, -1, &stmt, NULL);
    
    sqlite3_bind_int(stmt, 1, user_id);
    
    if (rv != SQLITE_OK){
        sqlite3_close(db);
        return result;
    }
    
    rv = sqlite3_step(stmt);
    if (rv != SQLITE_ROW){
        sqlite3_close(db);
        return result;
    }
    
    result.username = mx_strdup((char*) sqlite3_column_text(stmt, 0));
    result.profile_img = mx_strdup((char*) sqlite3_column_text(stmt, 1));

    sqlite3_finalize(stmt);

    sqlite3_close(db);
    
    result.user_id = user_id;
    return result;
}
