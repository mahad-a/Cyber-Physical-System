# Emulation of a Simplified Cyber-Physical System (CPS) Using Software Processes and Message Passing

# Developed by Mahad Ahmed

The assignment is to practice some process management related techniques, including process creation, sending/handling of signals, and process communication and synchronization via message passing using the FIFO client/server model.

# Usage
Compiling - You will be required to create three separate terminals, one for ECG server, one for Image Server and one for the main CPS program.
1. Launch terminal and navigate (use cd) to where the programs are located
2. Compile the ECG server on its own terminal (Image can also be first) by inputting:
gcc ecg_server.c -o ecg
3. Compile the Image server on its own terminal (ECG can also be second) by inputting:
gcc image_data_server.c -o image
4. Compile the main CPS program on its own terminal by inputting:
gcc medical_cps.c -o cps
5. Now run the two servers in any order desired by running these commands in their respective terminals:
./ecg
./image
6. Run the CPS program in its own terminal by inputting:
./cps
7. The final product will be outputted on screen. DISCLAIMER once CPS program has stopped ending, it may have no command line appear afterwards but the program has completely ended. A simple enter can bring the command line back or you can continue as normal. Usually in a infinite loop, even if you were to press enter, it will stay within the program and the not show the command line.
