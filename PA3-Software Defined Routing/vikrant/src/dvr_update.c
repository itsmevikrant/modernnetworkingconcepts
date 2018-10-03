#include <string.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <strings.h>
#include <sys/queue.h>
#include <unistd.h>
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
#include "../include/dvr_update.h"
#include "../include/PrintRoutingTable.h"
#define NUM_ROUTER_SIZE 2
#define UPDATE_INTERVAL_SIZE 2
#define ROUTER_UPDATE_SIZE 0x0C//12

#define INF 65535 //1 less than maximum of 2^16 = 65536
//create a UDP connection to listen and send data to other routers
//send routing_update to all other routers
//initiate timer for each router routing_update
// function to send response of ROUTING_update


void send_routing_updates()
//create routing update pkt
{
        //uint16_t BUFSIZE, response_len;
        int BUFSIZE;
        BUFSIZE = 8+12*nR;
        //printf("Bufsize %d, nR %d\n", BUFSIZE,nR);
        //char *cntrl_response_header, *cntrl_response_payload, *cntrl_response, *buf,*pl;
        unsigned char *buf = (unsigned char *) malloc(sizeof(char)*BUFSIZE);
        printf("size of buf %s\n", buf);
        uint16_t nr, rpR, rpD, ui, c, rID,rID1, cost, mypR;
        uint32_t rIP, myIP;
        int p,l,j,m;
        uint16_t pad = 00;


        //  printf("payload length %d\n", payload_len);
        //buf = (char *) malloc(sizeof(BUFSIZE));

        printf("%d\n", nR);
        nr = htons(nR);
        memcpy(buf, &nr, 2 );
        mypR = htons(myinfo.myRport);
        memcpy(buf+(0x02),&mypR, 2);
        myIP = htonl(myinfo.myIP);
        //    printf(" my IP: %s\n",ip_to_text(myinfo.myIP));
        memcpy(buf+(0x04), &myIP, 4);
        for (j=0; j<nR; j++)
        {
                //printf("copy\n"); //rtable[j].neighbour = 1;
                //m = j*ROUTER_UPDATE_SIZE;
                //printf("m=%d\n", m);
                rIP = htonl(rtable[j].router_ip);
                memcpy(buf+(0x08) + (j*ROUTER_UPDATE_SIZE), &rIP,4);
                //  printf("IP is = %s\n",ip_to_text(rtable[j].router_ip));
                rpR = htons(rtable[j].router_port_R);//     = routerinfo[j].router_port_R;
                memcpy(buf + (0x0C)+(j*ROUTER_UPDATE_SIZE), &rpR, 2);
                memcpy(buf +(0x0E)+(j*ROUTER_UPDATE_SIZE),&pad, 2);
                //printf("buf = %lu",sizeof(buf));
                rID1 = htons(rtable[j].router_ID);
                memcpy(buf +(0x10)+(j*ROUTER_UPDATE_SIZE),&rID1,2);
                //printf("buf = %lu",sizeof(buf));
                cost = htons(rtable[j].cost);
                memcpy(buf +(0x12)+(j*ROUTER_UPDATE_SIZE),&cost, 2);
                //printf("buf = %lu",sizeof(buf));

        }
        //printf("no. of bytes in buf is %lu\n", sizeof(buf));
        int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
        //memcpy( buf, buf, BUFSIZE);
        for (l=0; l<nR; l++)
        {int portno, n;
         int serverlen;
         struct sockaddr_in rserveraddr;
         //printf("WHWH");

         if(rtable[l].neighbour==1)
         { //printf("\nROuter %d is Nbur\n",l);
                 rtable[l].update_counter = rtable[l].update_counter - 1;
                 /* if (rtable[j].update_counter == 0)
                     {
                            printf("\n49.Neighbour-->%d has not sent updates since last %d intervals, MAKE COST INFINITE\n", l, rtable[l].update_counter);
                            rtable[l].cost = INF;
                            continue;
                     }*/
                 if (rtable[l].cost != 65535 && rtable[l].cost!=0)
                 {
                         //printf("\nROuter --> %d is a neighbour and SEND UPDATES\n", l);

                         if (sockfd < 0)
                                 printf("51.ERROR opening socket");
                         //printf("\n\tSOCKET [%d] OPENED\n", sockfd);
                         bzero((char *) &rserveraddr, sizeof(rserveraddr));
                         rserveraddr.sin_family = AF_INET;
                         portno = rtable[l].router_port_R;
                         rserveraddr.sin_port = htons(portno);
                         rserveraddr.sin_addr.s_addr= htonl(rtable[l].router_ip);
                         //printf("IP Address is %s, Port No R: %d", ip_to_text(rtable[l].router_ip),rtable[l].router_port_R);

                         serverlen = sizeof(rserveraddr);
                         n = sendto(sockfd, buf, BUFSIZE, 0, (struct sockaddr *)&rserveraddr, serverlen);
                         if (n < 0)
                         {printf("n %d",n);
                          exit(1);}
                         //else
                         //   printf("Number of Bytes  %d sent to router %d\n",n,l);
                 }
         }}
}

