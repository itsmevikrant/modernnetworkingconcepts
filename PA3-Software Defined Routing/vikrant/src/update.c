


#include <string.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <strings.h>
#include <sys/queue.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <arpa/inet.h>
#include "../include/global.h"
#include "../include/control_header_lib.h"
#include "../include/network_util.h"
#include "../include/author.h"
#include "../include/init_response.h"
#include "../include/connection_manager.h"
#include "../include/control_handler.h"
#include "../include/initial_routing_table.h"
#include "../include/PrintRoutingTable.h"
 #include "../include/update.h"
#define NUM_ROUTER_SIZE 2
#define UPDATE_INTERVAL_SIZE 2
#define ROUTER_UPDATE_SIZE 12



void update(int sock_index,char *cntrl_payload){
        //int BUFSIZE;
//  BUFSIZE = 8+12*nR;
        //printf("Bufsize %d, nR %d\n", BUFSIZE,nR);
        //char *cntrl_response_header, *cntrl_response_payload, *cntrl_response, *buf,*pl;
        uint16_t payload_len, response_len;
        char *cntrl_response_header, *cntrl_response;
        //unsigned char *buf = (unsigned char *) malloc(sizeof(char)*BUFSIZE);
//  printf("size of buf %s\n", buf);
        uint16_t nr, rpR, rpD, ui, c, rID,rID1, cost, mypR, Me,   MyNeighbour;
        uint32_t rIP, myIP;
        int p,l,j,m;
        uint16_t pad = 00;

        memcpy(&rID, cntrl_payload, 2);
        rID1 = ntohs(rID);
        memcpy(&cost, cntrl_payload + 0x02, 2);
        c = ntohs(cost);
        for (l=0; l<nR; l++)
        {       Me = myinfo.myID;
                MyNeighbour = rtable[l].router_ID;
                if (MyNeighbour != rID1)
                { printf("[%d]  to [%d]  is [%d] -[Not My Neighbour]\n", Me, MyNeighbour, rtable[l].cost);}
                else
                { if (c< rtable[l].cost) {
                          rtable[l].cost = c;
                          ncost.cost_from_to[Me][MyNeighbour] = c;
                          rtable[l].next_hop_ID = rID1;
                          printf("Cost from Me[%d] to [%d] is %d",myinfo.myID, MyNeighbour, rtable[l].cost );
                  }}}

        PrintRoutingTable();
        cntrl_response_header = create_response_header(sock_index, 3, 0, 0);
        response_len = CNTRL_RESP_HEADER_SIZE;
        cntrl_response = (char *) malloc(response_len);
        // Copy Header
        memcpy(cntrl_response, cntrl_response_header, CNTRL_RESP_HEADER_SIZE);
        free(cntrl_response_header);
        sendALL(sock_index, cntrl_response, response_len);

        free(cntrl_response);

}
