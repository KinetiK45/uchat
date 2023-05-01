#pragma once

#include "../../libraries/sqlite3/inc/sqlite3.h"
#include "../../libraries/libmx/inc/libmx.h"
#include "../../libraries/jsonc/inc/json.h"

#include <gtk/gtk.h>
#include <gtk/gtkeditable.h>
#include "stdbool.h"
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
#include <time.h>

#include <openssl/crypto.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/rand.h>
#include <openssl/evp.h>
#include <openssl/hmac.h>
#define PORT 1049
#define MAX_CLIENTS_PER_ONCE 120
#define RECV_BUFF_SIZE 16192

#define ENTRY_MSG "press # to exit \n"
#define MSG "im connected"

#define RESOURCES_LOCATION "resources/"

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

GtkWidget* dialog_add;
GtkWidget *window;
GtkApplication *app;
GtkWidget *l_entry;
GtkWidget *p_entry;
GtkWidget  *signUp;
GtkWidget* registration;
GtkWidget* registration_window;
GtkWidget* l_fixed;
GtkWidget* registration_login;
GtkWidget *menuitem1;
GtkWidget *menuitem2;
GtkWidget *menuitem3;
GtkWidget *message_box;
GtkWidget *menuitem4;
GtkWidget* registrate_fixed;
GtkWidget *menu;
GtkWidget* registration_password;
GtkWidget* confirmation_password;
GtkWidget* reg;
GtkWidget* interface_window;
GtkWidget* i_window;
GtkWidget* show_password;
GtkWidget* show_password_regisrtation;
GtkWidget* interface_search;
GtkWidget* registration_confirm;
GtkWidget* scroll_massage;
GtkWidget* fixed;
GtkWidget* box_user;
GtkWidget* button_close;
GtkWidget* button_delete;
GtkWidget* button_save;
GtkWidget* button_change_thema;
GtkWidget* entry_status;
GtkWidget* entry_name;
GtkWidget* entry_email;
GtkWidget* button_change_photo;
GtkImage* user_photo;
GtkWidget* interface_box1;
GtkWidget* interface_box2;
GtkWidget* scroll_user;
GtkWidget* dialog_vbox;
GtkWidget* box_show;
GtkWidget* profile_fixed;
GtkWidget* box_show_message;
GtkWidget* scroll_search;
GtkWidget* button_cancel;
GtkWidget* button_box_dialog;
GtkWidget* box_new_chat;
GtkWidget* box_mesage_horizontal_part;
GtkWidget* button_user_name;
GtkWidget* search_user;
GtkWidget* sent_message;
GtkWidget* button_send;
GtkWidget* button_add;
GtkWidget* scroll_search_group;
GtkWidget* box_search_group;
GtkWidget* box_show_group;
GtkWidget* search_group;
GtkWidget* box_search;
GtkWidget* profile_photo;
GtkWidget* group_box;
GtkWidget* button_cancel_group;
GtkWidget* button_box_group;
GtkWidget* button_search_group;
GtkWidget* window_info_about_user;
GtkWidget* button_box_dialog;
GtkWidget* dialog_group;
GtkWidget* button_search_name;
GtkWidget* fixed_search;
GtkWidget* box_get_message;
GtkWidget* entry_caption;
GtkWidget *menu_edit;
GtkWidget* window_profile;
GtkWidget* button_change_thema;
GtkWidget* button_dialog_name;

char buff[RECV_BUFF_SIZE];
char* find_name;

typedef struct sockaddr_in t_saddr;

typedef struct s_msg_to_edit{
    GtkWidget* label;
    GtkWidget* box;
}t_msg_to_edit;
typedef struct s_sock {
    int socket;
    t_saddr address;
    SSL_CTX *ctx;
    SSL *ssl;
    pthread_mutex_t mutex;
} t_sock;

typedef struct s_user_info{
    int user_id;
    char* username;
    char* photo_path;
    t_sock* sock;
}t_user_info;

typedef struct s_msg_info{
    t_sock *sock;
    GtkWidget* widget;
    char* recepient;
    char* text;
}t_msg_info;

typedef struct s_chat_msg{
    int msg_id;
    int sender_id;
    char* time;
    char* msg_text;
    char* filename;
}t_chat_msg;

typedef struct s_thread_args
{
    t_sock *client_sock;
} t_thread_args;


typedef struct s_wrapper{
    t_sock *sock;
}t_wrapper;


