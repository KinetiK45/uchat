#include "../inc/uchat.h"

void delete_chat_history_by_id(int chat_id){
    sqlite3 *db = open_db();
    int rc;
    
    sqlite3_stmt *stmt;
    char *sql = "DELETE FROM users_msgs WHERE CHAT_ID = ?1 AND TIME != 'first msg';";
        
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return;
    }

    sqlite3_bind_int(stmt, 1, chat_id);

    rc = sqlite3_step(stmt);

    if (rc != SQLITE_DONE) {
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
}
