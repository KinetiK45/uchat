#include "../inc/uchat.h"

void register_new_msg_in_db(int chat_id, int sender_id, char *time, char *text){
    sqlite3_stmt *stmt = NULL;
    
    sqlite3 *db = open_db();
    
    char *command = "INSERT INTO users_msgs (CHAT_ID, SENDER_ID, TIME, MSG_TEXT) "  \
              "VALUES (?1, ?2, ?3, ?4);";
    int rv = sqlite3_prepare_v2(db, command, -1, &stmt, NULL);
    
    if (rv == SQLITE_ERROR){
        sqlite3_close(db);
        return;
    }
    sqlite3_bind_int(stmt, 1, chat_id);
    sqlite3_bind_int(stmt, 2, sender_id);
    sqlite3_bind_text(stmt, 3, time, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, text, -1, SQLITE_STATIC);
    
    sqlite3_step(stmt);
    
    sqlite3_finalize(stmt);
        
    sqlite3_close(db);
}
