#include "../../header/headers.h"

const char *HOST = "man.he.net";
#define PORT 80
const char *PAGE = "/?topic=%s&section=all";

int printManPage(char *pageName) {
    int sockfd;
    struct sockaddr_in server_addr;
    struct hostent *server;
    char name[256];
    sprintf(name, PAGE, pageName);
    // Create a socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Error opening socket");
        exit(1);
    }

    // Get server's IP address
    server = gethostbyname(HOST);
    if (server == NULL) {
        // fprintf(stderr, "Error: No such host\n");
        errorPrintf("No such host\n");
        return EXEC_FAILURE;
    }

    // Initialize server_addr struct
    bzero((char *)&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    bcopy((char *)server->h_addr, (char *)&server_addr.sin_addr.s_addr,
          server->h_length);

    // Connect to the server
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) <
        0) {
        // perror("Error connecting to server");
        errorPrintf("Could not connect to server\n");
        return EXEC_FAILURE;
    }

    // Send the HTTP GET request
    char request[1000];
    sprintf(request, "GET %s HTTP/1.1\r\nHost: %s\r\n\r\n", name, HOST);
    if (send(sockfd, request, strlen(request), 0) < 0) {
        // perror("Error sending request");
        errorPrintf("Could not send request");
        return EXEC_FAILURE;
    }

    // Receive and print the response
    char response[4096];
    int cnt = 0;
    int last = 0;
    while (1) {
        cnt++;
        int bytes_received = recv(sockfd, response, sizeof(response) - 1, 0);
        if (bytes_received < 0) {
            // perror("Error receiving response");
            errorPrintf("Could not recieve response\n");
            return EXEC_FAILURE;
        } else if (bytes_received == 0) {
            break;  // End of response
        } else {
            response[bytes_received] = '\0';
            char *endPtr = strstr(response, "ATTRIBUTES\n");
            if (endPtr != NULL) {
                *endPtr = '\0';
                last = 1;
            }
            if (cnt == 1) {
                char *ptr = strstr(response, "NAME\n");
                if (ptr == NULL) {
                    // printf("nahi mila\n");
                    errorPrintf("Could not find man page\n");
                    return EXEC_FAILURE;
                } else {
                    printf(ptr);
                }
            } else {
                printf("%s", response);
            }

            if (last) break;
        }
    }

    // Close the socket
    close(sockfd);

    return EXEC_SUCCESS;
}

int iman(Subcommand command) {
    if (command->argc != 2) {
        errorPrintf("iman expects 1 argument found %lu\n", command->argc - 1);
        return EXEC_FAILURE;
    }

    if (printManPage(command->argv[1])) {
        return EXEC_FAILURE;
    } 
    
    return EXEC_SUCCESS;
}