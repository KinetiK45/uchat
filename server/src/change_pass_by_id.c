#include "../inc/uchat.h"

bool change_pass_by_id(int acc_id, char* new_pass){
    sqlite3 *db = open_db();

    sqlite3_stmt *stmt = NULL;
            
    char *command = "UPDATE users SET PASSWORD = ?1 WHERE USER_ID = ?2;";
    
    int rv = sqlite3_prepare_v2(db, command, -1, &stmt, NULL);

    sqlite3_bind_text(stmt, 1, new_pass, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, acc_id);

    if (rv != SQLITE_OK){
        sqlite3_close(db);
        return false;
    }

    rv = sqlite3_step(stmt);

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return true;
}
