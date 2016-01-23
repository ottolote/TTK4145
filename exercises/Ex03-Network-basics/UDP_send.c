#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <netdb.h>
#include <errno.h>

int main(){ 
  char *message = "Åååhåhåhåhåhåhhå wololo";
  const char* hostname = 0; //localhost
  const char* portname = "daytime"; //testing
  struct addrinfo* res=0;
  struct addrinfo hints;
  memset(&hints, 0, sizeof(hints));

//   //socket file descriptor
//   sockfd = socket(AF_INET, SOCK_DGRAM, 0);
// 
//   if (sockfd < 0) { 
//     printf("Unable to setup socket\n");
//     exit(1);
//   }



  hints.ai_family=AF_UNSPEC;
  hints.ai_socktype=SOCK_DGRAM;
  hints.ai_protocol=0;
  hints.ai_flags=AI_ADDRCONFIG;

  int err=getaddrinfo(hostname,portname,&hints,&res);
  if (err!=0) {
      printf("failed to resolve remote socket address (err=%d)",err);
      exit(1);
  }
  

  int fd=socket(res->ai_family,res->ai_socktype,res->ai_protocol);
  if (fd==-1) {
    printf("unable to setup socket");
  }

  //Set option (SO_REUSEADDR) to reuse outbound port
  if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &(int){ 1 }, sizeof(int)) < 0) { 
    printf("setsockopt(SO_REUSEADDR) failed");
    exit(1);
  }

  if (sendto(fd,message,sizeof(message),0,res->ai_addr,res->ai_addrlen)==-1) {
    printf("unable to send");
    exit(1);
  }
  
}
