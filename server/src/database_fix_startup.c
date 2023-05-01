#include "../inc/uchat.h"

void database_fix_startup(void){
  char *sql = "DELETE FROM users WHERE LOGIN = '' OR PASSWORD = '';";
    execute_sql_command(sql);
    sql = "UPDATE users SET USERNAME = 'unnamed' WHERE USERNAME = '';";
    execute_sql_command(sql);
    sql = "UPDATE users SET PROFILE_IMG = 'default.png' WHERE PROFILE_IMG = '';";
    execute_sql_command(sql);
    sql = "DELETE FROM users_msgs WHERE SENDER_ID < 1;";
    execute_sql_command(sql);
    sql = "DELETE FROM users_msgs WHERE CHAT_ID < 1 AND TIME != 'first msg';";
    execute_sql_command(sql);
    sql = "DELETE FROM users_msgs WHERE CHAT_ID NOT IN " \
    "(SELECT CHAT_ID FROM users_msgs WHERE TIME = 'first msg' " \
    " GROUP BY CHAT_ID HAVING COUNT(*) >= 2)";
    execute_sql_command(sql);
}
