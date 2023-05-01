#include "../inc/uchat.h"

int get_id_by_login(char *login){
    sqlite3 *db = open_db();
    
    sqlite3_stmt *stmt = NULL;
    int id;
    
    char *command = "SELECT * FROM users WHERE LOGIN = ?1";
    
    int rv = sqlite3_prepare_v2(db, command, -1, &stmt, NULL);

    sqlite3_bind_text(stmt, 1, login, -1, SQLITE_STATIC);

    if (rv != SQLITE_OK){
        sqlite3_close(db);
        return -1;
    }

    rv = sqlite3_step(stmt);
    if (rv != SQLITE_ROW){
        sqlite3_close(db);
        return -1;
    }

    id = sqlite3_column_int(stmt, 0);

    sqlite3_finalize(stmt);

    sqlite3_close(db);
    return id;
}
