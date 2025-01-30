//====== DECLARATIONS ======

// INCLUDES
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

// CONSTANTS
#define PROXY_IP "127.0.0.1"
#define PROXY_PORT 9050

// Type definition for the packet byte size management
typedef unsigned char int8;
typedef unsigned short int int16;
typedef unsigned int int32;

// 	           	+----+----+----+----+----+----+----+----+----+----+....+----+
// 	           	| VN | CD | DSTPORT |      DSTIP        | USERID       |NULL|
// 	           	+----+----+----+----+----+----+----+----+----+----+....+----+
//  # of bytes:	   1    1      2              4           variable       1

struct proxy_request
{
    // Request protocol SOCKSv4
    int8 vn;
    int8 cd;
    int16 dstport;
    int32 dstip;
    unsigned char userid[8];
};

typedef struct proxy_request Req;
/*

                +----+----+----+----+----+----+----+----+----+----+....+----+
                | VN | CD | DSTPORT |      DSTIP        | USERID       |NULL|
                +----+----+----+----+----+----+----+----+----+----+....+----+
 # of bytes:	   1    1      2              4           variable       1

*/

struct proxy_response
{

    int8 vn;
    int8 cd;
    int16 _;  // This values are ignored thus the underscore naming
    int32 __; // This values are ignored thus the underscore naming
    unsigned char userid[8];
};

typedef struct proxy_reponse Res;