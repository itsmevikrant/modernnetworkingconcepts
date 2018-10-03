#ifndef CONNECTION_MANAGER_H_
#define CONNECTION_MANAGER_H_
#include <sys/select.h>
#include <netinet/in.h>
#include <strings.h>
#include <sys/queue.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <netdb.h>

int control_socket, router_socket, data_socket;
int whats_timer;
int INTERVAL;
fd_set master_list, watch_list;
int head_fd;
/*----------------------------------*/
struct timeval tv;
int retval;

void init();
void router_conn();
//#define BUFSIZE 1024

#endif
