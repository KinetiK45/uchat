#include "../inc/uchat.h"

int get_chatid_between_users(int sender_id_1, int sender_id_2){
    if (sender_id_1 < 1 || sender_id_2 < 1) return -1;
    sqlite3 *db = open_db();
    sqlite3_stmt *stmt = NULL;
    int chat_id = -1;

    char *command = "SELECT um.CHAT_ID FROM users_msgs "\
                    "um WHERE um.SENDER_ID = ?1 AND um.CHAT_ID IN "\
                    "(SELECT um2.CHAT_ID FROM users_msgs um2 WHERE um2.SENDER_ID = ?2)"\
                    " LIMIT 1;";
    
    int rv = sqlite3_prepare_v2(db, command, -1, &stmt, NULL);
    if (rv != SQLITE_OK) {
        sqlite3_close(db);
        return -1;
    }

    sqlite3_bind_int(stmt, 1, sender_id_1);
    sqlite3_bind_int(stmt, 2, sender_id_2);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        chat_id = sqlite3_column_int(stmt, 0);
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    if (chat_id == -1){
        initialize_new_chat(sender_id_1, sender_id_2);
        return get_chatid_between_users(sender_id_1, sender_id_2);
    }
    return chat_id;
}
