#include "../inc/uchat.h"

void daemonize(int port) {
    pid_t pid = fork();
    if (pid < 0) //if forking failed
        die_with_error("fork failed\n");
    if (pid > 0 ) {
        mx_printerr(mx_itoa((int)pid));
        die_with_error("\n");
    }//if forking successful, exit from parent process
    else {
        //deamon_successfull_output(getpid()); //useful output about deamon
        umask(0);   //make files available to read/write
        close(STDIN_FILENO);    //close input from terminal
        close(STDOUT_FILENO);   //close output to terminal
        close(STDERR_FILENO);   //close error stream
        setsid();   //make child process parent, detach from terminal
    }
    start_server(port);
}
