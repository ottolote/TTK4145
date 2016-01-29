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
#define NETSTR "[ " TCOLOR_LIGHTBLUE "NetworkHandler" TCOLOR_NC " ] - "


// Private functions
void networkHandlerInit(int* sockfd);





// networkHandler "main()"
void* networkHandlerRoutine() {  
    // INIT //

    int sockfd;
    networkHandlerInit(&sockfd);
    
    // LOOP //
    
    char buf[MAXBUFLEN]; // Initialize receive buffer
    struct sockaddr senders_addr; // struct to store socketinfo from senders addr 
    socklen_t addr_len = sizeof senders_addr;
    char addr_str[INET_ADDRSTRLEN]; 
    int byteCount;
    
    
    // Fill receivebuffer with message, print buffer, repeat
    // USING BLOCKING SOCKET
    while(1) {
        printf( NETSTR "Waiting for datagram packets at port %s\n", MYPORT);
        
        
        // - recvfrom receives packet and stores sender's address
        if((byteCount = recvfrom(sockfd, buf, MAXBUFLEN-1, 0,
                &senders_addr, &addr_len)) == -1) {
            perror("recvfrom");
            exit(1);
        }
        
        
        // Print packet info and message
        buf[byteCount] = '\0'; // Add nullcharacter to buffer for printf
        printf( NETSTR "received packet from: %s\n", 
                inet_ntop(senders_addr.sa_family, 
                    senders_addr.sa_data, addr_str, sizeof addr_str));
        printf(NETSTR "packet is %d bytes long and contains: \"%s\"\n\n", byteCount, buf);
    } // end of while
    
    
    close(sockfd); 
    return 0;
}

void networkHandlerInit(int* sockfd) {
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
        return;
    }
    
    
    // loop through linked list and bind to first possible
    int new_sock;
    struct addrinfo *p; // p is pointer to 
    for (p = servinfo; p != NULL; p = p->ai_next) {
    

        // Try to set up socket with addrinfo in p
        if((new_sock = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1){
          perror("socket"); // perror prints argument, then prints errno
          continue;
        }
        

        // Try to bind socket to addressinfo (includes port information)
        if(bind(new_sock, p->ai_addr, p->ai_addrlen) == -1 ) {
          close(new_sock);
          perror("bind");
          continue;
        }
        

        break; //break if successful
    } //end of for-loop
    
    
    if(p == NULL) {
        fprintf(stderr, "networkhandler: failed to bind to socket");
        return;
    }
        
    // Release memory occupied by servinfo
    freeaddrinfo(servinfo);

    // Update sockfd used by networkHandlerRoutine
    *sockfd = new_sock;
} 
