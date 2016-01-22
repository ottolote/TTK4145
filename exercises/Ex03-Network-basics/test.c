#include <stdio.h>
#include <sys/socket.h>
#include <netdb.h>

int main(){ 
  int sock, sockName, msglen;
  struct sockaddr *restrict address;
  char message[1024]; 

  sock = socket(AF_INET, SOCK_DGRAM, 0);
  sockName = getsockname(sock, address, sizeof(address));

  if(sock < 0){
    printf("YOU BLEW IT!\n");
    exit(1);
  }
  
  if(bind(sock, clientAddress, sizeof(address))){
    printf("YOU BLEW IT EVEN MORE!\n");
    exit(1);
  }
  
  while(msglen = read(sock, message, 1024)){
    message[msglen] = '\0';
    printf("recv: %s\n", message);
    
  }

}
