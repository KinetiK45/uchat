#include "../inc/uchat.h"

void create_table_users_msgs(void){
    char *zErrMsg = 0;
    char *sql;
    int rc;
    sqlite3 *db = open_db();
    sql = "CREATE TABLE IF NOT EXISTS users_msgs("     \
          "MSG_ID          INTEGER      PRIMARY KEY  AUTOINCREMENT,"  \
          "CHAT_ID         INTEGER      NOT NULL," \
          "SENDER_ID       INTEGER      NOT NULL,"  \
          "TIME            TEXT         NOT NULL,"  \
          "MSG_TEXT        TEXT         NOT NULL);";
    
    rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
    
    if(rc != SQLITE_OK){
       sqlite3_free(zErrMsg);
    }
    sqlite3_close(db);
}
