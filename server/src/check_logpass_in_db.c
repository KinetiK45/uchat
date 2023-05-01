#include "../inc/uchat.h"

int check_logpass_in_db(t_registr *reg){
    int result = get_id_by_log_pass(reg -> login, reg -> pass);
    return result;
}
