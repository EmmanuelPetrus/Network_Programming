// program to get network adapters
#include "network_libraries.h"

int main(int argc, char const *argv[])
{
    WSADATA d;

    if (WSAStartup(MAKEWORD(2, 2), &d))
    {
        printf("Failed to initialize.\n");
        return -1;
    }
    // allocate memory for the adapter

    DWORD asize = 20000;
    PIP_ADAPTER_ADDRESSES adapters;
    do
    {
        adapters = (PIP_ADAPTER_ADDRESSES)malloc(asize);

        if (!adapters)
        {
            printf("Couldn't allocate %ld bytes for adapters.\n", asize);
            WSACleanup();
            return -1;
        }
        int r = GetAdaptersAddresses(AF_UNSPEC, GAA_FLAG_INCLUDE_PREFIX, 0, adapters, &asize);
        if (r == ERROR_BUFFER_OVERFLOW)
        {
            printf("GetAdaptersAddressses wants %ld bytes.\n", asize);
            free(adapters);
        }
        else if (r == ERROR_SUCCESS)
            break;
        else
        {
            printf("Error from GetAdaptersAddresses. %d\n", r);
            free(adapters);
            WSACleanup();
            return -1;
        }

    } while (!adapters);

    PIP_ADAPTER_ADDRESSES adapter = adapters;
    while (adapter)
    {
        printf("\n Adapter name: %s\n", adapter->FriendlyName);

        PIP_ADAPTER_UNICAST_ADDRESS address = adapter->FirstUnicastAddress;
        while (address)
        {
            printf("\t%s", address->Address.lpSockaddr->sa_family == AF_INET ? "IPv4" : "IPv6");

            char ap[100];

            getnameinfo(address->Address.lpSockaddr, address->Address.iSockaddrLength, ap, sizeof(ap), 0, 0, NI_NUMERICHOST);
            printf("\t%s\n", ap);

            address = address->Next;
            adapter = adapter->Next;
        }
    }

    free(adapters);
    WSACleanup();
    return 0;
}
