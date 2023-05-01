#include "../inc/uchat.h"

void initialize_new_chat(int user_1, int user_2){
    sqlite3 *db = open_db();
    sqlite3_stmt *stmt;
    
    int max_chat_id = 0;
    
    char *sql = "SELECT MAX(CHAT_ID) FROM users_msgs;";
    
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

    if (rc == SQLITE_OK) {
        rc = sqlite3_step(stmt);
        if (rc == SQLITE_ROW) {
            max_chat_id = sqlite3_column_int(stmt, 0);
        }
    }
    
    sqlite3_finalize(stmt);
    
    sqlite3_close(db);
    
    register_new_msg_in_db(max_chat_id + 1, user_1, "first msg", "first msg");
    register_new_msg_in_db(max_chat_id + 1, user_2, "first msg", "first msg");
}
