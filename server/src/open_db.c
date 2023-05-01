#include "../inc/uchat.h"

sqlite3 *open_db(void){
    sqlite3 *db;
    int rc;
    
    rc = sqlite3_open(RESOURCES_LOCATION \
                      "database.db", &db);
    
    if( rc ) {
        return NULL;
    }
    return db;
}
