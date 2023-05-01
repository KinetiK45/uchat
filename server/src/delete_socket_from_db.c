#include "../inc/uchat.h"

void delete_socket_from_db(int client_socket) {
    sqlite3 *db = open_db();
    int rc;
    
    sqlite3_stmt *stmt = NULL;

    char *sql = "UPDATE users SET СLIENT_SOCKET = NULL WHERE СLIENT_SOCKET = ?1";
    
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return;
    }

    sqlite3_bind_int(stmt, 1, client_socket);

    rc = sqlite3_step(stmt);

    sqlite3_finalize(stmt);
    sqlite3_close(db);
}
