
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>
#include <ctype.h>
#include <time.h>
#include "../include/simulator.h"


#define TIMEOUT 20 
int maxbuff = 2000;
struct buffer
{
	struct pkt pkt;
	int ackd; // 0 if Not, 1 if Yes
	int sent; // 0 if Not, 1 if Yes
    //int starttime;
    float time;

};
struct buffer sbuff[2000];

struct buff
{
	struct pkt rpkt;
	int pktbufd;
};
struct buff rbuff[2000];

#define A 0
#define B 1
int base = 0;
int w = 10,i,pktsent ;
int xc =0;
int xs=0 ; //seqnum
int xa =0 ;//acknum
int count = 0;
int countB=0;
int rs;
int currtime;
int countz=0;
int seqnum = 0;
int acknum = 0;
int xpectedseqnumA = 0;
int xpectedseqnumB = 0;
int bufmsg = 0;

struct pkt ackpkt;
struct pkt spkt;
int RB = 0;
int N = 10;
/*_________________________________________*//* compute checksum */

//CALCULATE CHECKSUM
int checksum(struct pkt pktfrmAorB)
  {
  int  i ;
  //1st checksum
 int checksumx =  pktfrmAorB.seqnum + pktfrmAorB.acknum;
    	//sum += packet.acknum;
  for (i = 0; i < 20; i += 1)
    {  checksumx += (unsigned char)pktfrmAorB.payload[i];
      }
   printf("checksum = %d\n", checksumx);
    return checksumx;
 }
//
/*int diff(int time1, int time2)
{
int temp;
temp = time1 - time2;
return temp;
}*/


void A_output(message)
struct msg message;
{
printf("A_OUTPUT STARTED_____________");
  //Recd from L5
  //assign seqnum etc.

printf("packet spkt created with seqnum %d\n",spkt.seqnum);
//check
  if (xpectedseqnumA >= base && xpectedseqnumA < base + w)
    {  spkt.seqnum = seqnum;
     spkt.acknum = acknum;

    for(i=0;i<20;i++)
	spkt.payload[i] = message.data[i];
    spkt.checksum = checksum(spkt);
		printf("\nSeqNum - %d is within the range of base to base+window [%d,%d] \n\n",seqnum,base,base+w);
		sbuff[count].pkt.seqnum = spkt.seqnum;
        sbuff[count].pkt.acknum = spkt.acknum;
        sbuff[count].pkt.checksum = spkt.checksum;
        for(i=0;i<20;i++)
			sbuff[count].pkt.payload[i] = spkt.payload[i];
		printf("ADDED TO BUFFER: packet added to buffer SBUFF at %d  with seqnum %d\n",count, sbuff[count].pkt.seqnum);
    	printf("SENT TO LAYER 3: packet with data <%s> and seqnum %d \n", sbuff[count].pkt.payload, sbuff[count].pkt.seqnum);
        sbuff[count].sent = 1;
    	starttimer(A,TIMEOUT);
		sbuff[count].time=get_sim_time()+TIMEOUT;
	tolayer3(A, sbuff[count].pkt) ;
	printf("SENT TIME to B: packet %d sent at time %f to B \n \n", sbuff[count].pkt.seqnum, sbuff[count].time);
        count++;
        seqnum++;
        xpectedseqnumA++;
        return;
      }
      else //Add to buffer

      {
           sbuff[count].pkt.seqnum = spkt.seqnum;
           sbuff[count].pkt.acknum = spkt.acknum;
           sbuff[count].pkt.checksum = spkt.checksum;
             for(i=0;i<20;i++)
			sbuff[count].pkt.payload[i] = spkt.payload[i];
	   printf("ADDED TO BUFFER:  _____seqnum >base+w___ Packet spkt added to buffer SBUFF at %d  with seqnum %d\n",count, sbuff[count].pkt.seqnum);
           sbuff[count].sent = 0;
           count++;
           seqnum++;

      }


printf(" NO. OF PKTS IN BUFFER-A: %d \n", count);
}

void A_input(packet)
struct pkt packet;
{
	printf("\nA_input Ack number recieved with acknum %d",packet.acknum);
	xc=checksum(packet);
	if(xc==packet.checksum)
	{
		printf("\nA_input Chesum is equal\n");
		if(packet.acknum>-1 && packet.acknum<2000)
		{
			sbuff[packet.acknum].ackd=1;

		   //sbuff[packet.acknum].time=get_sim_time()+TIMEOUT;
			printf("\nA_input Stopping A's timer for packet with seq num %d\n",packet.acknum);
			stoptimer(A);
		}

	   // int cnt;
		if(packet.acknum==base)
		{
			printf("\n A_input ackNumber is equal  to base \n");

			for(i=base;i<base+w;i++)
			{
				if(sbuff[i].ackd==-1)
					break;
				countz++;
			}
			printf("\n Moving sender window %d positions \n",countz);
			printf("\nCurrent base position %d \n",base);
			base=base+countz;//{

		}


        printf("A_input seqnum %d, base %d, w %d\n", seqnum, base, w);
		if (xpectedseqnumA < base+w)
		{
			if (base = xpectedseqnumA)
				{
				    starttimer(A, TIMEOUT);

				}
			// printf("xpectedseqnumA = %d\n", xpectedseqnumA);
		   else
			{  for (i = base; i < xpectedseqnumA; i++)
			{if(sbuff[i].sent!=1)
		   	{ tolayer3(A, sbuff[i].pkt);
			printf("\n A_input sending packet with seqnum %d \n ", i);

			    sbuff[i].time=get_sim_time()+TIMEOUT;
			}
			}
			}
			xpectedseqnumA++;
		}



	}

