#include "../inc/uchat.h"

bool find_client_socket_in_db(int client_socket){
    sqlite3_stmt *stmt = NULL;
    sqlite3 *db = open_db();
    int result = -1;
    
    char *command = "SELECT * FROM users WHERE СLIENT_SOCKET = ?1";
    
    int rv = sqlite3_prepare_v2(db, command, -1, &stmt, NULL);

    sqlite3_bind_int(stmt, 1, client_socket);

    if (rv != SQLITE_OK){
        sqlite3_close(db);
        return false;
    }

    rv = sqlite3_step(stmt);
    if (rv != SQLITE_ROW)
        if (rv == SQLITE_ERROR){
            sqlite3_close(db);
            return false;
        }

    result = sqlite3_column_int(stmt, 4);

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return result == 0 ? false : true;
}
