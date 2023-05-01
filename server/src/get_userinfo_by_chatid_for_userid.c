#include "../inc/uchat.h"

int get_userid_by_chatid_for_request_user(int chat_id, int request_for_user_id){
    sqlite3 *db = open_db();
    
    sqlite3_stmt *stmt = NULL;
    int id;
    
    char *command = "SELECT * FROM users_msgs WHERE CHAT_ID = ?1 AND SENDER_ID != ?2 LIMIT 1";
    
    int rv = sqlite3_prepare_v2(db, command, -1, &stmt, NULL);

    sqlite3_bind_int(stmt, 1, chat_id);
    sqlite3_bind_int(stmt, 2, request_for_user_id);

    if (rv != SQLITE_OK){
        sqlite3_close(db);
        return -1;
    }

    rv = sqlite3_step(stmt);
    if (rv != SQLITE_ROW){
        sqlite3_close(db);
        return -1;
    }

    id = sqlite3_column_int(stmt, 2);

    sqlite3_finalize(stmt);

    sqlite3_close(db);
    return id;
}

t_userinfo get_userinfo_by_chatid_for_userid(int chat_id, int request_for_user_id){
    t_userinfo result;
    result.username = NULL;
    result.profile_img = NULL;
    int user_id = get_userid_by_chatid_for_request_user(chat_id, request_for_user_id);
    result.user_id = user_id;
    
    sqlite3 *db = open_db();
    
    sqlite3_stmt *stmt = NULL;
    
    char *command = "SELECT PROFILE_IMG, USERNAME FROM users WHERE USER_ID = ?1";
    
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
    
    result.username = mx_strdup((char*) sqlite3_column_text(stmt, 1));
    result.profile_img = mx_strdup((char*) sqlite3_column_text(stmt, 0));

    sqlite3_finalize(stmt);

    sqlite3_close(db);
    
    return result;
}
