// networkhandler.c
// Otto Kristoffer Lote
// www.github.com/ottolote/TTK4145/exercises/Ex03-Network-basics/
// 
// Written using this great guide:
// http://www.beej.us/guide/bgnet/output/html/singlepage/bgnet.html
//
// Works with only IPv4
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


// FOR EPIC COLOROUTPUT
#define NETWORKSTRING "[ " TCOLOR_LIGHTBLUE "NetworkHandler" TCOLOR_NC " ] - "




void* networkHandlerRoutine() {  
  // INIT //

  // Initialize structs of type addrinfo
  // - hints:     for specifying initial options
  // - servinfo.  using specified hints to fill in possible servinfo
  // servinfo is pointer to first element in linked list of possible addrinfo
  struct addrinfo hints, *servinfo; 
  memset(&hints, 0, sizeof hints);

  hints.ai_family = AF_INET; // AF_INET for forced IPv4
  hints.ai_socktype = SOCK_DGRAM; // Only listen for datagrams
  hints.ai_flags = AI_PASSIVE; // Set addrinfo with 'my ip' (for server socket)



  int retval;
  // Evaluation of assignment is equal to left operand after assignment
  if ((retval = getaddrinfo(NULL, MYPORT, &hints, &servinfo)) != 0) {
    fprintf(stderr, "networkhandler: getaddrinfo: %s\n", gai_strerror(retval));
    return NULL;
  }


  // loop through linked list and bind to first possible
  struct addrinfo *p; // p is pointer to 
  int sockfd;
  for (p = servinfo; p != NULL; p = p->ai_next) {

    // Try to set up socket with addrinfo in p
    if((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1){
      perror("socket"); // perror prints argument, then prints errno
      continue;
    }

    // Try to bind socket to addressinfo (includes port information)
    if(bind(sockfd, p->ai_addr, p->ai_addrlen) == -1 ) {
      close(sockfd);
      perror("bind");
      continue;
    }

    break; //break if successful
  } //end of for-loop


  if(p == NULL) {
    fprintf(stderr, "networkhandler: failed to bind to socket");
    return NULL;
  }
      
  // Release memory occupied by servinfo
  freeaddrinfo(servinfo);




  // LOOP //

  char buf[MAXBUFLEN]; // Initialize receive buffer
  struct sockaddr senders_addr; // struct to store socketinfo from senders addr 
  socklen_t addr_len = sizeof senders_addr;
  char addr_str[INET6_ADDRSTRLEN]; 
  int byteCount;


  // Fill receivebuffer with message, print buffer, repeat
  // USING BLOCKING SOCKET
  while(1) {
    printf( NETWORKSTRING "Waiting for datagram packets at port %s\n", MYPORT);


    // - recvfrom receives packet and stores sender's address
    if((byteCount = recvfrom(sockfd, buf, MAXBUFLEN-1, 0,
            &senders_addr, &addr_len)) == -1) {
      perror("recvfrom");
      exit(1);
    }


    // Print packet info and message
    buf[byteCount] = '\0'; // Add nullcharacter to buffer for printf
    printf( NETWORKSTRING "received packet from: %s\n", 
        inet_ntop(senders_addr.sa_family, 
            senders_addr.sa_data, addr_str, sizeof addr_str));
    printf( NETWORKSTRING "packet is %d bytes long\n", byteCount);
    printf( NETWORKSTRING "packet contains: \"%s\"\n\n", buf);
  } // end of while


  close(sockfd); 
  return 0;
}

