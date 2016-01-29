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

#define MAXBUFLEN 100

#define SERVERPORT "30000"

void *get_in_addr(struct sockaddr *sa);


int main(int argc, char *argv[]) {

  // If arguments passed is not two + 1 (you count the executable's name)
  if(argc != 2) {
    fprintf(stderr, "usage: %s <hostname>\n", argv[0]);
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
  hints.ai_socktype = SOCK_STREAM; // Only send datagrams

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

    if(connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
      close(sockfd);
      perror("client: connect");
      continue;
    }

    break; //break if successful
  }

  if(p == NULL) {
    fprintf(stderr, "failed to connect");
    return 2;
  }

  char s[INET6_ADDRSTRLEN];
  inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr),
      s, sizeof s);
  printf("client: connecting to %s\n", s);
      
  // Release memory occupied by clientinfo
  freeaddrinfo(clientinfo);

  int numbytes;
  char sendstr[MAXBUFLEN]; // could be different from MAXBUFLEN

  do {
     gets(sendstr);
     // Send package with data from argument 2
     if ((numbytes = send(sockfd, sendstr, strlen(sendstr)+1, 0 )) == -1) {
         perror("send");
         exit(1);
     }
 
     printf("sent %d bytes to %s\n", numbytes, argv[1]);

     char buf[MAXBUFLEN];

     if((numbytes =  recv(sockfd, buf, MAXBUFLEN-1, 0)) == -1) {
       perror("recv");
       exit(1);
     }
 
     buf[numbytes] = '\0';
     printf("client: received '%s'\n", buf);
      printf("%d\n", strcmp(sendstr, "kuk"));
  } while (strcmp(sendstr, "nope") != 0);



  close(sockfd);

  return 0;
}

// From www.beej.us
// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}
