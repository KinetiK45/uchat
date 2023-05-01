#include "../inc/uchat.h"

int get_count_chats_by_user_id(int user_id){
    sqlite3 *db = open_db();
    sqlite3_stmt *stmt;
    int count = 0;

    char *sql = "SELECT COUNT(DISTINCT CHAT_ID) FROM users_msgs WHERE SENDER_ID = ?1";
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    sqlite3_bind_int(stmt, 1, user_id);
    
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        count = sqlite3_column_int(stmt, 0);
    }
    
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return count;
}

int *get_chats_for_user(int user_id){
    int size = get_count_chats_by_user_id(user_id);
    
    if (size == 0) return NULL;
    
    sqlite3 *db = open_db();
    sqlite3_stmt *stmt = NULL;

    char *command = "SELECT DISTINCT CHAT_ID FROM users_msgs " \
    "WHERE SENDER_ID = ?1;";
    
    int rv = sqlite3_prepare_v2(db, command, -1, &stmt, NULL);
    if (rv != SQLITE_OK) {
        sqlite3_close(db);
        return NULL;
    }

    sqlite3_bind_int(stmt, 1, user_id);
    
    int *result = malloc(sizeof(int) * (size + 1));
    for (int i = 0; i < size + 1; i++) {
        result[i] = -1;
    }
    
    for (int i = 0; i < size && sqlite3_step(stmt) == SQLITE_ROW; i++) {
        result[i] = sqlite3_column_int(stmt, 0);
    }
        
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return result;
}
