#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
// #include <arpa/inet.h> 

#define PORT 8080

void logging(char* text, ...);

int main (void)
{
  
    char* init_server_message = "Web server has been reached by the client!";
    logging("Web server started!");

    // Initialize/create the server socket
    int server_socket_addr = socket(AF_INET, SOCK_STREAM, 0);
    logging("Succesfully initialized the server socket:", server_socket_addr);

    // Defining the servers address
    struct sockaddr_in server_ip_address;
    server_ip_address.sin_family = AF_INET;
    server_ip_address.sin_port = htons(PORT);
    server_ip_address.sin_addr.s_addr = INADDR_ANY; // <- change this so it is not any but specific ip
    logging("Succesfully defined the servers address");

    // Bind the socket to our specified IP and port
    bind(server_socket_addr, (struct sockaddr*) &server_ip_address, sizeof(server_ip_address));
    logging("Succesfully binded the socket to the specified address");
    
    // Let the server stay open until closed by intention
    
    // Listen for client connections
    listen(server_socket_addr, 10);
    logging("Started listening for client connections");
    int i = 1;
    while(i == 1) {
    // Two way connection, send and receive data both ways
    int client_socket = accept(server_socket_addr, NULL, NULL);
    logging("Started to way connection with", client_socket);

    printf("Succesfully connected: %i\n", client_socket);
    send(client_socket, init_server_message, sizeof(init_server_message), 0);
    }
        
    return 0;
}


/* 
This method is implemented to create a simple logging system that logs actions and append it to a log file.
It also takes the string and post it in a structured design in the console for better maintainability
*/

void logging(char* text, ...) {
time_t currentTime;
time(&currentTime);

FILE *logFile = fopen("Logfile.txt", "a");
if (logFile == NULL)
{
    printf("Logfile doesn't exist\n");
    exit(0);
}
fprintf(logFile, "%s: %s\n", ctime(&currentTime), text);
printf("==========================================\n");
printf("%s: %s \n", ctime(&currentTime), text);
}
