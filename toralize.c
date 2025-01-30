#include "toralize.h"

// ./toralize XXX.XXX.XXX PORT

int main(int argc, char *argv[])
{
    char *host; // Pointer to host name connecting to
    int port, s;
    struct sockaddr_in sock; // Where specify ip

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

    close(s);

    return 0;
}