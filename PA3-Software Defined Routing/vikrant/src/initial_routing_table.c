
#include <string.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <strings.h>
#include <sys/queue.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include "../include/global.h"
#include "../include/control_header_lib.h"
#include "../include/network_util.h"
#include "../include/author.h"
#include "../include/init_response.h"
#include "../include/connection_manager.h"
#include "../include/control_handler.h"
#include "../include/initial_routing_table.h"
#include "../include/PrintRoutingTable.h"

#define ROUTING_TABLE_UPDATE_SIZE 8
#define INF 65535



void initial_routing_table(int sock_index)
{
        uint16_t pad = 00;
        //printf("CREATING RESPONSE TO CONTROLLER FOR ROUTING TABLE CONTROL REQUEST\n");
        uint16_t payload_len, response_len;
        char *cntrl_response_header, *cntrl_response_payload, *cntrl_response, *pl;
        uint16_t nr, rpR, rpD, ui, c, rID,rID1, cost;
        //payload_len = sizeof(8*nR);
        payload_len = 8*nR;
        //  printf("payload length %d\n", payload_len);
        cntrl_response_payload = (char *) malloc(payload_len);
        cntrl_response_header = create_response_header(sock_index, 2, 0, payload_len);
        response_len = CNTRL_RESP_HEADER_SIZE+payload_len;
        cntrl_response = (char *) malloc(response_len);
        memcpy(cntrl_response, cntrl_response_header, CNTRL_RESP_HEADER_SIZE);
        free(cntrl_response_header);


        for(j=0; j<nR; j++) {
                if (rtable[j].cost != INF)
                {
                        printf("\nrID %d\n", rtable[j].router_ID);
                        rID = htons(rtable[j].router_ID);
                        memcpy(pl +(j*ROUTING_TABLE_UPDATE_SIZE), &rID, 2);
                        //printf("routerID %d\n", rID);
                        memcpy(pl +(0x02)+(j*ROUTING_TABLE_UPDATE_SIZE),&pad, 2);
                        //printf("\nrID1 %d\n", rtable[j].router_ID);
                        printf("\nrouterID %d\n", rtable[j].next_hop_ID);
                        rID1 = htons(rtable[j].next_hop_ID);
                        memcpy(pl +(0x04)+(j*ROUTING_TABLE_UPDATE_SIZE), &rID1,2);

                        printf("\ncost %d\n", rtable[j].cost);
                        cost = htons(rtable[j].cost);
                        memcpy(pl +(0x06)+(j*ROUTING_TABLE_UPDATE_SIZE), &cost, 2);
                        //printf("nexthop ID %d cost %d \n", rID1, cost);
                }
                else if (rtable[j].cost == INF)
                {
                        rID = htons(rtable[j].router_ID);
                        memcpy(pl +(j*ROUTING_TABLE_UPDATE_SIZE), &rID, 2);
                        //  printf("routerID %d\n", rID);
                        memcpy(pl +(0x02)+(j*ROUTING_TABLE_UPDATE_SIZE),&pad, 2);
                        //  printf("\nrID1 %d\n", rtable[j].router_ID);
                        rID1 = htons(INF);
                        memcpy(pl +(0x04)+(j*ROUTING_TABLE_UPDATE_SIZE), &rID1,2);
                        //  printf("\nrouterID %d\n", rID1);
                        cost = htons(rtable[j].cost);
                        //  printf("\ncost %d\n", rtable[j].cost);
                        memcpy(pl +(0x06)+(j*ROUTING_TABLE_UPDATE_SIZE), &cost, 2);
                        //  printf("nexthop ID %d cost %d \n", rID1, cost);
                }
        }
        memcpy(cntrl_response_payload, pl, payload_len);
        PrintRoutingTable();

        //Copy Payload
        memcpy(cntrl_response+CNTRL_RESP_HEADER_SIZE, cntrl_response_payload, payload_len);
        free(cntrl_response_payload);

        sendALL(sock_index, cntrl_response, response_len);

        free(cntrl_response);
}
