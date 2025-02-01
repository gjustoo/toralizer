#include "toralize.h"

// REQUEST STRUCTURE
Req *request(struct sockaddr_in *sock2)
{
    Req *req; // Request pointer

    // Allocating the memory for the struct

    req = malloc(reqsize); // Using the size of the requ

    req->vn = 4; // same as req.vn but when using pointer to structure to grab the reference/value
    req->cd = 1;
    req->dstport = sock2->sin_port;
    req->dstip = sock2->sin_addr.s_addr;
    strncpy(req->userid, USERNAME, 8);

    return req;
}

int connect(int s2,                       // Socket file descriptor
            const struct sockaddr *sock2, // Data to point at
            socklen_t addrlen)
{

    int s;
    struct sockaddr_in sock; // Where specify ip
    Req *req;
    Res *res;
    char buf[ressize]; // buffer for the response
    int success;       // For a Predicate ->function that returns true or false
    char tmp[512];

    int (*p)(int, const struct sockaddr *, socklen_t);

    p = dlsym(RTLD_NEXT, "connect");
    s = socket(AF_INET, SOCK_STREAM, 0);

    if (s < 0)
    {
        perror("socket");
        return -1;
    }

    // Connection to proxy server
    sock.sin_family = AF_INET;
    sock.sin_port = htons(PROXY_PORT);
    sock.sin_addr.s_addr = inet_addr(PROXY_IP);

    if (p(s, (struct sockaddr *)&sock, sizeof(sock)))
    {
        perror("ERROR CONNECTING!");
        return -1;
    }

    printf("Connected to proxy\n");
    req = request((struct sockaddr_in *)sock2); // Pointing to our packet
    write(s, req, reqsize);                     // Sending paquet using write() systemcall

    // Created a buffer for the response and setting that buffered memory space to 0 to get a "clean" space
    memset(buf, 0, ressize);
    if (read(s, buf, ressize) < 1) // read returns number of bytes received. ( Less than 1 is empty response)
    {
        perror("ERROR READING");
        free(req); // free the heap from req
        close(s);
        return -1;
    }

    res = (Res *)buf; // Buff is a string pointer, we need to explicit cast
    success = (res->cd == 90);
    if (!success)
    {
        fprintf(stderr, "Unable to traverse the proxy, error code: %d\n", res->cd);
        close(s);
        free(req);
        return -1;
    }

    printf("Successfully connected through the proxy. \n");

    dup2(s, s2);
    free(req);
    return 0;
}