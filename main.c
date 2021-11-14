#include <sys/socket.h>
//#include <netinet/if_ether.h>
//#include <net/ethernet.h> //链路层
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <linux/if_ether.h>
#include <linux/if_packet.h>


// socket bind port (recv)

int socket_init()
{
   struct ifreq ifr;
   int sock;
   memset(&ifr, 0x0, sizeof(struct ifreq));

   if((sock = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL))) < 0)
   {
       perror(" socket ");
       exit(0);
   }
   	//设置  eth0 网卡 的 flag
   strcpy(ifr.ifr_name, "eth0");
   if(ioctl(sock, SIOCGIFFLAGS, &ifr) == -1)
   {
       perror("siocgifflags");
       exit(0);
   }

   
   ifr.ifr_flags |= IFF_PROMISC;
   if(ioctl(sock, SIOCSIFFLAGS, &ifr) == -1)
   {
       perror("fejwaio?");
       exit(0);
   }
   return sock;
}


void print_packet(unsigned char *buff,int len)
{

         if(NULL== buff)
         {
           return ;
         }
		 int i=0;
         for(i=0;i<len;i++)
		    printf("%x",buff[i]);
		 
		 printf("\n");
		 return ;

}



int main(void)
{
	
    unsigned   char	buffer [ 65536 ] ;
	struct  sockaddr_ll  in_addr ;
	struct  sockaddr_ll  dest ;
	//struct sockaddr in_addr ;
	int sendfd;
    int socklen = 0;
	ssize_t recv_len = 0;
	int n = 0;
	struct ifreq ifr;
	struct packet_mreq mr;

	
    printf("Now I'm going to make a bridge for vm taps\n");


    int sock = socket_init();
	socklen = sizeof(struct  sockaddr_ll);
    sendfd = socket(PF_PACKET,SOCK_RAW,htons(ETH_P_ALL) ) ;
    strcpy ( ifr.ifr_name , "eth0" ) ;
   	//获取eth0 网卡 的 mac
    ioctl ( sendfd , SIOCGIFINDEX , &ifr ) ;
    mr.mr_ifindex = ifr.ifr_ifindex ;
    mr.mr_type = PACKET_MR_PROMISC ;
    if ( setsockopt (sendfd , SOL_PACKET , PACKET_ADD_MEMBERSHIP, &mr,sizeof(mr))== -1)
    {
      perror ( " can not set to promisc\n" ) ;
      exit ( 0 ) ;
    }
   
    while(1)
    {
      
      memset(&in_addr, 0x0, socklen);
	  memset(&dest, 0x0, socklen);
      recv_len = recvfrom(sock, buffer, sizeof(buffer), 0,
          (struct sockaddr *)&in_addr, &socklen); 
      print_packet(buffer,34);
     
      if(in_addr.sll_pkttype == PACKET_OUTGOING || in_addr.sll_pkttype ==PACKET_HOST) 
	  {
	      printf("won't send me again;\n");
		  continue;
	  } 
      dest.sll_family=AF_PACKET;
      dest.sll_ifindex =ifr.ifr_ifindex;
      dest.sll_halen=htons(ETH_HLEN); 
      memmove(&dest.sll_addr,buffer,sizeof(buffer[0])*8);
      n=sendto(sendfd,buffer,recv_len,0,(struct sockaddr*)&dest,sizeof(dest));
      printf(" send n=%d\n",n);
     
    }
    
    return 0;

}
