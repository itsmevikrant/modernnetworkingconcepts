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
#define NUM_ROUTER_SIZE 2
#define UPDATE_INTERVAL_SIZE 2
#define ROUTER_UPDATE_SIZE 12

#define INF 65535 //1 less than maximum of 2^16 = 65536


char* ip_to_text(uint32_t ip_address) {
        struct in_addr ip_addr;
        ip_addr.s_addr = htonl(ip_address);
        return inet_ntoa(ip_addr);
}

void init_response(int sock_index, char *cntrl_payload)
{
        //printf(" \tstarting creation of initial routing table from controller update\n");
        uint16_t payload_len, response_len;
        char *cntrl_response_header, *cntrl_response;
        uint16_t nr, rpR, rpD, ui, c, rID,rID1, cost;
        uint32_t rIP;
        int i,j,k;
        unsigned char ipv4[4];

//https://stackoverflow.com/questions/4393516/c-byte-copy-unsigned-char-value
//payload_len = sizeof(cntrl_payload);
        memcpy(&nr, cntrl_payload, 2);
        memcpy(&ui,cntrl_payload + 0x02,2);
//int HEADER_SIZE = NUM_ROUTER_SIZE+UPDATE_INTERVAL_SIZE;
//gvofR.num_routers = ntohs(nR);
        nR = ntohs(nr);
        //printf("\n no of routers %d \n",nR);
        uI = ntohs(ui);
        //  printf("\n Upate Interval %d \n",uI);
//gvofR.update_interval = ntohs(uI);
        //printf("---------------------------------------------\n");
        //printf(" Data recieved from Control Payload for INIT \n");
        //printf("---------------------------------------------\n");
        for(i=0; i < nR; i++)
        { //int STARTING_POINT = HEADER_SIZE+(ROUTER_UPDATE_SIZE)*(i);

                memcpy(&routerinit,cntrl_payload + (0x04)+(ROUTER_UPDATE_SIZE*i),8);
                memcpy(&rIP,cntrl_payload + (0x0C)+(ROUTER_UPDATE_SIZE*i),4);///sizeof(routerinfo[i]));
                routerinfo[i].router_ID = ntohs(routerinit.router_ID);
                routerinfo[i].router_port_R = ntohs(routerinit.router_port_R);
                routerinfo[i].router_port_D = ntohs(routerinit.router_port_D);
                routerinfo[i].cost = ntohs(routerinit.cost);
                routerinfo[i].router_ip_addr = ntohl(rIP);
        }

        //Finde neighbours with inf
        //printf ("No. of Loops %d \n ",nR);
        for (j=0; j<nR; j++)
        {
                if (routerinfo[j].cost == 0)
                {
                        myinfo.myID         = routerinfo[j].router_ID;
                        myinfo.myIP         = routerinfo[j].router_ip_addr;
                        //printf("MY IP 1 : %s\n",ip_to_text(myinfo.myIP));
                        myinfo.my_next_hop_ID  = routerinfo[j].router_ID;
                        myinfo.myRport      = routerinfo[j].router_port_R;
                        myinfo.myDport      = routerinfo[j].router_port_D;
                        myinfo.myCport = CONTROL_PORT;
                        rtable[j].router_ID          = myinfo.myID;
                        rtable[j].router_port_R      = myinfo.myRport;
                        rtable[j].router_ip          = myinfo.myIP;
                        rtable[j].cost               = routerinfo[j].cost;
                        rtable[j].next_hop_ID    = myinfo.myID;
                        for (k=0; k<nR; k++)
                        {
                                ncost.cost_from_to[j][k]=routerinfo[k].cost;
                                //  printf("distance vector cost from %d to %d is %d \n",j,k,ncost.cost_from_to[j][k]);
                                //printf(" \n\t%d  %d  %d\n",j,k,ncost.cost_from_to[j][k]);

                        }
                }
                else if (routerinfo[j].cost == INF) //INF
                {
                        rtable[j].neighbour = 0;
                        rtable[j].router_ID          = routerinfo[j].router_ID;
                        //printf("\nrID %d\n", htons(rtable[j].router_ID));
                        rtable[j].router_port_R      = routerinfo[j].router_port_R;
                        //  printf("\nrP %d\n", htons(rtable[j].router_port_R));
                        rtable[j].router_ip          = routerinfo[j].router_ip_addr;
                        //  printf("Not NEIGBOUR IP : %s\n",ip_to_text(rtable[j].router_ip));
                        rtable[j].next_hop_ID  = htons(INF);
                        //  printf("\nnext HoP %d\n", htons(rtable[j].router_ID));
                        rtable[j].cost               = routerinfo[j].cost;
                        //  printf("\nCost %d\n", rtable[j].cost);
                }
                else //if (routerinfo[j].cost < 65535 && routerinfo[j].cost > 0)
                {
                        rtable[j].neighbour = 1;
                        rtable[j].router_ID          = routerinfo[j].router_ID;
                        //printf("\nrID %d\n", htons(rtable[j].router_ID));
                        rtable[j].router_port_R      = routerinfo[j].router_port_R;
                        //printf("\nrP %d\n", htons(rtable[j].router_port_R));
                        rtable[j].router_ip          = routerinfo[j].router_ip_addr;
                        //  printf("NEIGBOUR IP : %s\n",ip_to_text(rtable[j].router_ip));
                        rtable[j].next_hop_ID    = routerinfo[j].router_ID;
                        //  printf("\nnext HoP %d\n", htons(rtable[j].router_ID));
                        rtable[j].cost               = routerinfo[j].cost;
                        //printf("\nCost %d\n", rtable[j].cost);


                }
        }


        cntrl_response_header = create_response_header(sock_index, 1, 0, payload_len);

        response_len = CNTRL_RESP_HEADER_SIZE;
        cntrl_response = (char *) malloc(response_len);
// Copy Header
        memcpy(cntrl_response, cntrl_response_header, CNTRL_RESP_HEADER_SIZE);
        free(cntrl_response_header);


        sendALL(sock_index, cntrl_response, response_len);

        free(cntrl_response);

        //printf("\t\nInit Response to controller sent\n");
        PrintRoutingTable();
        //printf("Create UDP Connection for routing table update\n");
        router_conn();

}
