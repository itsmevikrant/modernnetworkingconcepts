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

#define BIDIRECTIONAL 0    

/********* STUDENTS WRITE THE NEXT SEVEN ROUTINES *********/
//#define
/* common defines */
#define   A    0

#define   B    1
/* timeout for the timer */
#define TIMEOUT 50.0
#define strcpy(dest, src)
int seqnum = 0; //seqnum = 0, sn
int acknum = 0; //acknum = 0,an
int xpectedchecksum = 0;//, xpectedchksum
int xpectedseqnumA = 0;
int xpectedseqnumB = 0; //, xpectedseqnum =0
int csns = 0;// corectsnsent = 0
int isWait = 0;
int c, count,cnt ;//count,c
int i;
int maxbuff = 2000;
int base = 0;
int windowsize = 10 ;
struct pkt p;
struct pkt lastackpkt;
struct pkt rp;
struct pkt ap;
struct pkt sp;
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


void A_output(message)
struct msg message;
{
//struct pkt sp
	if (xpectedseqnumA < base + windowsize)
			
	{ printf("xpetedseqnumA is %d ok \n",xpectedseqnumA);
			
		sp.seqnum = xpectedseqnumA;
		sp.acknum = acknum;
	
		for(i=0;i<20;i++)
			sp.payload[i] = message.data[i];
		sp.checksum = chksum(sp);
		b[c].bp.seqnum = sp.seqnum;
		b[c].bp.acknum = sp.acknum;
		b[c].bp.checksum = sp.checksum;
		//strcpy(b[c].bp.payload, sp.payload);
		for (i=0; i<20;i++)
			b[c].bp.payload[i] = sp.payload[i];
		printf("\ncopied pkt %d with seqnum = %d,to buff as Seqnum is LESS than base + windowsize %d\n", c, b[c].bp.seqnum, base+windowsize);
	//	printf("packet with seqnum = %d, buffered, \n",b[c].bp.seqnum);
		tolayer3(A, b[c].bp);
	    	b[c].sent = 1;
		printf("packet IF %d with message %s sent\n", b[c].bp.seqnum, b[c].bp.payload);
	
	if (base == xpectedseqnumA)
	    {
	    printf ("BASE: %d = SEQNUM: %d", base , xpectedseqnumA);
	    starttimer(A, TIMEOUT);
	    printf("Timer started for SEQNUM:%d base %d\n", xpectedseqnumA, base);
	   }
	else
	{  tolayer3(A, b[c].bp);
         printf("packet ELSE %d with message %s sent\n", b[c].bp.seqnum, b[c].bp.payload);	
	    	b[c].sent = 1;
  	printf ("New BASE: %d = SEQNUM: %d", base , xpectedseqnumA);
	    starttimer(A, TIMEOUT);
	    printf("Timer started for SEQNUM:%d \n base %d\n", xpectedseqnumA, base);
	}  
	
	 xpectedseqnumA++;
	    c++;	
printf("next seqnum %d and buffer count %d base %d",xpectedseqnumA,c, base);
//isWait = 1;
//if (seqnum == 0)
		//	seqnum = 1;
    	//else
		//	seqnum = 0;
	}
	else if (c < maxbuff)
	{		

sp.seqnum = c;
printf("\nso SeqnumA %d is bigger than base + windowsize. packet %d stored in buff\n", sp.seqnum,c);
			sp.acknum = acknum;

			b[c].bp.seqnum = sp.seqnum;
			b[c].bp.acknum = sp.acknum;
			b[c].bp.checksum = sp.checksum;
			//strcpy(b[c].bp.payload, sp.payload);
			for (i=0; i<20;i++)
				b[c].bp.payload[i] = sp.payload[i];
			printf("\ncopied pkt %d to buff \n", c);
			printf("packet with seqnum = %d, buffered, \n",b[c].bp.seqnum);
			printf("message is : %s\n",b[c].bp.payload);
			//tolayer3(A, b[c].bp);
			//b[c].sent = 1;
			c++;
	    		xpectedseqnumA++;
			//isWait = 1;

	}
else {
		printf( "SEND LATER, BUSY RN\n");
		tolayer5(A, b[c].bp.payload);
	}
}

void A_input(ap)
struct pkt ap;
{ 
	//xpectedchecksum = 0;
	xpectedchecksum = chksum(ap);
	//stoptimer(A);
	if (xpectedchecksum == ap.checksum)
	{
	//&& ap.acknum == xpectedseqnumA)
		base = ap.acknum + 1;
printf("BASE is %d\n", base);
		if (base == xpectedseqnumA)
		{stoptimer(A);
		printf("ACK = %d with message = %s recd\n", ap.acknum, ap.payload);
		printf("stopping timer for packet %d\n", ap.acknum);
	}

		else 
		starttimer(A, TIMEOUT);
		//= 1 - xpectedseqnumA;
		b[ap.acknum].ackd = 1;


}
	else 
	{
		printf("corrupt ACK");
	//	tolayer3(A, b[c].bp);
	//	starttimer(A, TIMEOUT);
	}
}
/* called when A's timer goes off */
void A_timerinterrupt()
{
	printf("TIMER INTERRUPT is Called :(\n");
	printf("xpectedseqnumA is %d\n",xpectedseqnumA)   ;
	cnt = 1;
	if (cnt)
	{	
		tolayer3(A, b[cnt-1].bp);
		printf("\nSeding packet message is : %s\n",b[cnt-1].bp.payload);
		printf("sending pkt 1");
		starttimer(A, TIMEOUT);
	}

	else
	{	//	
		starttimer(A, TIMEOUT);
		count = base;
	//while (count < xpectedseqnumA -1)
		for (count = base; count < xpectedseqnumA-1; count++)	
		{
	//if (b[c].ackd == 0)
	
	//	{
	
			printf("resending xpectedseqnum %d\n",count);
			tolayer3(A, b[count].bp);
			c++;
	//		starttimer(A, TIMEOUT);
		}
	}
}

/* the following routine will be called once (only) before any other */
/* entity A routines are called. You can use it to do any initialization */
void A_init()
{
	int c =0;
}
/* Note that with simplex transfer from a-to-B, there is no B_output() */

/* called from layer 3, when a packet arrives for layer 4 at B*/
void B_input(rp)
struct pkt rp;
{ printf("packet recd at B\n");
    printf("Message is : %s\n",rp.payload);
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
		xpectedseqnumB = 1+ xpectedseqnumB;
		printf("\tmessage delivered to layer5\n");
		//ap.seqnum = rp.seqnum;
		ap.acknum = rp.seqnum;
		for (i = 0; i < 20; i = i+1)
			ap.payload[i] = rp.payload[i] ;
		ap.checksum = chksum(ap);
		tolayer3(B, ap);
		lastackpkt = ap;
		printf("ACK = %d with message %s SENT to A\n", ap.acknum,ap.payload);
	}
	else if (xpectedchecksum != rp.checksum || rp.seqnum != xpectedseqnumB)
	{
		//if (rp.seqnum= 0)
		//	ap.seqnum= 1;
		//else
		//	ap.seqnum= 0;
		//ap.acknum = lastackpkt;
		//for (i = 0; i < 20; i= i+1)
		//ap.payload[i] = 0;
		//ap.checksum = chksum(ap);
		tolayer3(B, lastackpkt);
	}
}
/* called from layer 3, when a packet arrives for layer 4 at B*/
void B_init()
{
printf("starting B");
}


