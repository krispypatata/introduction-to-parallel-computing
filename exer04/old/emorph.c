

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct sockaddr
{
    unsigned short sa_family;
    char sa_data[14];
};

struct sockaddr_in
{
    short sin_family;
    unsigned short sin_port; // Port Number
    struct in_addr sin_addr; // IP Address
    char sin_zero[8];
};

struct in_addr
{
    unsigned long s_addr; // 4 bytes long
};


