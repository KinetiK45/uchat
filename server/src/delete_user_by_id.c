#include "../inc/uchat.h"

void delete_user_by_id(int id){
    sqlite3 *db = open_db();
    
    sqlite3_stmt *stmt = NULL;

    char *command = "DELETE FROM users WHERE USER_ID = ?1;";
    
    int rv = sqlite3_prepare_v2(db, command, -1, &stmt, NULL);

    sqlite3_bind_int(stmt, 1, id);

    if (rv != SQLITE_OK){
        return;
    }

    rv = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    int * chat_ids = get_chats_for_user(id);
    for (int i = 0; chat_ids && chat_ids[i] && chat_ids[i] != -1; i++){
        delete_chat(chat_ids[i]);
    }
}