	else
	{
		printf("\n A_input  Packet corrupted with acknum %d \n" ,packet.acknum);
		printf("\nSo ignoring the packet\n");
	}

}

void A_timerinterrupt()
{printf("A_TIMERINTERRUPT STARTED_____________%d\n",base);
starttimer(A, 5.0);
	printf("\n A_TIMERINTERRUPT A's timer interrupt call \n");
	printf("\n A_TIMERINTERRUPT Traversing A's sender window and transmitting unacknowledged packets with window from [%d--%d]\n",base,base+w-1);
	for(i=0;i<xpectedseqnumA-base;i++)
	{
		currtime = get_sim_time();
		printf("\n-------------------------------------------\n");

		if(sbuff[i].sent==1 && (currtime>sbuff[i].time))//timeout[i].rtt))
		{
				stoptimer(A);
				printf("\nA_TIMERINTERRUPT Not yet acknowledged \n");
				printf("\nA_TIMERINTERRUPT Retransmitting the packet with seqnum %d\n",i);
				printf("\nA_TIMERINTERRUPT Data at Sender is : %s\n",sbuff[i].pkt.payload);
				tolayer3(A,sbuff[i].pkt);
				printf("\nA_TIMERINTERRUPT Packet sent\n");
				sbuff[i].sent=1;
				starttimer(A,TIMEOUT);

			      sbuff[i].time=get_sim_time()+TIMEOUT;
			}
		}

	}



void A_init()
{
	int i;
	for(i=0;i<2000;i++){
		rbuff[i].pktbufd=-1;
		sbuff[i].ackd=-1;
	}
}

void B_input(rcvpkt)
struct pkt rcvpkt;
{
  xc = checksum(rcvpkt);
  if (xc == rcvpkt.checksum)// check if rcvpkt corrupt or not
  { 	rs = rcvpkt.seqnum;
		printf("\nB_input packet with seqnum %d recieved from A\n", rs);
		if (rs>=RB && rs <RB+N-1)
        {
			printf("\n B_input Received seqnum-%d is within the range of RB to RB+N [%d,%d]",rs,RB,RB+N);
			countB = rs;
			printf("\n B_input Count : %d\n\n ",countB);
			if(rbuff[countB].pktbufd == -1){

			  rbuff[countB].rpkt = rcvpkt;
			  for(i=0;i<20;i++)
				rbuff[countB].rpkt.payload[i] = rcvpkt.payload[i];
			    rbuff[countB].rpkt.checksum = checksum(rbuff[countB].rpkt);
				rbuff[countB].pktbufd = 1;
				printf("\nB_input Received packet is buffered\n");
				ackpkt = rbuff[countB].rpkt;
			ackpkt.acknum = rbuff[countB].rpkt.seqnum;

			ackpkt.checksum = checksum(ackpkt);
            tolayer3(B, ackpkt) ; //send ack to a
			printf("B_input ACK %d sent to layer 3 to A, next expected packet with SEQNUM %d, checksum %d",ackpkt.acknum, ackpkt.seqnum+1,ackpkt.checksum);
			}
			if(rs == RB){

				printf("\nB_input base and seqNUm equal\nSending packets to upperlayer\n");
				int j;
				for(j=RB;j<RB+w;j++){
					if(rbuff[j].pktbufd == -1){
						break;
					}else{
						printf("\nB_input packet %d sent to upperlayer\n",j);
						tolayer5(B,rbuff[j].rpkt.payload);
					}

				}
				printf("\nMoviing receiver window %d steps\n",j-RB);
				RB = RB+(j-RB);


			}
		xpectedseqnumB++;//change
		}
	    else if( (rs >= (xpectedseqnumB-N)) && (rs<xpectedseqnumB) ){//change
			ackpkt.acknum = rbuff[countB].rpkt.acknum;

			ackpkt.seqnum = rbuff[countB].rpkt.seqnum;
			for(i=0;i<20;i++)
				ackpkt.payload[i] = rbuff[countB].rpkt.payload[i];
			ackpkt.checksum = checksum(rcvpkt);
			tolayer3(B,ackpkt);
			printf("B_input ACK %d sent to layer 3 to A, next expected packet with SEQNUM %d",ackpkt.acknum, ackpkt.seqnum);

		}
	}else{

		printf("\n\nPacket is corrupt\n\n");
	}
}

void B_init()
{

}
