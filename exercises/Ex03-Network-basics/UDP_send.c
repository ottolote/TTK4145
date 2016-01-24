// UDP_send.c
// Otto Kristoffer Lote
// www.github.com/ottolote/TTK4145/exercises/Ex03-Network-basics/
// 
// Written using this great guide:
// http://www.beej.us/guide/bgnet/output/html/singlepage/bgnet.html
//
// Works with both IPv4 and IPv6


// for 
//  - printf()
//  - fprintf()
#include <stdio.h>

// for 
//  - exit()
#include <stdlib.h>

// for
//  - memset()
#include <string.h>

// for
//  - usleep()
//  - close()
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

#define SERVERPORT "30000"



int main(int argc, char *argv[]) {

  // If arguments passed is not two + 1 (you count the executable's name)
  if(argc != 3) {
    fprintf(stderr, "usage: %s <hostname> <message>\n", argv[0]);
    exit(1);
  }

  int retval;

  // Initialize structs of type addrinfo
  // - hints:       for specifying initial options
  // - clientinfo:  using specified hints to fill in possible clientinfo
  // clientinfo is pointer to first element in linked list of possible clientinfo
  struct addrinfo hints, *clientinfo; 
  memset(&hints, 0, sizeof(hints));

  hints.ai_family = AF_UNSPEC; // Use IPv4 or IPv6, AF_INET for forced IPv4
  hints.ai_socktype = SOCK_DGRAM; // Only send datagrams

  // Evaluation of assignment is equal to left operand after assignment
  if ( (retval = getaddrinfo(argv[1], SERVERPORT, &hints, &clientinfo)) != 0 ) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(retval));
    return 1;
  }

  struct addrinfo *p;
  int sockfd;

  // loop through linked list and try to make a socket
  for (p = clientinfo; p != NULL; p = p->ai_next) {

    // Try to set up socket with addrinfo in p
    if((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1){
      perror("socket"); // perror prints argument, then print errno
      continue;
    }

    break; //break if successful
  }

  if(p == NULL) {
    fprintf(stderr, "failed to create socket");
    return 2;
  }
      
  // Release memory occupied by clientinfo
  freeaddrinfo(clientinfo);

  int numbytes;

  // Send package with data from argument 2
  if ((numbytes = sendto(sockfd, argv[2], strlen(argv[2]), 0,
           p->ai_addr, p->ai_addrlen)) == -1) {
      perror("sendto");
      exit(1);
  }

  printf("sent %d bytes to %s\n", numbytes, argv[1]);
  close(sockfd);

  return 0;
}
