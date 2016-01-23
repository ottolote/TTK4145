// for 
//  - strerr error handling
#include <errno.h>

// for
//  - memset()
#include <string.h>

// for
//  - usleep()
#include <unistd.h>

// for: 
//  - getaddrinfo()
//  - freeaddrinfo()
//  - struct addrinfo
//  - AF_UNSPEC
//  - SOCK_DGRAM
//  - AI_PASSIVE
//  - AI_ADDRCONFIG
#include <netdb.h>

// for
//  - socket() 
//  - bind()
//  - setsockopt()
//  - recvfrom()
//  - struct sockaddr_storage
#include <sys/socket.h>

// for
//  - 
// #include <netinet/in.h>

// for 
//  - exit()
#include <stdlib.h>

// for 
//  - printf()
#include <stdio.h>


// Prototype for handling datagrams
int handle_datagram(char* buffer, int count);


int main() {
  const char* hostname=0; // wildcard 
  const char* portname="daytime"; // daytime service = port 13

  // Initialize hints (addrinfo struct) to zero
  struct addrinfo hints;
  memset(&hints,0,sizeof(hints));

  // Accept any address family (IPv4 and IPv6)
  hints.ai_family=AF_UNSPEC; 

  // Accept only datagrams, excludes TCP
  hints.ai_socktype=SOCK_DGRAM;

  // Non-zero protocol is only meaningful for specified address family
  hints.ai_protocol=0;

  // AI_PASSIVE must be set to be able to bind socket for accepting incoming connections
  // AI_ADDRCONFIG must be set to assure IPv4 is returned only when IPv4 is configured and IPv6 is returned only when IPv6 is configured
  hints.ai_flags=AI_PASSIVE|AI_ADDRCONFIG;

  // Create pointer to addrinfo struct (res) and fill with addrinfo collected by getaddrinfo() 
  struct addrinfo* res=0;
  int err=getaddrinfo(hostname,portname,&hints,&res);

  // getaddrinfo() returns zero int on success
  if (err!=0) {
      printf("failed to resolve local socket address (err=%d)",err);
      exit(1);
  }

  // Initialize socket file with socket() using addrinfo collected from getaddrinfo
  int fd=socket(res->ai_family,res->ai_socktype,res->ai_protocol);
  if (fd==-1) {
      printf("%s",strerror(errno));
      exit(1);
  }

  //Set option (SO_REUSEADDR) to reuse outbound port
  // if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &(int){ 1 }, sizeof(int)) < 0) { 
  //   printf("setsockopt(SO_REUSEADDR) failed");
  //   exit(1);
  // }

  // Bind the local address to the socket with bind() using the socket file descriptor fd and addrinfo collected by getaddrinfo
  if (bind(fd,res->ai_addr,res->ai_addrlen)==-1) {
      printf("%s",strerror(errno));
      exit(1);
  }

  // Release the memory occupied by the address list created with getaddrinfo
  freeaddrinfo(res);

  
  // Create buffer for receiving messages
  char buffer[549];
  struct sockaddr_storage src_addr;
  socklen_t src_addr_len=sizeof(src_addr);

  printf("listening\n");
  ssize_t count=recvfrom(fd,buffer,sizeof(buffer),0,(struct sockaddr*)&src_addr,&src_addr_len);
  printf("received\n");
  if (count==-1) {
      printf("%s",strerror(errno));
      exit(1);
  } else if (count==sizeof(buffer)) {
      printf("datagram too large for buffer: truncated");
  } else {
      handle_datagram(buffer,count);
  }

}


// Printf all characters in buffer
int handle_datagram(char *buffer, int count) {
  printf("handling datagram\n");
  count++;
  count--;
  //for (int i = 0; i < count; i++) {
    printf("%c", buffer[0]);
  //}
  return 0;
}


