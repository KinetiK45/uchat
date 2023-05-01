#include "../inc/uchat.h"

char** get_files_from_msg(int msg_id){
    char** result = NULL;
    int count = 0;
    
    sqlite3* db = open_db();
    char* sql = "SELECT MSG_TEXT FROM users_msgs WHERE MSG_ID = ?1";
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        return NULL;
    }

    sqlite3_bind_int(stmt, 1, msg_id);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_ROW) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return NULL;
    }

    const unsigned char* msg_text = sqlite3_column_text(stmt, 0);

    char* start = mx_strstr((char*)msg_text, "<file>");
    while (start != NULL) {
        start += 6;
        char* end = mx_strstr(start, "</file>");
        if (end == NULL) {
            break;
        }

        int len = end - start;
        char* filename = mx_strndup(start, len);
        count++;
        result = mx_realloc(result, sizeof(char*) * (count + 1));
        result[count - 1] = filename;
        result[count] = NULL;

        start = mx_strstr(end, "<file>");
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return result;
}
