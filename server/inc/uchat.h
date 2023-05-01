#ifndef uchat_h
#define uchat_h

#include "../../libraries/sqlite3/inc/sqlite3.h"
#include "../../libraries/libmx/inc/libmx.h"
#include "../../libraries/jsonc/inc/json.h"

#include <unistd.h>
#include "sys/stat.h"
#include "sys/types.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/select.h>

#include <openssl/crypto.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/rand.h>
#include <openssl/evp.h>
#include <openssl/hmac.h>


#define PORT 9999
#define MAX_CLIENTS_PER_ONCE 120
#define BUFF_SIZE 16192
#define OPT 128;

#define RESOURCES_LOCATION "resources/"

#define CHANGE_READ_MODE 1
#define LONG_READ_MODE 1
#define SHORT_READ_MODE 0
#define REG "reg"
#define LOG "log"
#define SEARCH_USER "search_usr"
#define LOGIN_SUCCESS "logined"
#define REGIST_SUCCESS "registered"
#define LOGIN_ERROR "log_error"
#define REG_ERROR "reg_error"
#define USER_FOUND "user_found"
#define NO_USER "no_user"
#define MSG_HISTORY "msg_history"
#define EDIT_PROFILE "edit_profile"
#define CUR_USER "cur_user"
#define DEL_USER "del_user"
#define BIG_FILE "big_file"



//server

typedef struct s_registr t_registr;
typedef struct sockaddr_in t_saddr;
typedef struct s_connected_peer {
    int clnt_socket;
    int user_id;
    t_saddr address;
    SSL *ssl;
    char send_buff[BUFF_SIZE];
    char recv_buff[BUFF_SIZE];
    int reading_mode;
    int recipient_id;
    unsigned long filesize;
    char *file_path;
} t_connected_peer;


typedef struct s_sock{
    int main_socket_fd;
    struct s_connected_peer connections[MAX_CLIENTS_PER_ONCE];
    t_saddr address;
    int addrlen;
    fd_set readfds;
    fd_set exceptfds;
    SSL_CTX *ctx;
    
} t_sock;

typedef struct s_message{
    int reciepient;
    char* text;
    char* date;
}t_message;

typedef struct s_userinfo{
    int user_id;
    char* profile_img;
    char* username;
}t_userinfo;

typedef struct s_chat_msg{
    int msg_id;
    int sender_id;
    char* time;
    char* msg_text;
}t_chat_msg;


void start_server(int port);
void load_certificates(SSL_CTX *ctx, char *cert_file, char *key_file);
SSL *create_new_clnt_ssl(SSL *ssl,int clnt_sock);
void create_server_context(t_sock *sock);
void recieve_msg_from_client(t_sock *sock, t_connected_peer *peer);
void handle_receiving_data(t_sock *sock);
void add_new_socket_to_connections_arr(t_sock *sock, int *new_clnt_sock);
void handle_connection(t_sock *sock);
void waiting_for_connections_loop(t_sock *sock);
char *mx_getline(void);
int *get_chats_for_user(int user_id);
void daemonize(int port);
t_sock *server_socket_creation(int port);
int create_fd_set(t_sock *sock);
void die_with_error(char *err_msg);
void change_msg_by_msgid(int msg_id, char *new_msg);
void accept_connection(t_sock *serv_sock, int *new_clnt_sock);
void sending_msg(void *thread_args);
bool is_clnt_leaving(char *msg);
t_message* get_msg_info(char* buffer);
char* msg_for_registr(void);
t_registr* get_login_info(char *buffer);
t_registr* get_registr_info(char *buffer);
void delete_socket_from_db(int client_socket);
char* get_action(char *msg);
ssize_t send_message_by_socket(t_connected_peer *peer, char *sending_msg);
void message_from_client_handling(char *msg, t_connected_peer *client_socket, int action);
char* login_success(char* login);
bool check_user_online_by_id(int user_id);

struct thread_args {
    int clnt_socket;
};

char *mx_getline(void);

typedef struct s_registr{
    char *login;
    char *pass;
}t_registr;

int callback(void *NotUsed, int argc, char **argv, char **azColName); //для вывода ошибок
bool register_new_user(char *login, char *pass);
void check_action(t_sock *sock, char *msg, t_connected_peer *peer);


int get_chatid_between_users(int sender_id_1, int sender_id_2);

int get_id_by_log_pass(char *login, char *pass);
void create_table_users(void);
void execute_sql_command(char *command);
void print_table_from_db(char *table_name);
int check_logpass_in_db(t_registr *reg);
bool find_client_socket_in_db(int client_socket);
void clear_sockets_from_db(void);
void write_socket_in_db(int client_socket, int user_id);
sqlite3 *open_db(void);
t_userinfo *find_user(char *username_start);
void change_username(int acc_id, char* new_name);
void delete_user_by_id(int id);
int get_id_by_login(char *login);
void change_profile_img_to_accid(int acc_id, char* new_profile_img);
bool change_pass_by_id(int acc_id, char* new_pass);
t_chat_msg* get_msg_history(int user_1, int user_2, int chat_id);
void create_table_users_msgs(void);
void register_new_msg_in_db(int chat_id, int sender_id, char *time, char *text);
void initialize_new_chat(int user_1, int user_2);
void register_new_msg_from_user(int sender_id, int send_to, char *msg);
void delete_chat(int chat_id);
void delete_msg(int msg_id);
t_userinfo get_userinfo_by_chatid_for_userid(int chat_id, int request_for_user_id);
void database_fix_startup(void);
void prepare_database(void);
void create_history(t_connected_peer* peer, int id_1, int id_2);

void add_file_to_msg_id(int msg_id, char *filename);
char** get_files_from_msg(int msg_id);
t_userinfo get_userinfo_by_id(int user_id);
json_object* userinfo_to_json(t_userinfo user_info);
t_connected_peer *get_sock_for_send(t_sock *sock, int clnt_sock);
int get_user_socket_by_id(int user_id);
int check_chat_existing(int sender_id_1, int sender_id_2);
char* user_to_add(int reciepient);
int get_msg_size(int chat_id);
char* info_about_user(t_userinfo user_info, int num, int count_files);
void delete_chat_history_by_id(int chat_id);
char* chat_deleted(int reciepient);

#endif
