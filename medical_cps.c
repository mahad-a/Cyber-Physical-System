#include "medical_cps.h"


/* 
Handler, checks to see which flag was raised and responds accordingly
*/
void difference_in_bytes(int sig, siginfo_t *info, void* context) {
	if (sig == SIGUSR1) {
		printf("SIGUSR1 has been raised, bytes sent through pipe and bytes received match.\n");
	} else if (sig == SIGALRM) {
		printf("SIGALRM has been raised, bytes sent through pipe and bytes received do not match.\n");
	}
}

/*
The ECG child process. This function sets up communication between the ECG client and server using FIFOs. 
It writes a message to the ECG server, reads a response, and compares bytes sent and received.
*/
void ecgChildProcess() {
	int client_fd, server_fd;
	struct data_to_pass_st my_data;
	int bytes_sent = 0;
	int bytes_received;
	int res, res2;
	char ecg_fifo[256]; 

	my_data.client_pid = getpid();
	sprintf(ecg_fifo, ECG_FIFO_NAME, my_data.client_pid);

	res = mkfifo(ecg_fifo, 0777);
	
	if (res != 0) {
		fprintf(stderr, "Failure to create ECG fifo: %s\n", ecg_fifo);
		exit(EXIT_FAILURE);
	}

	server_fd = open(ECG_SERVER_FIFO_NAME, O_WRONLY);
	
	if (server_fd == -1) {
		fprintf(stderr, "Failure to open ECG server fifo for writing: %s\n", ECG_SERVER_FIFO_NAME);
		exit(EXIT_FAILURE);
	}

	strcpy(my_data.some_data, "This is an empty message for the ECG Server for Assignment 1\n");

	printf("User PID: %d, is attempting to write on ECG server pipe.\n", my_data.client_pid);

	res = write(server_fd, &my_data, sizeof(my_data));
	
	if (res == -1) {
		fprintf(stderr, "Error on writing to ECG server pipe.\n");
		exit(EXIT_FAILURE);
	}

	bytes_sent = strlen(my_data.some_data);
	printf("Successfully writing to ECG server pipe.\n");
	printf("# of Bytes sent: %d\n", bytes_sent);

	client_fd = open(ecg_fifo, O_RDONLY);
	
	if (client_fd == -1) {
		fprintf(stderr, "Failure to open ECG fifo for reading: %s\n", ecg_fifo);
		exit(EXIT_FAILURE);
	}

	res2 = read(client_fd, &my_data, sizeof(my_data));
	
	if (res2 > 0) {
		printf("Incoming message from PID: %d\n", my_data.client_pid);
		printf("Message received: %s\n", my_data.some_data);
		bytes_received = strlen(my_data.some_data);
	} else {
		printf("Failure, message could not be read.\n");
		exit(EXIT_FAILURE);
	}
	
	printf("Now comparing bytes sent and bytes received between ECG server and ECG client.\n");
	printf("Bytes sent: %d\n", bytes_sent);
	
	printf("Bytes received: %d\n", bytes_received);
	printf("Program successful. Closing.\n");
	
	close(client_fd);

	if (bytes_received != bytes_sent) { // bytes sent are different from bytes received
		raise(SIGALRM);
		exit(EXIT_FAILURE);
	} else {
		raise(SIGUSR1);
		kill(getppid(), SIGUSR1);
		exit(EXIT_SUCCESS);
	}
}

/*
The Image Data child process. This function sets up communication between the Image Data client and server 
using FIFOs. It writes a message to the Image server, reads a response, and compares bytes sent and received.
*/
void imageChildProcess() {
	int client_fd, server_fd;
	struct data_to_pass_st my_data;
	int bytes_sent = 0;
	int bytes_received;
	int res, res2;
	char image_fifo[256];

	my_data.client_pid = getpid();
	sprintf(image_fifo, IMAGE_FIFO_NAME, my_data.client_pid);

	res = mkfifo(image_fifo, 0777);
	
	if (res != 0) {
		fprintf(stderr, "Failure to create IMAGE fifo: %s\n", image_fifo);
		exit(EXIT_FAILURE);
	}

	server_fd = open(IMAGE_SERVER_FIFO_NAME, O_WRONLY);
	
	if (server_fd == -1) {
		fprintf(stderr, "Failure to open image server fifo for writing: %s\n", IMAGE_SERVER_FIFO_NAME);
		exit(EXIT_FAILURE);
	}

	strcpy(my_data.some_data, "This is an empty message for the Image Data Server for Assignment 1\n");

	printf("User PID: %d, is attempting to write on image server pipe.\n", my_data.client_pid);

	res = write(server_fd, &my_data, sizeof(my_data));
	
	if (res == -1) {
		fprintf(stderr, "Error on writing to image server pipe.\n");
		exit(EXIT_FAILURE);
	}

	bytes_sent = strlen(my_data.some_data);
	printf("Successfully writing to Image server pipe.\n");
	printf("# of Bytes sent: %d\n", bytes_sent);

	client_fd = open(image_fifo, O_RDONLY);
	
	if (client_fd == -1) {
		fprintf(stderr, "Failure to open image fifo for reading: %s\n", image_fifo);
		exit(EXIT_FAILURE);
	}

	res2 = read(client_fd, &my_data, sizeof(my_data));
	
	if (res2 > 0) {
		printf("Incoming message from PID: %d\n", my_data.client_pid);
		printf("Message received: %s\n", my_data.some_data);
		bytes_received = strlen(my_data.some_data);
	} else {
		printf("Failure, message could not be read.\n");
		exit(EXIT_FAILURE);
	}
	
	printf("Now comparing bytes sent and bytes received between Image Server and Image Client.\n");
	printf("Bytes sent: %d\n", bytes_sent);
	
	printf("Bytes received: %d\n", bytes_received);
	printf("Program successful. Closing.\n");
	
	close(client_fd);

	if (bytes_received != bytes_sent) { // bytes sent are different from bytes received
		raise(SIGALRM);
		exit(EXIT_FAILURE);
	} else {
		raise(SIGUSR1);
		kill(getppid(), SIGUSR1);
		exit(EXIT_SUCCESS);
	}
}


/*
Main, the creation of ECG and Image Data child processes. It also sets up signal handlers to handle byte comparison results.
*/
int main() {
	pid_t ecg_pid, image_pid;
	struct sigaction signal_bytes;

	// Set up signal handler
	signal_bytes.sa_sigaction = difference_in_bytes;
	signal_bytes.sa_flags = SA_SIGINFO;
	sigaction(SIGUSR1, &signal_bytes, 0);
	sigaction(SIGALRM, &signal_bytes, 0);

	// Start by forking ecg, then image
	printf("Attempting to create child process for ECG server\n");
	ecg_pid = fork();

	if (ecg_pid == 0) {
		// ECG's child process
		printf("We are now entering the child process for ECG server.\n");
		ecgChildProcess();
	} else if (ecg_pid > 0) {
		// ECG's parent process
		// Proceed to fork image
		printf("We are now entering the parent process for ECG server. Attempting to create child process for Image server.\n");
		image_pid = fork();
		
		if (image_pid == 0) {
		    // Image's child process
		    printf("We are now entering the child process for Image server.\n");
		    imageChildProcess();
		} else if (image_pid > 0) {
		    // Image's parent process
		} else {
		    perror("fork failed for image process");
		    exit(1);
		}
	} else {
		perror("fork failed for ecg process");
		exit(1);
	}


	unlink(IMAGE_FIFO_NAME);
	unlink(ECG_FIFO_NAME);


	return 0;
}
