#include <arpa/inet.h>
#include <assert.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdio.h> // stderr
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define MAXBUFFLEN 1023

// assert grather than 0
#ifndef assert_gt0
#define assert_gt0(status)                            \
    {                                                 \
        if ((status) < 0)                             \
        {                                             \
            fprintf(stderr, "%s: ", __FILE__);        \
            fprintf(stderr, "%s: ", __FUNCTION__);    \
            fprintf(stderr, "%d: ", __LINE__ - 4);    \
            fprintf(stderr, "%s\n", strerror(errno)); \
            exit(~0);                                 \
        }                                             \
    }
#endif

int
main(int argc, char *argv[])
{
    struct sockaddr_in sock_in;
    socklen_t          addr_len;
    int                sockfd;
    int                clientfd;
    int                sent_n;
    int                port;
    char               buffer[MAXBUFFLEN];
    char               client_ip[INET_ADDRSTRLEN];

    if (argc != 2)
    {
        fprintf(stderr, "Puerto no definido\n");
        // exit(~0);
        puts("Usando 5001");
        port = 5001;
    }
    else
        port = atoi(argv[1]);

    // crear el socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    assert_gt0(sockfd);

    // bind
    sock_in.sin_family      = AF_INET;
    sock_in.sin_addr.s_addr = htonl(INADDR_ANY);
    sock_in.sin_port        = htons(port);
    assert_gt0(bind(sockfd, (struct sockaddr *) &sock_in, sizeof(struct sockaddr_in)));

    // listen
    assert_gt0(listen(sockfd, 1));

    while (1)
    {
        addr_len = sizeof(struct sockaddr_in);

        // acept
        assert_gt0(clientfd = accept(sockfd, (struct sockaddr *) &sock_in, &addr_len));

        inet_ntop(AF_INET, &sock_in.sin_addr, client_ip, addr_len);
        printf("Nueva conexion: IP:%s\n", client_ip);

        // send
        strcpy(buffer, "Hello from server!\n");
        assert_gt0(sent_n = send(clientfd, buffer, strlen(buffer) + 1, 0));

    }

    close(sockfd);

    return 0;
}
