#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <arpa/inet.h>



int socket_init()
{
   struct ifreq ifr;
   int sock;
   memset(&ifr, 0x0, sizeof(struct ifreq));

   if((sock = socket(PF_PACKET, SOCK_RAW, htons(0x0003))) < 0)
   {
       perror(" socket ");
       exit(0);
   }

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
	int sock = socket_init();
    unsigned   char	buffer [ 65536 ] ;
	struct sockaddr in_addr ;
    socklen_t socklen;
	ssize_t recv_len = 0;

	
      printf("Now I'm going to make a bridge for vm taps\n");

	  // socket bind port (recv)
   
	 while(1)
     {
       
       memset(&in_addr, 0x0, socklen);
       recv_len = recvfrom(sock, buffer, sizeof(buffer), 0,
           (struct sockaddr *)&in_addr, &socklen); 
	   print_packet(buffer,20);
      
     }
	 
     return 0;

}
