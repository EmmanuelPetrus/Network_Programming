#include "network_libraries.h"
#include <string.h>
#include <time.h>

#if defined(_WIN32)
#define ISVALIDSOCKET(s) ((s) != INVALID_SOCKET)
#define CLOSESOCKET(s) closesocket(s)
#define GETSOCKETERRNO() (WSAGetLastError())
#endif

int main()
{

#if defined(_WIN32)
    WSADATA d;
    if (WSAStartup(MAKEWORD(2, 2), &d))
    {
        fprintf(stderr, "Failed to initialize.\n");
        return 1;
    }
#endif
    // configuring a local address on the server
    printf("Configuring local address.....\n");
    struct addrinfo hints;            // a struct that contains info about our local addresss
    memset(&hints, 0, sizeof(hints)); // it sets all variables inside the struct to zero
    hints.ai_family = AF_INET6;        // it specifies the type of address we're trying to return from the server AF_INET stands for IPv4
    hints.ai_socktype = SOCK_STREAM;  // it specifies that we're establishing a TCP connection protocol
    hints.ai_flags = AI_PASSIVE;      // it tells our program that we're ready to bind

    // importing other important info into our program
    struct addrinfo *bind_address;                 // creating a address to bind our infos
    getaddrinfo(0, "8080", &hints, &bind_address); // it fills a struct addresss info  with the needed information more like copying some details from hints variable to bind address variable coupled with the port and other information that the getaddringo fetches automatically

    // creating socket for use
    printf("Creating socket...\n");
    SOCKET socket_listen;                                                                                  // an unsigned int type to store value returned by socket
    socket_listen = socket(bind_address->ai_family, bind_address->ai_socktype, bind_address->ai_protocol); // creating a socket for listening

    // checking if our socket creation was successful
    if (!ISVALIDSOCKET(socket_listen))
    {
        fprintf(stderr, "socket() failed.  (%d)\n", GETSOCKETERRNO());
        return 1;
    }

    // binding our socket with our local address
    printf("Binding socket to local address...\n");
    if (bind(socket_listen, bind_address->ai_addr, bind_address->ai_addrlen)) // binding socket to address so that the ipaddress and the port can be bound together
    {
        fprintf(stderr, "bind() failed. (%d)\n", GETSOCKETERRNO());
        return 1;
    }
    freeaddrinfo(bind_address); // free the bind_address

    // listening
    printf("Listening...\n");          // listening for connection
    if (listen(socket_listen, 10) < 0) // ensuring that the socket is listening to at most ten connection requests
    {
        fprintf(stderr, "listen() failed. (%d)\n", GETSOCKETERRNO());
        return 1;
    }

    // Accepting incoming functions
    printf("Waiting for connection...\n");
    struct sockaddr_storage client_address; // a struct to store client address that will be sent when the client tries to connect
    socklen_t client_len = sizeof(client_address);
    SOCKET socket_client = accept(socket_listen, (struct sockaddr *)&client_address, &client_len); // creating a new connection for sending and receiving data
    if (!ISVALIDSOCKET(socket_client))
    {
        fprintf(stderr, "accept () failed. (%d)\n", GETSOCKETERRNO());
        return 1;
    }

    // printing the address to the console
    printf("Client is connected...");
    char address_buffer[100];
    getnameinfo((struct sockaddr *)&client_address, client_len, address_buffer, sizeof(address_buffer), 0, 0, NI_NUMERICHOST); // it gets information about our hostname and other strings attached
    printf("%s\n", address_buffer);

    // Client side programming
    printf("Reading request...\n");
    char request[1024];
    int bytes_received = recv(socket_client, request, 1024, 0);
    printf("Received %d bytes.\n", bytes_received);
    printf("%.*s", bytes_received, request);

    // sending our response
    printf("Sending response...\n");
    const char *response = "HTTP/1.1 200 OK\r\n"
                           "Connection: close\r\n"
                           "Content-Type: text/plain\r\n\r\n"
                           "Local time is: ";
    int bytes_sent = send(socket_client, response, strlen(response), 0);
    printf("Sent %d of %d bytes.\n", bytes_sent, (int)strlen(response));

    // the time program
    time_t timer;
    time(&timer);
    char *time_msg = ctime(&timer);
    char *name = "\tMy name is Emmanuel Peter Ameh";
    bytes_sent = send(socket_client, time_msg, strlen(time_msg), 0);
    bytes_sent = send(socket_client, name, strlen(name), 0);
    printf("Sent %d of %d bytes.\n", bytes_sent, (int)strlen(name));

    printf("Closing connection...\n");
    CLOSESOCKET(socket_client);

    printf("Closing listening socket...\n");
    CLOSESOCKET(socket_listen);

#if defined(_WIN32)
    WSACleanup();
#endif

    printf("Finished.\n");

    return 0;
}