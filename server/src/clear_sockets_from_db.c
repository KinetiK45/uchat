#include "../inc/uchat.h"

void clear_sockets_from_db(void){
    char *command = "UPDATE users SET СLIENT_SOCKET = NULL";
    execute_sql_command(command);
}
