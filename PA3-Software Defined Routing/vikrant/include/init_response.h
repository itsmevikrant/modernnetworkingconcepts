#ifndef INIT_RESPONSE_H_
#define INIT_RESPONSE_H_

void init_response(int sock_index, char *cntrl_payload);
//void take_routing_info();
//void send_routing_updates();
char* ip_to_text(uint32_t ip_address);
//void initial_routing_table(int sock_index, char *cntrl_payload);
uint16_t nR, uI;
//typedef unsigned char routerinfo;
//typedef unsigned char ip_v4;
typedef struct ip
{
  unsigned char s1, s2, s3, s4;
  //unsigned int uip1, uip2;
}IP;
IP ip_v4;
typedef struct ROUTING_INIT_ROUTER
{
    uint16_t router_ID ;
    uint16_t router_port_R;
    uint16_t router_port_D;
    uint16_t cost;
    uint32_t rIP;
    uint32_t router_ip_addr;
    int neighbour;// = 0;
    //int host;
    //uint32_t router_next_hop;
} ROUTING_INFO_FROM_CONTROLLER;
ROUTING_INFO_FROM_CONTROLLER routerinfo[5];
ROUTING_INFO_FROM_CONTROLLER routerinit;
typedef struct self
{
uint16_t myID;
uint32_t myIP;
//int my_next_hop;
uint16_t myRport;
uint16_t myDport;
uint16_t myCport;
uint16_t my_next_hop_ID;
//uint16_t cost;
} self;
struct self myinfo;

typedef struct routingtable
{
uint16_t router_ID ;
uint16_t router_port_R;
//uint32_t source_ip;
uint32_t router_ip;
uint32_t next_hop_ID;
//int neighbor_status;
uint16_t cost;
int neighbour, update_counter;
//uint16_t router_port;

}routingtable;
struct routingtable rtable[10];



typedef struct distancecost
{
  //int i,j;
  //uint16_t router_ID;
  int cost_from_to[5][5];
}dcost;
dcost ncost;

/*typedef struct ROUTING_INIT_HEADER
  {
      uint16_t num_routers;
      uint16_t update_interval;
  }ROUTING_INIT_HEADER;
struct ROUTING_INIT_HEADER gvofR;*/


//uint16_t cost_from_rupdate[i][j]
int i,j,p,l,m,n,k,o;
int cost_from_to[5][5];
#endif
