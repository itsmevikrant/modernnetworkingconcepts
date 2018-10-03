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
#include "../include/update.h"
#define NUM_ROUTER_SIZE 2
#define UPDATE_INTERVAL_SIZE 2
#define ROUTER_UPDATE_SIZE 12

void PrintRoutingTable()
{
        uint16_t nr, rpR, rpD, ui, c, rID,rID1, cost;
        uint32_t rIP;
        int i,j,k;

        //  char heading[100] = "SourceRouterID  SourceIP  NeigbourROuterID  NeighbourIP    CostfromSource  router_port   DataPort  Isneighbour";
        //  printf("%s\n",heading);

        printf("%10s|%10s|%10s\n","ID","To","Cost");

        for (j=0; j<nR; j++) {
                printf("%10d|%10d|%10d\n",myinfo.myID,rtable[j].router_ID,rtable[j].cost);
                //printf("%s ID:%d IP:%2s RPr:%2d DPr:%2d","SourceROuter", myinfo.myID,ip_to_text(myinfo.myIP),  myinfo.myRport,myinfo.myDport);
                //printf("To:%d cost:%d\n",rtable[j].neighbour, rtable[j].cost );
                //printf("NID:%3d NIP: %2s NRpr:%d ",rtable[j].router_ID,ip_to_text(rtable[j].router_ip),rtable[j].router_port_R );


        }
}
