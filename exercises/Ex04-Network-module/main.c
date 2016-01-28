// UDP_listen.c
// Otto Kristoffer Lote
// www.github.com/ottolote/TTK4145/exercises/Ex03-Network-basics/
// 
// Written using this great guide:
// http://www.beej.us/guide/bgnet/output/html/singlepage/bgnet.html
//
// Works with both IPv4 and IPv6

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

#include "networkhandler.h"



int main() {
  pthread_t network_handler;

  // Start networkhandler
  pthread_create(&network_handler, NULL, networkHandlerRoutine, NULL);

  // Wait for network_handler
  pthread_join(network_handler, NULL);
  return 0;
}


