#include "../inc/uchat.h"

bool register_new_user(char *login, char *pass){
    sqlite3_stmt *stmt = NULL;
    if(login == NULL || pass == NULL){
        return false;
    }
    if (get_id_by_login(login) != -1){
        return false;
    }
    sqlite3 *db = open_db();
    
    char *command = "INSERT INTO users (LOGIN, PASSWORD, USERNAME, PROFILE_IMG) "  \
              "VALUES (?1, ?2, ?3, ?4);";
    int rv = sqlite3_prepare_v2(db, command, -1, &stmt, NULL);
    if (rv == SQLITE_ERROR){
        sqlite3_close(db);
        return false;
    }
    sqlite3_bind_text(stmt, 1, login, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, pass, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, "unnamed", -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, "default.png", -1, SQLITE_STATIC);
    
    if ((rv = sqlite3_step(stmt)) != SQLITE_DONE) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return false;
    }
    
    sqlite3_finalize(stmt);
        
    sqlite3_close(db);
    return true;
}
