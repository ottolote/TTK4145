#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types>


int waitflag = 0;

void sigusr_callback_handler(int signum) {
	waitflag = 1;
}

int main(argc, argv[]) {

	signal(SIGUSR1, sigusr_callback_handler);

	if(argc == 2) {
		// spawned as backup
		int count = (int)strtol(argv[1]);

		do {

			waitflag = 0;
			sleep(1.1);

		} while (waitflag);
	} else {
		//spawned as primary
		int count = 0;
		pit_t child_pid = fork();
	}




	do {
		count++;
		printf("%d\n",count);
		sleep(1);
	} while ( count > 0 );

	return 0;
}


	
	
