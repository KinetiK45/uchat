#include "../inc/uchat.h"

void execute_sql_command(char *command){
    char *zErrMsg = 0;
    int rc;
    sqlite3 *db = open_db();
    rc = sqlite3_exec(db, command, callback, 0, &zErrMsg);
    if( rc != SQLITE_OK ){
       sqlite3_free(zErrMsg);
    }
    sqlite3_close(db);
}
