#include "chap04.h" // Including the library neeeded

int main(int argc, char const *argv[])
{
#if defined(_WIN32) // initializing winsock
    WSADATA d;
    if (WSAStartup(MAKEWORD(2, 2), &d))
    {
        fprintf(stderr, "Failed to initialize.\n");
        return 1;
    }
#endif
    printf("Configuring local address...\n");
    struct addrinfo hints;            // variable hints holds the different characteristics of our socket
    memset(&hints, 0, sizeof(hints)); // setting the varia ble inside the struct hints to zero
    hints.ai_family = AF_INET;        // specifying that we're using an IPv4 connection
    hints.ai_socktype = SOCK_DGRAM;   // specifying that we're using a UDP connection
    hints.ai_flags = AI_PASSIVE;      // telling the getaddrinfo() function that we want to bind to the supplied address

    struct addrinfo *bind_address;                 // specifying a bind address that will hold all the information like the port number and and the address we're binding to
    getaddrinfo(0, "8080", &hints, &bind_address); // the function copies the port number and other properties in hints into the  bindaddress variable

    // socket creation
    SOCKET socket_listen;
    socket_listen = socket(bind_address->ai_family, bind_address->ai_socktype, bind_address->ai_protocol); // specifying the properties that our socket must possess
    if (!ISVALIDSOCKET(socket_listen))
    {
        fprintf(stderr, "socket () failed. (%d)\n", GETSOCKETERRNO());
        return 1;
    }
    printf("Binding socket to local address....\n");
    if (bind(socket_listen, bind_address->ai_addr, bind_address->ai_addrlen)) // binding our address with the created socket
    {
        fprintf(stderr, "bind() failed. (%d)\n", GETSOCKETERRNO());
        return 1;
    }
    freeaddrinfo(bind_address);

    struct sockaddr_storage client_address;
    socklen_t client_len = sizeof(client_address);
    char read[1024];
    int bytes_received = recvfrom(socket_listen, read, 1024, 0, (struct sockaddr *)&client_address, &client_len); // storing information received from listening socket into the read variable

    printf("Received (%d bytes): %.*s\n", bytes_received, bytes_received, read);

    printf("Remote address is: ");
    char address_buffer[100];
    char service_buffer[100];
    getnameinfo(((struct sockaddr *)&client_address), client_len, address_buffer, sizeof(address_buffer), service_buffer, sizeof(service_buffer), NI_NUMERICHOST | NI_NUMERICSERV); // getting the address info and service info for printing to terminal
    printf("%s %s\n", address_buffer, service_buffer);

    CLOSESOCKET(socket_listen);
#if defined(_WIN32)
    WSACleanup();
#endif
    printf("Finished.\n");
    return 0;
}
