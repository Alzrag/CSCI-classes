
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>

struct message {
	char source[50];
	char target[50]; 
	char msg[200]; // message body
};

void terminate(int sig) {
	printf("Exiting....\n");
	fflush(stdout);
	exit(0);
}

int main() {
	int server;
	int target;
	int dummyfd;
	struct message req;
	signal(SIGPIPE,SIG_IGN);
	signal(SIGINT,terminate);
	server = open("serverFIFO",O_RDONLY);
	dummyfd = open("serverFIFO",O_WRONLY);

	while (1) {
		// TODO:
		// read requests from serverFIFO
        
        if (read(server, &req, sizeof(req))!=sizeof(req)){
            continue;
        }

        req.source[sizeof(req.source) - 1] = '\0';
		req.target[sizeof(req.target) - 1] = '\0';
		req.msg[sizeof(req.msg) - 1] = '\0';

		printf("Received a request from %s to send the message %s to %s.\n",req.source,req.msg,req.target);

		// TODO:
		// open target FIFO and write the whole message struct to the target FIFO
		// close target FIFO after writing the message

        fflush(stdout);
        
        target = open(req.target, O_WRONLY);
        if (target == -1){
            perror("server: could not open target FIFO");
			continue;
        }

        write(target, &req, sizeof(req));

        close(target);

	}
	close(server);
	close(dummyfd);
	return 0;
}

