#include "network_libraries.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <clock.h>
#if defined(_WIN32)
#define ISVALIDSOCKET(s) ((s) != INVALID_SOCKET)
#define CLOSESOCKET(s) closesocket(s)
#define GETSOCKETERRNO() (WSAGetLastError())
#endif
