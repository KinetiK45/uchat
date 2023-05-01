#include "../inc/uchat.h"

void create_table_users(void){
    char *zErrMsg = 0;
    char *sql;
    int rc;
    sqlite3 *db = open_db();

    
    sql = "CREATE TABLE IF NOT EXISTS users("     \
          "USER_ID         INTEGER      PRIMARY KEY  AUTOINCREMENT," \
          "LOGIN           TEXT         NOT NULL,"  \
          "PASSWORD        TEXT         NOT NULL,"   \
          "USERNAME        TEXT         NOT NULL," \
          "СLIENT_SOCKET   INT," \
          "PROFILE_IMG     TEXT         NOT NULL);";
    rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
    
    if( rc != SQLITE_OK ){
       sqlite3_free(zErrMsg);
    }
    sqlite3_close(db);

}
