#include <stdio.h>      /* for snprintf */
#include "csapp.h"
#include "calc.h"
#include <poll.h>

#define LINEBUF_SIZE 1024

//Argument struct to pass to threads
struct Argument {
    int client_fd;
    struct Calc* sharedCalc;
    int *shutdown;
    rio_t in;
    pthread_mutex_t threadMutex;
    int *threadCount;
};

void *chat_with_client(void *vargp);

int main(int argc, char **argv) {
    struct Calc *calc = calc_create();
    rio_t in;
    pthread_t threadId;
    int shutdown = 0;
    int threadCount = 0;
    pthread_mutex_t threadMutex = PTHREAD_MUTEX_INITIALIZER; //Initialize mutex
    struct pollfd pollfds[32];

    if (argc != 2) {
        printf("Usage: ./calcServer <port>\n");
        return 1;
    }
    int server_fd = open_listenfd(argv[1]);
    if (server_fd < 0) {
        printf("Couldn't open server socket\n");
        return 1;
    }

    for (int i = 0; i < 32; i++) { //Make sure no data is uninitialized
        pollfds[i].fd = -1;
        pollfds[i].events = 0;
    }


    pollfds[0].fd = server_fd; //Set up server stuff
    pollfds[0].events = POLLIN;

    while (!shutdown) {
        for(int i = 0; i < 32; i++) { //reset poll each time
            pollfds[i].revents = 0;
        }
        poll(pollfds, 32, 1); //Poll for new connections
        if(pollfds[0].revents & POLLIN) { //On new connection
            int client_fd = accept(server_fd, NULL, NULL); //Accept the connection
            rio_readinitb(&in, client_fd); //Wrap for each client
            if (client_fd > 0) {
                struct Argument *arguments = malloc(sizeof(struct Argument));
                //Prepare arguments to be passed
                arguments->client_fd = client_fd;
                arguments->sharedCalc = calc;
                arguments->in = in;
                arguments->shutdown = &shutdown;
                arguments->threadMutex = threadMutex;
                arguments->threadCount = &threadCount;
                pthread_create(&threadId, NULL, chat_with_client, arguments); //Create a new thread
            }
        }
    }
    close(server_fd); // close server socket
    while(1) {
        //wait for final client to close
        if(threadCount <= 0) {
            //Delete the calc object and mutex and return
            pthread_mutex_destroy(&threadMutex);
            calc_destroy(calc);
            return 0;
        }
    }
}

void *chat_with_client(void *vargp) { //Adapted from calcInteractive
    pthread_detach(pthread_self());
    struct Argument *arguments = (struct Argument *) vargp;


    pthread_mutex_lock(&arguments->threadMutex);//Mutex locks to prevent discrepancies
    (*arguments->threadCount)++; //Add a client to thread count
    pthread_mutex_unlock(&arguments->threadMutex);


    //Parse arguments for use in program
    struct Calc *calc = arguments->sharedCalc;
    rio_t *in = &arguments->in;
    int infd = arguments->client_fd;
    int outfd = infd;
    char linebuf[LINEBUF_SIZE];

    /* wrap standard input (which is file descriptor 0) */
    rio_readinitb(in, infd);

    while (1) {
        ssize_t n = rio_readlineb(in, linebuf, LINEBUF_SIZE);
        if (n <= 0) {
            /* error or end of input */
            close(arguments->client_fd);
            break;
        } else if (strcmp(linebuf, "quit") == 0 || strcmp(linebuf, "quit\n") == 0 || strcmp(linebuf, "quit\r\n") == 0) {
            /* quit command */
            close(arguments->client_fd);
            break;
        } else if (strcmp(linebuf, "shutdown") == 0 || strcmp(linebuf, "shutdown\n") == 0 || strcmp(linebuf, "shutdown\r\n") == 0) {
            (*arguments->shutdown) = 1; //Indicate to server to shutdown
            close(arguments->client_fd);
            break;
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
    pthread_mutex_lock(&arguments->threadMutex); //Mutex locks to prevent discrepancies
    (*arguments->threadCount)--; //remove client from thread count
    pthread_mutex_unlock(&arguments->threadMutex);
    free(arguments); //Free arguments struct
    return NULL;
}
