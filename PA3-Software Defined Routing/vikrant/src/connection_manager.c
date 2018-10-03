/**

 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details at
 * http://www.gnu.org/copyleft/gpl.html
 *
 * @section DESCRIPTION
 *
 * Connection Manager listens for incoming connections/messages from the
 * controller and other routers and calls the desginated handlers.
 */

 #include <sys/select.h>
 #include <netinet/in.h>
 #include <strings.h>
 #include <sys/queue.h>
 #include <unistd.h>
 #include <string.h>
 #include <time.h>
 #include <netdb.h>
 #include "../include/connection_manager.h"
 #include "../include/global.h"
 #include "../include/control_handler.h"
 #include "../include/initial_routing_table.h"
 #include "../include/dvr_update.h"
 #include "../include/init_response.h"
 #include "../include/PrintRoutingTable.h"
 #include "../include/update.h"

//fd_set master_list, watch_list;
//int head_fd;
time_t lasttime;
void main_loop()
{
        int selret, sock_index, fdaccept;

        printf("LastTime 1= %ld\n",lasttime);
        while(TRUE)
        {

                watch_list = master_list;
                if(whats_timer==0) {
                        printf("TIMER FLAG 0");
                        selret = select(head_fd+1, &watch_list, NULL, NULL, NULL);
                } else {
                        //  printf("TIMER FLAG 1");
                        int second_interval = uI - (time(NULL) - lasttime);
                        printf("\n\t\tINTERVAL is %d\n", second_interval);
                        tv.tv_sec = second_interval;
                        tv.tv_usec = 0;
                        selret = select(head_fd+1, &watch_list, NULL, NULL, &tv);
                }


                if(selret < 0)
                {  ERROR("select failed.");}
                else if(selret == 0)
                {
                        printf("**********\n");
                        printf("sending updates\n");
                        printf("**********\n");
                        send_routing_updates();
                        lasttime = time(NULL);
                }
                else
                {
                        /* Loop through file descriptors to check which ones are ready */
                        for(sock_index=0; sock_index<=head_fd; sock_index+=1) {

                                if(FD_ISSET(sock_index, &watch_list)) {

                                        /* control_socket */
                                        if(sock_index == control_socket) {
                                                fdaccept = new_control_conn(sock_index);

                                                /* Add to watched socket list */
                                                FD_SET(fdaccept, &master_list);
                                                if(fdaccept > head_fd) head_fd = fdaccept;
                                        }

                                        /* router_socket */
                                        else if(sock_index == router_socket) {
                                                uint16_t nr,nRT, rpR, rpD, ui, c, rID,rID1, cost, mypR, num_routers, source_rport, routecost, routecost1;
                                                uint32_t rIP,source_ip,rIP1;
                                                int l,k,j,p;
                                                int isSource1 = 0, sourceRouter;
                                                printf("Hey ROUTER SOCKET RECD\n");
                                                int BUFSIZE, response_len;
                                                BUFSIZE = 8+12*nR;
                                                //  printf("\nrecieved router socket %d\n", sock_index);

                                                char *recvbuf = ( char *) malloc(sizeof(char)*BUFSIZE);
                                                //  char *cntrl_response_header, *cntrl_response_payload, *cntrl_response, *recvbuf;
                                                //  recvbuf = (char *) malloc(BUFSIZE);

                                                struct sockaddr_in clientaddr;
                                                int n;
                                                int clientlen;
                                                clientlen = sizeof(clientaddr);

                                                bzero(recvbuf, BUFSIZE);
                                                n = recvfrom(sock_index, recvbuf, BUFSIZE, 0,(struct sockaddr *) &clientaddr, (socklen_t *)&clientlen);
                                                if (n < 0)
                                                {printf("n-->%d", n);
                                                 exit(1);}

                                                printf("Reading this many bytes: %d\n", n);
                                                memcpy(&nr, recvbuf, 2);
                                                nRT = ntohs(nr);
                                                //printf("\n59. NO. of ROUTERS in Received PAYLOAD (Routing Table Update) %d\n", nRT);
                                                memcpy(&rpR, recvbuf+(0x02), 2);
                                                source_rport = ntohs(rpR);
                                                //printf("\n59. Port No. of Source ROuter in Received PAYLOAD (Routing Table Update) %d\n", source_rport);
                                                memcpy(&rIP, recvbuf+(0x04), 4);
                                                source_ip =ntohl(rIP);
                                                //printf("\n61.  IP of ROuter from which Received PAYLOAD (Routing Table Update)is %s \n", ip_to_text(source_ip));
                                                int index_neighbor=0;
                                                int cost_neighbour = 0;
                                                for (p=0; p<nRT; p++)
                                                {
                                                        if (rtable[p].router_ip == source_ip)
                                                        {
                                                                //printf("62.Routing Update Recd. from Neighbour %d with IP %s\n", rtable[p].router_ID, ip_to_text(source_ip));
                                                                isSource1 = 1;
                                                                sourceRouter = rtable[p].router_ID;
                                                                index_neighbor = p;
                                                                rtable[p].update_counter =3;
                                                                cost_neighbour = rtable[p].cost;
                                                                break;
                                                        }
                                                }
                                                printf("Receive update from [%d] index %d\n", sourceRouter,index_neighbor);
                                                //
                                                //printf("Cost from Me[%d] to [%d]  is [%d]", myinfo.myID, )

                                                for (l=0; l<nRT; l++)
                                                {
                                                        memcpy(&rID1, recvbuf+0x010 + (l* (0x0C)), 2);
                                                        int destination = ntohs(rID1);
                                                        if (destination == rtable[l].router_ID)
                                                        //memcpy(&rIP1, recvbuf+0x08 + (l* (0x0C)), 2);
                                                        {

                                                                //printf(" Me[%d] to dest:[%d]  is [%d]\n\n", myinfo.myID, destination, rtable[l].cost );
                                                                //printf(" Me[%d] to Nbur:[%d] is [%d]\n\n",myinfo.myID, sourceRouter, cost_neighbour );
                                                                memcpy(&routecost, recvbuf+0x012 + (l* (0x0C)), 2);
                                                                cost_from_to[index_neighbor][l] = ntohs(routecost);
                                                                //printf(" cost from Nbur:[%d]to Destn:[%d]  is [%d]\n\n",sourceRouter,destination,cost_from_to[index_neighbor][l]);
                                                                int newCost = cost_neighbour+cost_from_to[index_neighbor][l];
                                                                //printf("%d is newCost\n\n", newCost);
                                                                if( newCost< rtable[l].cost) {
                                                                        printf("%d to %d %d\n",sourceRouter, destination, newCost);
                                                                        rtable[l].cost = newCost;
                                                                        rtable[l].next_hop_ID = sourceRouter;
                                                                        //printf("Cost from Me[%d] to [%d] is %d",myinfo.myID, destination, rtable[l].cost );
                                                                }

                                                        }
                                                }
                                                PrintRoutingTable();
                                        }


                                        /* data_socket */
                                        else if(sock_index == data_socket) {
                                                //new_data_conn(sock_index);
                                        }

                                        /* Existing connection */
                                        else{
                                                if(isControl(sock_index)) {
                                                        if(!control_recv_hook(sock_index)) FD_CLR(sock_index, &master_list);
                                                }
                                                //else if isData(sock_index);
                                                else ERROR("Unknown socket index");
                                        }
                                }
                        }
                }
        }

}
void init()
{
        control_socket = create_control_sock();

        //router_socket and data_socket will be initialized after INIT from controller

        FD_ZERO(&master_list);
        FD_ZERO(&watch_list);

        /* Register the control socket */
        FD_SET(control_socket, &master_list);
        head_fd = control_socket;

        main_loop();
}
void router_conn()
{
        whats_timer=1;

        lasttime = time(NULL);
        printf("LastTime 2= %ld\n",lasttime);
        //  printf("Initiate ROuterConnection\n");
        router_socket = create_router_sock();
        printf("\n75. ROUTER UDP CONNECTION UP AT PORT  --> %d, router socket %d\n", myinfo.myRport, router_socket);


        /* Register the control socket */

        FD_SET(router_socket, &master_list);
        if(router_socket>head_fd) {
                head_fd = router_socket;
        }

        //send_routing_updates();
}
