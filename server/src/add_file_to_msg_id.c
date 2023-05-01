#include "../inc/uchat.h"

void add_file_to_msg_id(int msg_id, char *filename){
    char *temp = mx_strjoin("<file>", filename);
    char *file_str = mx_strjoin(temp, "</file>");
    mx_strdel(&temp);

    sqlite3 *db = open_db();

    char *command = "UPDATE users_msgs SET MSG_TEXT = MSG_TEXT || ?1 WHERE MSG_ID = ?2";

    sqlite3_stmt *stmt = NULL;
    int rv = sqlite3_prepare_v2(db, command, -1, &stmt, NULL);
    if (rv != SQLITE_OK) {
        sqlite3_close(db);
        mx_strdel(&file_str);
        return;
    }

    sqlite3_bind_text(stmt, 1, file_str, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, msg_id);

    rv = sqlite3_step(stmt);
    
    if (rv != SQLITE_DONE) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        mx_strdel(&file_str);
        return;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    mx_strdel(&file_str);

}
