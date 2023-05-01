#include "../inc/uchat.h"

void prepare_database(void){
    create_table_users();
    create_table_users_msgs();
    clear_sockets_from_db();
    database_fix_startup();
}
