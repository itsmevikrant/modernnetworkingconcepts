#include <stdio.h>
#include <stdlib.h>
# include <string.h>
#include "../include/simulator.h"

/* ******************************************************************
 ALTERNATING BIT AND GO-BACK-N NETWORK EMULATOR: VERSION 1.1  J.F.Kurose

   This code should be used for PA2, unidirectional or bidirectional
   data transfer protocols (from A to B. Bidirectional transfer of data
   is for extra credit and is not required).  Network properties:
   - one way network delay averages five time units (longer if there
     are other messages in the channel for GBN), but can be larger
   - packets can be corrupted (either the header or the data portion)
     or lost, according to user-defined probabilities
   - packets will be delivered in the order in which they were sent
     (although some can be lost).
**********************************************************************/

#define BIDIRECTIONAL 0    /* change to 1 if you're doing extra credit */
                           /* and write a routine called B_output */

/********* STUDENTS WRITE THE NEXT SEVEN ROUTINES *********/
//#define
/* common defines */
#define   A    0
#define   B    1
/* timeout for the timer */
#define TIMEOUT 20.0
#define strcpy(dest, src)
int seqnum = 0; //seqnum = 0, sn
int acknum = 0; //acknum = 0,an
int xpectedchecksum = 0;//, xpectedchksum
int xpectedseqnumA = 1;
int xpectedseqnumB = 0; //, xpectedseqnum =0
int csns = 0;// corectsnsent = 0
int isWait = 0;
int c ;//count,c
int i;
int maxbuff = 2000;
struct pkt p;

struct pkt rp;
struct pkt ap;
int chksum(struct pkt);
struct buf
{
  struct pkt bp;
  int sent;// = 0;
  int ackd;// = 0;
};
struct buf b[2000];
//struct pkt copytobuff (struct buf b[c].bp, struct pkt p);
int chksum(struct pkt packet)
{ int i;
  int checksumx = 0;
  checksumx = packet.seqnum;
  checksumx += packet.acknum;
  for (i = 0; i < 20; i+=1)
  checksumx += packet.payload[i];
  return checksumx;
}

/*struct pkt copytobuff (struct buf b[c].bp, struct pkt p)
{
 
 
  return (b[c].bp);
}*/
int counter=-1;

void A_output(message)
struct msg message;
{
	counter++;
	if (c < maxbuff)
	{
		starttimer(A, TIMEOUT);
		struct pkt sp;
		xpectedseqnumA = 1 - xpectedseqnumA;
		sp.seqnum = xpectedseqnumA;
		sp.acknum = acknum;
		int i;
		for(i=0;i<20;i++)
			sp.payload[i] = message.data[i];
		sp.checksum = chksum(sp);
		if(isWait){
			b[counter].bp.seqnum = sp.seqnum;
			b[counter].bp.acknum = sp.acknum;
			b[counter].bp.checksum = sp.checksum;
			for (i=0; i<20;i++)
				b[counter].bp.payload[i] = sp.payload[i];
		}else{
			b[c].bp.seqnum = sp.seqnum;
			b[c].bp.acknum = sp.acknum;
			b[c].bp.checksum = sp.checksum;
			//strcpy(b[c].bp.payload, sp.payload);
			for (i=0; i<20;i++)
				b[c].bp.payload[i] = sp.payload[i];
			printf("\ncopied pkt %d to buff \n", c);
			printf("packet with seqnum = %d, buffered, \n",b[c].bp.seqnum);
			printf("message is : %s\n",b[c].bp.payload);
			tolayer3(A, b[c].bp);
			b[c].sent = 1;
			isWait = 1;
		}
		
		//if (seqnum == 0)
		//	seqnum = 1;
		//else
		//	seqnum = 0;
	}
	else
	{
		printf( "buffer is full");
		tolayer5(A, message.data);
	}
}

void A_input(ap)
struct pkt ap;
{ 
	//xpectedchecksum = 0;
	xpectedchecksum = chksum(ap);
	if (xpectedchecksum == ap.checksum && ap.acknum == xpectedseqnumA)
	{
		b[ap.seqnum].ackd = 1;
		printf ("packet with seqnum = %d, has been Acknowledged", ap.seqnum);
		stoptimer(A);
		isWait = 0;
		c++;
	}
	else
	{
		stoptimer(A);
		printf("corrupt pkt");
		tolayer3(A, b[c].bp);
		starttimer(A, TIMEOUT);
	}
}
/* called when A's timer goes off */
void A_timerinterrupt()
{
	//for (c = 0; c< maxbuff; c++)
	//{
	//	if (b[c].ackd == 0)
	//	{
		    printf("\n\n\n\nTimer interrupt\n\n\n");
			tolayer3(A, b[c].bp);
			starttimer(A, TIMEOUT);
	//	}
	//}
}
/* the following routine will be called once (only) before any other */
/* entity A routines are called. You can use it to do any initialization */
void A_init()
{
	c =0;
}
/* Note that with simplex transfer from a-to-B, there is no B_output() */

/* called from layer 3, when a packet arrives for layer 4 at B*/
void B_input(rp)
struct pkt rp;
{ 
	struct msg message;
	xpectedchecksum = 0;
	xpectedchecksum = chksum(rp);
	if (xpectedchecksum == rp.checksum && rp.seqnum == xpectedseqnumB)
	{  
		//csns = xpectedseqnum;
		//strcpy(message.data , rp.payload);
		for(i=0;i<20;i++)
			message.data[i] = rp.payload[i];
		tolayer5(B, message.data);
		xpectedseqnumB = 1-xpectedseqnumB;
		printf("\tmessage delivered to layer5\n");
		ap.acknum = rp.seqnum;
		for (i = 0; i < 20; i = i+1)
			ap.payload[i] = message.data[i];
		ap.checksum = chksum(ap);
		tolayer3(B, ap);
		//printf("packet with seqnum = %d sent to layer 3", ap.seqnum);
	}
	else
	{
		printf("\n\n\n\n%d , %d,  %d,  %d",xpectedchecksum,rp.checksum,rp.seqnum,xpectedseqnumB);
		ap.acknum = -1;
		for (i = 0; i < 20; i= i+1)
		ap.payload[i] = 0;
		ap.checksum = chksum(ap);
		tolayer3(B, ap);
	}
}
/* called from layer 3, when a packet arrives for layer 4 at B*/
void B_init()
{

}


