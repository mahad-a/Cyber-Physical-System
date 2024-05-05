/*
Header file, initializes all the fifos, and struct for the data to pass through
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

// fifos
#define ECG_FIFO_NAME "/tmp/ecg_client_%d_Fifo"
#define IMAGE_FIFO_NAME "/tmp/image_client_%d_Fifo"
#define TEN_MEG (1024*1024*10)
#define BUFFER_SIZE PIPE_BUF

#define ECG_SERVER_FIFO_NAME "/tmp/ecg_server_fifo"
#define IMAGE_SERVER_FIFO_NAME "/tmp/image_server_fifo"


// data to pass
struct data_to_pass_st {
    pid_t  client_pid;
    char   some_data[BUFFER_SIZE - 1];
};
