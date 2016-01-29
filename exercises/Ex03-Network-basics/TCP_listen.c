// UDP_listen.c
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
//  - memset()
#include <string.h>

// for
//  - usleep()
//  - close()
#include <unistd.h>

// for 
//  - exit()
#include <stdlib.h>

// for
//  - inet_ntop()
#include <arpa/inet.h>

// for
//  - socket() 
//  - bind()
//  - setsockopt()
//  - recvfrom()
//  - struct sockaddr_storage
#include <sys/socket.h>

#define MAXBUFLEN 100 // could be bigger
#define MYPORT "30000"

// get sockaddress IPv4 or IPv6
void *get_in_addr(struct sockaddr *sa);





int main() {
  
  int retval;

  // Initialize structs of type addrinfo
  // - hints:     for specifying initial options
  // - servinfo.  using specified hints to fill in possible servinfo
  // servinfo is pointer to first element in linked list of possible servinfo
  struct addrinfo hints, *servinfo; 
  memset(&hints, 0, sizeof(hints));

  hints.ai_family = AF_UNSPEC; // Use IPv4 or IPv6, AF_INET for forced IPv4
  hints.ai_socktype = SOCK_STREAM; // Only listen for datagrams
  hints.ai_flags = AI_PASSIVE; // Set ip to my ip (for server socket)

  // Evaluation of assignment is equal to left operand after assignment
  if ( (retval = getaddrinfo(NULL, MYPORT, &hints, &servinfo)) != 0 ) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(retval));
    return 1;
  }

  struct addrinfo *p;
  int sockfd;

  // loop through linked list and bind to first possible
  for (p = servinfo; p != NULL; p = p->ai_next) {

    // Try to set up socket with addrinfo in p
    if((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1){
      perror("socket"); // perror prints argument, then print errno
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
    fprintf(stderr, "failed to bind to socket");
    return 2;
  }

  printf("listening for connections\n");
      
  if(listen(sockfd, 5) == -1) {
    perror("listen");
    close(sockfd);
    exit(1);
  }


  // Release memory occupied by servinfo
  freeaddrinfo(servinfo);

  struct sockaddr_storage senders_addr; // struct to store IPv4 sockaddr AND/OR IPv6 sockaddr
  char buf[MAXBUFLEN]; // Receive buffer
  socklen_t addr_len = sizeof senders_addr;
  char s[INET6_ADDRSTRLEN]; // string s is never longer than INET6_ADDRSTRLEN
  int numbytes;
  int new_fd;

  if(( new_fd = accept(sockfd, (struct sockaddr *)&senders_addr, &addr_len)) == -1){
    perror("accept");
    exit(1);
  }

  do {
    if((numbytes =  recv(new_fd, buf, MAXBUFLEN-1, 0)) == -1) {
      perror("recv");
      exit(1);
    }

    buf[numbytes] = '\0';

    printf("server: received '%s'\n", buf);
    
    gets(buf);
    // Send package with data from argument 2
    if ((numbytes = send(sockfd, buf, strlen(buf)+1, 0 )) == -1) {
        perror("send");
        exit(1);
    }

    printf("sent %d bytes to %s\n", numbytes, argv[1]);
    
  } while (strcmp(buf,"stop"));

  close(sockfd);

  return 0;
}




    // recvfrom receives packet and stores sender's address

//     if((numbytes = recvfrom(sockfd, buf, MAXBUFLEN-1, 0,
//             (struct sockaddr*)&senders_addr, &addr_len)) == -1) {
//       perror("recvfrom");
//       exit(1);
//     }
// 
//     printf("received packet from: %s\n", 
//         inet_ntop(senders_addr.ss_family, 
//           get_in_addr((struct sockaddr*)&senders_addr), s, sizeof s));
// 
//     printf("packet is %d bytes long\n", numbytes);
// 
//     buf[numbytes] = '\0'; // Add nullcharacter to buffer for printf



// From www.beej.us
// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}
