#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

// Store final variables to save redudancy
#define PORT 8080
#define BUFFER_SIZE 1024

// Initialize functions 
void logging(char *text, ...);

int main(void)
{

    // Open the HTML file and storing the data
    FILE *html_index = fopen("index.html", "r");
    if (html_index == NULL)
    {
        perror("error opening HTML page");
        return 1;
    }

    // Open 404 html and storing the data
    FILE *html_notFound = fopen("notfound.html", "r");
    if (html_notFound == NULL)
    {
        perror("error opening HTML page");
        return 1;
    }

    // initialize respone data and not found data
    char buffer[BUFFER_SIZE];
    char response_data[BUFFER_SIZE];
    char response_data_notfound[BUFFER_SIZE];


    // Stores the input from the files into the array
    fgets(response_data, BUFFER_SIZE, html_index);
    fgets(response_data_notfound, BUFFER_SIZE, html_notFound);

    // append the response data with the http header to send the response :: TODO change this to remove redudancy
    char http_header[2048] =
    "HTTP/1.1 200 OK\r\n"
    "Content-Type: text/html\r\n"
    "\r\n";

    char http_header_not_found[2048] = 
    "HTTP/1.1 404 NOT FOUND\r\n"
    "Content-Type: text/html\r\n"
    "\r\n";

    char http_header_jpeg[2048] =
    "HTTP/1.1 200 OK\r\n"
    "Content-Type: image/jpeg\r\n"
    "\r\n";

    char http_header_gif[2048] =
    "HTTP/1.1 200 OK\r\n"
    "Content-Type: image/gif\r\n"
    "\r\n";

    char http_header_mp3[2048] =
    "HTTP/1.1 200 OK\r\n"
    "Content-Type: audio/mpeg\r\n"
    "\r\n";

    char http_header_pdf[2048] =
    "HTTP/1.1 200 OK\r\n"
    "Content-Type: application/pdf\r\n"
    "\r\n";

    // Prompting the terminal window to show the application has started
    char *init_server_message = "Web server has been reached by the client!";
    logging("Web server started!");

    // Initialize/create the server socket
    int server_socket_addr = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket_addr == -1)
    {
        perror("Error");
    }
    logging("Succesfully initialized the server socket:", server_socket_addr);

    // Defining the servers address
    struct sockaddr_in server_ip_address;
    int server_ip_address_len = sizeof(server_ip_address);

    server_ip_address.sin_family = AF_INET;
    server_ip_address.sin_port = htons(PORT);
    server_ip_address.sin_addr.s_addr = INADDR_ANY; // <- change this so it is not any but specific ip
    logging("Succesfully defined the servers address");

    struct sockaddr_in client_addr;
    int client_addrlen = sizeof(client_addr);

    // Bind the socket to our specified IP and port
    int bind_status = bind(server_socket_addr, (struct sockaddr *)&server_ip_address, server_ip_address_len);
    if (bind_status < 0)
    {
        logging("Error binding");
        perror("accept");
        exit(1);
    }
    logging("Succesfully binded the socket to the specified address");

    // Listen for client connections
    listen(server_socket_addr, 10);
    logging("Started listening for client connections");

    // Let the server stay open until closed by intention
    while (1)
    {

        // Two way connection, send and receive data both ways
        int client_socket = accept(server_socket_addr, (struct sockaddr *)&server_ip_address, (socklen_t *)&server_ip_address_len);
        logging("Started to way connection with", client_socket);
        if (client_socket < 0)
        {
            perror("Error");
            continue;
        }
        int readValue = read(client_socket, buffer, BUFFER_SIZE);
        if (readValue < 0)
        {
            perror("Error");
            continue;
        }
        printf("Succesfully connected: %i\n", readValue);

        // Take the clients request and print the first line in the terminal :: TODO change to the whole request
        char method[BUFFER_SIZE], uri[BUFFER_SIZE], version[BUFFER_SIZE];
        sscanf(buffer, "%s %s %s", method, uri, version);
        printf("[%s:%u] %s %s %s\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port), method, version, uri);

        // Compare uri and the index
        if(strstr(uri, "index.html") != NULL) {
        
        // Send the html header to the client
        int responseValue = write(client_socket, http_header, strlen(http_header));
        if (responseValue < 0) {
            perror("Error");
            continue;
        }
        // Send the html body to the client
        int bodyValue = write(client_socket, response_data, strlen(response_data));
        if (bodyValue < 0) {
            perror("Error");
           continue;
        }

        // Error handling if values is less than 0 and continue the loop instead of crashing the program
        if (responseValue < 0) {
            perror("Error");
            continue;
        }

        } else if (strstr(uri, ".gif") != NULL) {
            printf("GIF\n");

        } else if (strstr(uri, ".jpeg") != NULL) {
            printf("jpeg\n");

        } else if (strstr(uri, ".pdf") != NULL) {
            printf("pdf\n");

        } else if (strstr(uri, ".mp3") != NULL) {
            printf("mp3\n");
        }
        
        else {
            // If the URI doesnt meet requirements "html, gif, jpeg, pdf, mp3" send 404 not found
            write(client_socket, http_header_not_found, strlen(http_header_not_found));
        }

        // Succesfully close the socket
        close(client_socket);
    }

    // Close the files after being used
    fclose(html_index);
    fclose(html_notFound);

    // Show that the web server has been terminated
    return 0;
}

/*
This method is implemented to create a simple logging system that logs actions and append it to a log file.
It also takes the string and post it in a structured design in the console for better maintainability
*/

void logging(char *text, ...)
{
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

    fclose(logFile);
}
