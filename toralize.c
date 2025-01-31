#include "toralize.h"

// ./toralize XXX.XXX.XXX PORT

// REQUEST STRUCTURE
Req *request(const char *dstip, const int dstport)
{
    Req *req; // Request pointer

    // Allocating the memory for the struct

    req = malloc(reqsize); // Using the size of the requ

    req->vn = 4; // same as req.vn but when using pointer to structure to grab the reference/value
    req->cd = 1;
    req->dstport = htons(dstport);
    req->dstip = inet_addr(dstip);
    strncpy(req->userid, USERNAME, 8);

    return req;
}

int main(int argc, char *argv[]) // argv == array of char pointers.

{
    char *host; // Pointer to host name connecting to
    int port, s;
    struct sockaddr_in sock; // Where specify ip
    Req *req;
    Res *res;
    char buf[ressize]; // buffer for the response
    int success;       // For a Predicate ->function that returns true or false
    char tmp[512];

    // Check arg quantity

    if (argc < 3)
    {
        fprintf(stderr, "Usage: %s <host> <port>\n", argv[0]);
        return -1;
    }

    host = argv[1];
    port = atoi(argv[2]);

    s = socket(AF_INET, SOCK_STREAM, 0);

    if (s < 0)
    {
        perror("socket");
        return -1;
    }

    sock.sin_family = AF_INET;
    sock.sin_port = htons(PROXY_PORT);
    sock.sin_addr.s_addr = inet_addr(PROXY_IP);

    printf("sock details:\n");
    printf("Family: %d\n", sock.sin_family);
    printf("Port: %d\n", ntohs(sock.sin_port));
    printf("Address: %s\n", inet_ntoa(sock.sin_addr));

    printf("trying to connect... \n");

    if (connect(s, (struct sockaddr *)&sock, sizeof(sock)))
    {
        perror("ERROR CONNECTING!");
        return -1;
    }

    printf("Connected to proxy\n");

    req = request(host, port); // Pointing to our packet
    write(s, req, reqsize);    // Sending paquet using write() systemcall

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

    printf("Successfully connected through the proxy to %s:%d\n", host, port);

    memset(tmp, 0, 512);
    snprintf(tmp, 511,
             "HEAD / HTTP/1.0\r\n"
             "Host: www.google.com\r\n"
             "\r\n");

    write(s, tmp, strlen(tmp));
    memset(tmp, 0, 512);

    read(s, tmp, 511);
    printf("'%s'\n", tmp);

    close(s);
    free(req);
    return 0;
}