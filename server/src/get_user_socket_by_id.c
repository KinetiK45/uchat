#include "../inc/uchat.h"

int get_user_socket_by_id(int user_id){
    int result_socket = -1;
    sqlite3 *db = open_db();
    sqlite3_stmt *stmt;

    char *sql = "SELECT СLIENT_SOCKET FROM users WHERE USER_ID = ?1";
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        sqlite3_close(db);
        return false;
    }
    
    sqlite3_bind_int(stmt, 1, user_id);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        int client_socket = sqlite3_column_int(stmt, 0);
        if (client_socket != 0) {
            result_socket = client_socket;
        }
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return result_socket;
}
