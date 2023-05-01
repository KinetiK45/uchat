#include "../inc/uchat.h"

void write_socket_in_db(int client_socket, int user_id){
    sqlite3_stmt *stmt = NULL;
    sqlite3 *db = open_db();
    
    char *command = "UPDATE users SET СLIENT_SOCKET = ?1 WHERE USER_ID = ?2";
    
    int rv = sqlite3_prepare_v2(db, command, -1, &stmt, NULL);

    sqlite3_bind_int(stmt, 1, client_socket);
    sqlite3_bind_int(stmt, 2, user_id);
    
    if (rv != SQLITE_OK){
        sqlite3_close(db);
        return;
    }

    rv = sqlite3_step(stmt);
    if (rv != SQLITE_ROW){
        if (rv == SQLITE_ERROR){
            sqlite3_close(db);
            return;
        }
    }
        
    
    sqlite3_finalize(stmt);
    sqlite3_close(db);
}
