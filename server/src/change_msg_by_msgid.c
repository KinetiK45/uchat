#include "../inc/uchat.h"

void change_msg_by_msgid(int msg_id, char *new_msg){
    time_t now = time(0);
    char *temp = ctime(&now);
    char *time_edited = mx_strndup(temp, mx_strlen(temp) - 1);
    
    
    sqlite3 *db = open_db();
    sqlite3_stmt *stmt;
    
    char *sql = "UPDATE users_msgs SET MSG_TEXT = ?1, TIME = ?2 WHERE MSG_ID = ?3;";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        sqlite3_close(db);
        return;
    }
    sqlite3_bind_text(stmt, 1, new_msg, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, time_edited, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 3, msg_id);

    sqlite3_step(stmt);
    sqlite3_close(db);
    mx_strdel(&time_edited);
}
