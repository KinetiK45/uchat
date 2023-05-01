#include "../inc/uchat.h"

int get_msg_size(int chat_id){
    sqlite3 *db = open_db();
    sqlite3_stmt *stmt;
    int count = 0;
    
    char *sql = "SELECT COUNT(*) FROM users_msgs WHERE CHAT_ID = ?1;";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    sqlite3_bind_int(stmt, 1, chat_id);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        count = sqlite3_column_int(stmt, 0);
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return count;
}

t_chat_msg* get_msg_history(int user_1, int user_2, int chat_id){
    if (chat_id < 0){
        chat_id = get_chatid_between_users(user_1, user_2);
    }
    int count = get_msg_size(chat_id);
    if (count <= 2) return NULL;
    
    //--------------------------
    
    sqlite3 *db = open_db();

    t_chat_msg *history = malloc(sizeof(t_chat_msg) * (count + 1));
    
    for(int i = 0; i < count + 1; i++){
        history[i].msg_id = -1;
        history[i].sender_id = -1;
        history[i].time = NULL;
        history[i].msg_text = NULL;
    }
        
    sqlite3_stmt *stmt;

    char *sql = "SELECT * FROM users_msgs WHERE CHAT_ID = ?1;";
            
    int rv = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    
    sqlite3_bind_int(stmt, 1, chat_id);

    if (rv != SQLITE_OK){
        free(history);
        sqlite3_close(db);
        return NULL;
    }

    rv = sqlite3_step(stmt);
    
    if (rv != SQLITE_ROW){
        free(history);
        sqlite3_close(db);
        return NULL;
    }
    for(int i = 0; i < count && rv == SQLITE_ROW; i++){
        history[i].msg_id = sqlite3_column_int(stmt, 0);
        history[i].sender_id = sqlite3_column_int(stmt, 2);
        history[i].time = mx_strdup((char*) sqlite3_column_text(stmt, 3));
        history[i].msg_text = mx_strdup((char*) sqlite3_column_text(stmt, 4));
        rv = sqlite3_step(stmt);
    }

    sqlite3_finalize(stmt);
    
    sqlite3_close(db);
    return history;
}
