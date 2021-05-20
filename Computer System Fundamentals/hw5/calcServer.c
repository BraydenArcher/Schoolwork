#include <stdio.h>      /* for snprintf */
#include "csapp.h"
#include "calc.h"

#define LINEBUF_SIZE 1024

int chat_with_client(struct Calc *calc, int infd, int outfd, rio_t *in);

int main(int argc, char **argv) {
    struct Calc *calc = calc_create();
    rio_t in; //Creates a single rio object and passes

    if (argc != 2) { printf("Usage: ./calcServer <port>"); }
    int server_fd = open_listenfd(argv[1]);
    if (server_fd < 0) { printf("Couldn't open server socket\n"); }
    int keep_going = 1;
    while (keep_going) {
        int client_fd = Accept(server_fd, NULL, NULL);
        rio_readinitb(&in, client_fd); //Wrap for each client
        if (client_fd > 0) {
            keep_going = chat_with_client(calc, client_fd, client_fd, &in);
            close(client_fd); // close the connection
        }
    }
    close(server_fd); // close server socket
    return 0;
}

int chat_with_client(struct Calc *calc, int infd, int outfd, rio_t *in) { //Adapted from calcInteractive
    char linebuf[LINEBUF_SIZE];

    /* wrap standard input (which is file descriptor 0) */
    rio_readinitb(in, infd);

    /*
     * Read lines of input, evaluate them as calculator expressions,
     * and (if evaluation was successful) print the result of each
     * expression.  Quit when "quit" command is received.
     */

    while (1) {
        ssize_t n = rio_readlineb(in, linebuf, LINEBUF_SIZE);
        if (n <= 0 || strcmp(linebuf, "quit\n") == 0 || strcmp(linebuf, "quit\n\r") == 0) {
            /* error or end of input */
            return 1;
        } else if (strcmp(linebuf, "shutdown\n") == 0 || strcmp(linebuf, "shutdown\r\n") == 0) {
            /* quit command */
            return 0;
        } else {
            /* process input line */
            int result;
            if (calc_eval(calc, linebuf, &result) == 0) {
                /* expression couldn't be evaluated */
                rio_writen(outfd, "Error\n", 6);
            } else {
                /* output result */
                int len = snprintf(linebuf, LINEBUF_SIZE, "%d\n", result);
                if (len < LINEBUF_SIZE) {
                    rio_writen(outfd, linebuf, len);
                }
            }
        }
    }
}