void take_routing_info(char *recvbuf)
{

        /*  //printf("\n55.Receive ROuting UPDATE on socket %d\n",router_socket);
           //https://www.cs.cmu.edu/afs/cs/academic/class/15213-f99/www/class26/udpserver.c
           //sleep(5);
           uint16_t nr, rpR, rpD, ui, c, rID,rID1, cost, mypR, num_routers, source_rport, routecost;
           uint32_t rIP,source_ip;
           int p,l;
           int isSource1 = 0, sourceRouter;

           ///  printf("\n********************************************\n");
           //CALC_DVR_SEND_UPDATE(n);

           printf("\n58. Update ROuting TABLE BASED on Info from Neighbour Update\n");

           memcpy(&nr, recvbuf, 2);
           nR = ntohs(nr);
           printf("\n59. NO. of ROUTERS in Received PAYLOAD (Routing Table Update) %d\n", nR);

           memcpy(&rpR, recvbuf+0x02, 2);
           source_rport = ntohs(rpR);
           printf("\n60.  Port of ROuter from which Received PAYLOAD (Routing Table Update)%d \n", source_rport);

           memcpy(&rIP, recvbuf+0x04, 4);
           source_ip =rIP;
           printf("\n61.  IP of ROuter from which Received PAYLOAD (Routing Table Update)is %s \n", ip_to_text(source_ip));


           for (p=0; p<num_routers; p++)
           {printf("enter\n");
           if (rtable[p].router_port_R == source_rport)
           { printf("enter2\n");
             printf("62.Routing Update Recd. from Neighbour %d with IP %d\n", rtable[p].router_port_R, source_ip);
             isSource1 = 1;
             sourceRouter = rtable[p].router_ID;
             printf("\n63. Set Update Counter for router--> %d\n", p);
             rtable[p].update_counter =3;
             break;}
           for (l = 0; l<num_routers; l++)
           {printf("enter3\n");
            memcpy(&routecost, recvbuf+0x012 + (l* (0x0C)), 2);
            cost_from_to[p][l] = ntohs(routecost);
            printf("\n64. Cost of Route from Router-->%d to Router-->%d is:: %d\n",p,l,cost_from_to[p][l]);}}

           for (p=0; p<num_routers; p++)
           { //rtable[p].update_counter =3;
                  printf("enter4\n");
                  routecost = cost_from_to[sourceRouter][p]+ncost.cost_from_to[myinfo.myID][sourceRouter];
                  printf("\n65. New Cost of ROute from MySelf-->%d to ROuter-->%d via Neighbour--> %d is:: %d\n",myinfo.myID, p, sourceRouter, routecost);
                  if (routecost < ncost.cost_from_to[myinfo.myID][p])
                  {printf("enter5\n");
                   ncost.cost_from_to[myinfo.myID][p] = routecost;
                   rtable[p].cost = routecost;
                   rtable[p].next_hop_ID = sourceRouter;       //rtable[p].router_ID;
                   printf("\n66. ------UPDATE ROUTING TABLE---------\n");
                   printf("\n67. RouteCost to router--> %d via Neighbour %d is %d",p,  rtable[p].next_hop_ID, rtable[p].cost);}
           }
           //send_routing_updates();*/
}
