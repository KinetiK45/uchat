#include "../inc/uchat.h"

t_userinfo *find_user(char* username_start){
    t_userinfo *result = malloc(sizeof(t_userinfo) * 4);
    sqlite3 *db = open_db();
    
    sqlite3_stmt *stmt = NULL;
    
    char *pattern = mx_strjoin(username_start, "%");
        
    char *command = "SELECT * FROM users WHERE USERNAME LIKE ?1";
    
    int rv = sqlite3_prepare_v2(db, command, -1, &stmt, NULL);

    sqlite3_bind_text(stmt, 1, pattern, -1, SQLITE_TRANSIENT);
    mx_strdel(&pattern);

    if (rv != SQLITE_OK){
        free(result);
        sqlite3_close(db);
        return NULL;
    }

    rv = sqlite3_step(stmt);
    
    if (rv != SQLITE_ROW){
        free(result);
        sqlite3_close(db);
        return NULL;
    }
    for(int i = 0; i <= 3; i++){
        result[i].user_id = -100;
        result[i].username = NULL;
        result[i].profile_img = NULL;
    }
    for(int i = 0; i < 3 && rv == SQLITE_ROW; i++){
        result[i].user_id = sqlite3_column_int(stmt, 0);
        result[i].username = mx_strdup((char*) sqlite3_column_text(stmt, 3));
        result[i].profile_img = mx_strdup((char*) sqlite3_column_text(stmt, 5));
        rv = sqlite3_step(stmt);
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return result;
}
