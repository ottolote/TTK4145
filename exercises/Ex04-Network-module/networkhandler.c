// networkhandler.c
// Otto Kristoffer Lote
// www.github.com/ottolote/TTK4145/exercises/Ex03-Network-basics/
// 
// Written using this great guide:
// http://www.beej.us/guide/bgnet/output/html/singlepage/bgnet.html
//
// Works with both IPv4 and IPv6
//
// TODO:
//  - Message passing
//  - Crash checking? 
//  - Return values when unsuccessful
//  - Do something useful except just printing message buffer
//  - Check buffer truncation

#include <stdio.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include "terminalcolors.h"
#include "networkhandler.h"

#define MAXBUFLEN 549 // could be bigger, 549 was good according to random site
#define MYPORT "30000"

// FOR 
#define NETWORKSTRING "[ " TCOLOR_BLUE "networkhandler" TCOLOR_NC " ]: "


// Private functions:
// get sockaddress IPv4 or IPv6
void *get_in_addr(struct sockaddr *sa);





void* networkHandlerRoutine() { // running as thread
  // INIT //

  // Initialize structs of type addrinfo
  // - hints:     for specifying initial options
  // - servinfo.  using specified hints to fill in possible servinfo
  // servinfo is pointer to first element in linked list of possible servinfo
  struct addrinfo hints, *servinfo; 
  memset(&hints, 0, sizeof hints);

  hints.ai_family = AF_UNSPEC; // Use IPv4 or IPv6, AF_INET for forced IPv4
  hints.ai_socktype = SOCK_DGRAM; // Only listen for datagrams
  hints.ai_flags = AI_PASSIVE; // Set ip to my ip (for server socket)



  int retval;
  // Evaluation of assignment is equal to left operand after assignment
  if ( (retval = getaddrinfo(NULL, MYPORT, &hints, &servinfo)) != 0 ) {
    fprintf(stderr, "networkhandler: getaddrinfo: %s\n", gai_strerror(retval));
    return NULL;
  }


  // loop through linked list and bind to first possible
  struct addrinfo *p;
  int sockfd;
  for (p = servinfo; p != NULL; p = p->ai_next) {

    // Try to set up socket with addrinfo in p
    if((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1){
      perror("socket"); // perror prints argument, then prints errno
      continue;
    }

    // Try to bind socket to address
    if(bind(sockfd, p->ai_addr, p->ai_addrlen) == -1 ) {
      close(sockfd);
      perror("bind");
      continue;
    }

    break; //break if successful
  }

  if(p == NULL) {
    fprintf(stderr, "networkhandler: failed to bind to socket");
    return NULL;
  }
      
  // Release memory occupied by servinfo
  freeaddrinfo(servinfo);




  // LOOP //

  char buf[MAXBUFLEN]; // Initialize receive buffer
  struct sockaddr_storage senders_addr; // struct to store IPv4 sockaddr OR IPv6 sockaddr from sender
  socklen_t addr_len = sizeof senders_addr;
  char addr_str[INET6_ADDRSTRLEN]; 
  int byteCount;


  // Fill receivebuffer with message, print buffer, repeat
  while(1) {
    printf( NETWORKSTRING "Waiting for datagram packets at port %s\n", MYPORT);


    // - recvfrom receives packet and stores sender's address
    // - sockaddr_storage (senders_addr) can be cast to sockaddr 
    //      and both IPv4 and IPv6 information is magically cast 
    //      correctly into sockaddr and addr_len automatically
    //      reduced if truncation is detected
    if((byteCount = recvfrom(sockfd, buf, MAXBUFLEN-1, 0,
            (struct sockaddr*)&senders_addr, &addr_len)) == -1) {
      perror("recvfrom");
      exit(1);
    }


    // Print packet info and message
    buf[byteCount] = '\0'; // Add nullcharacter to buffer for printf
    printf( NETWORKSTRING "received packet from: %s\n", 
        inet_ntop(senders_addr.ss_family, 
          get_in_addr((struct sockaddr*)&senders_addr),
            addr_str, sizeof addr_str));
    printf( NETWORKSTRING "packet is %d bytes long\n", byteCount);
    printf( NETWORKSTRING "packet contains: \"%s\"\n", buf);
  } // end of while


  close(sockfd); 
  return 0;
}

// From www.beej.us
// Chack if IPv4 or IPv6 and cast accordingly
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}