typedef struct s_rabotyaga_reciepient{
    int user_id;
    t_sock* sock;
    int count_files;
    int num_msgs;
    char* username;
    char* profile_img;
    char *warning_msg;
    char *file_path;
    char *filename;
    unsigned long filesize;
}t_rabotyaga_reciepient;

int current_user;
t_rabotyaga_reciepient *rabotyaga;
t_user_info* user_to_add;
GtkWidget* button_search_name;

unsigned long send_file(t_sock *sock, char *path, unsigned long filesize);
t_sock *socket_creation(char* argv[]);
void init_clnt_ctx(t_sock *sock);
void init_clnt_ssl(t_sock *sock);
void create_and_launch_recv_thread(t_sock *sock);
void *recv_messages_from_server(void *thread_args);
char* create_msg(char* sending_msg, int recepient);
char* create_login(char* login, char* pass);
char* create_reg(char* login, char* pass);
int mx_str_cmp(const char *s1, const char *s2);
char* gettime(void);
ssize_t send_message(t_sock *sock, char *sending_msg);
gboolean interface(gpointer data);
void regist(GtkWidget* w, t_sock* sock);
void log_in(GtkWidget *w,t_sock* sock);
void log_in_table (GtkApplication* app, t_sock* sock);
int init_client(int argc,char **argv, t_sock* sock);
void request_msg_history(GtkButton* button, gpointer data);
t_sock* get_socket(char* argv[]);
gboolean on_parent_box_button_press(GtkWidget* widget,GdkEventButton *event, gpointer data);
bool check_login_info(char* buff);
t_chat_msg* get_msg_history(char* buff);
bool check_history_info(char* buff);
bool check_users_info(char* buff);
char* check_user_info(char* buff);
bool check_reg_info(char* buff);
void show_found_users(t_user_info* user_info);
void send_clnts_msg_to_server(GtkButton *button, gpointer data);
gboolean show_user_what_found(gpointer data);
gboolean sent(gpointer data);
void load_css(void);
gboolean on_draw_event(GtkWidget* widget, cairo_t* cr, gpointer user_data);

char *get_text_no_files(char* msg_text);
void request_photo_change(char *name,t_sock *sock,unsigned long filesize);
void search_request(GtkButton *button, gpointer data);
gboolean on_key_press(GtkWidget *widget, GdkEventKey *event, t_sock* sock);
t_wrapper* wrapper(t_sock* sock);
gboolean add_new_to_box_message(gpointer data);
void add_new_dialog(GtkButton* button,gpointer data);
gboolean add(gpointer data);
void get_dialog(GtkButton* button, gpointer data);
t_user_info* get_users_info(char* buff);
GtkWidget* create_round_image(const gchar* filename, gint size);
void show_group_what_found(void);
void on_switch_toggled(GtkSwitch *switch_button, gpointer user_data);
void destroy_widget(GtkWidget *widget, gpointer data);
gboolean info_about_user(gpointer data);
gboolean change_profile(gpointer data);
char* edit_profile(char* username, char* password);
gboolean request_editing(GtkButton* button, gpointer data);
t_user_info* get_user_info(char* buff);
gboolean send_photo(GtkButton* b,gpointer data);
char* create_file_warning(char *file_name,int reciepient_id, unsigned long filesize, char *msg_text);
gboolean refresh_messages(gpointer data);
gboolean on_button_clicked(GtkSwitch *switch_button, gpointer user_data);
gboolean on_label_button_press_event(GtkWidget *widget, GdkEventButton *event, gpointer data);
void on_copy_activate(GtkWidget *widget, gpointer user_data);
void on_edit_activate(GtkWidget *widget, gpointer user_data);
void on_delete_activate(GtkWidget *widget, gpointer user_data);
char* edit_msg(char* new_msg, int msg_id);
char* delete_msg(int msg_id, int reciepient);
gboolean add_request(GtkButton* button, gpointer data);
gboolean delete_chat(GtkButton* button, gpointer data);
gboolean clear_history_chat(GtkButton* button, gpointer data);
void save_switch_state(gboolean state);
gboolean read_switch_state(void);
void on_window_destroy(GtkWidget *widget, gpointer data);
gboolean request_info_about_user(GtkButton* button, gpointer data);
void get_info_about_user(char* buff);
gboolean window_reconnect(gpointer data);
void get_user_to_delete(char* buff);
gboolean delete_user_box(gpointer data);
