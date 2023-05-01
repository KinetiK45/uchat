#include "../inc/uchat.h"

void delete_msg(int msg_id){
    sqlite3 *db = open_db();
    int rc;
    
    sqlite3_stmt *stmt;
    char *sql = "DELETE FROM users_msgs WHERE MSG_ID = ?1;";
        
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return;
    }

    sqlite3_bind_int(stmt, 1, msg_id);

    rc = sqlite3_step(stmt);

    sqlite3_finalize(stmt);
    sqlite3_close(db);
}
