/*
The ECG Server
*/
#include "medical_cps.h"

int main() {
	int server_fd, client_fd;
	struct data_to_pass_st my_data;
	int bytes_received  = 0;
	int res, res2;
	char ecg_fifo[256];
	
	unlink(ECG_SERVER_FIFO_NAME);
	
	res = mkfifo(ECG_SERVER_FIFO_NAME, 0777);
	
	if (res == -1) {
		fprintf(stderr, "Failure to create ECG Server fifo: %s\n", ECG_SERVER_FIFO_NAME);
		exit(EXIT_FAILURE);
	}
	
	printf("ECG Server has been created.\n");	
	
	server_fd = open(ECG_SERVER_FIFO_NAME, O_RDONLY | O_NONBLOCK);
	
	if (server_fd == -1) {
		fprintf(stderr, "Failure to open ECG SERVER fifo for reading: %s\n", ECG_SERVER_FIFO_NAME);
		exit(EXIT_FAILURE);
	}
	
	printf("ECG Server connection for reading has been established.\n");
	
	
	do {
		res = read(server_fd, &my_data, sizeof(my_data));
		
		// server fifo has read something
		if (res > 0) {
		
			sprintf(ecg_fifo, ECG_FIFO_NAME, my_data.client_pid);
			
			printf("Incoming message from PID: %d\n", my_data.client_pid);
			printf("Message received: %s\n", my_data.some_data);
			
			bytes_received = strlen(my_data.some_data);
			
			printf("# of Bytes received: %d\n", bytes_received);
			client_fd = open(ecg_fifo, O_WRONLY);
			
			if (client_fd == -1) {
				fprintf(stderr, "Failure to open ECG fifo for writing\n");
				exit(EXIT_FAILURE);
			}
			
			printf("ECG fifo has been opened.\n");
			res2 = write(client_fd, &bytes_received, sizeof(bytes_received));
			
			if (res2 < 0) {
				fprintf(stderr, "Failure to write to the ECG fifo.\n");
				close(client_fd);
				exit(EXIT_FAILURE);
			} else {
				printf("Message successfully sent.\n");
				printf("Delivered: %s\n", my_data.some_data);
				
			}
			
		} else {
			usleep(50);
		}

	} while (res <= 0);
	
	printf("Program successful. Closing.\n");
	close(server_fd);

	exit(EXIT_SUCCESS);
}

