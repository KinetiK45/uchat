#include "../inc/uchat.h"

void change_profile_img_to_accid(int acc_id, char* new_profile_img){
    sqlite3 *db = open_db();

    sqlite3_stmt *stmt = NULL;
            
    char *command = "UPDATE users SET PROFILE_IMG = ?1 WHERE USER_ID = ?2;";
    
    int rv = sqlite3_prepare_v2(db, command, -1, &stmt, NULL);

    sqlite3_bind_text(stmt, 1, new_profile_img, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, acc_id);

    if (rv != SQLITE_OK){
        sqlite3_close(db);
        return;
    }

    rv = sqlite3_step(stmt);

    sqlite3_finalize(stmt);
    sqlite3_close(db);
}
