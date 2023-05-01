#include "../inc/uchat.h"

int get_id_by_log_pass(char *login, char *pass){
    sqlite3_stmt *stmt = NULL;
    sqlite3 *db = open_db();
    int id = -1;
    
    char *command = "SELECT * FROM users WHERE LOGIN = ?1 AND PASSWORD = ?2";
    
    int rv = sqlite3_prepare_v2(db, command, -1, &stmt, NULL);

    sqlite3_bind_text(stmt, 1, login, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, pass, -1, SQLITE_STATIC);

    if (rv != SQLITE_OK){
        sqlite3_close(db);
        return -1;
    }

    rv = sqlite3_step(stmt);
    if (rv != SQLITE_ROW)
        if (rv == SQLITE_ERROR){
            sqlite3_close(db);
            return -1;
        }

    id = sqlite3_column_int(stmt, 0);

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return id == 0 ? -1 : id;
}
