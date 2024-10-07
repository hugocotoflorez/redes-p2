#include <arpa/inet.h>
#include <assert.h>
#include <errno.h>
#include <inttypes.h>
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
// raise error if status is not greater than 0
#define assert_gt0(status)                             \
    {                                                  \
        if ((status) <= 0)                             \
        {                                              \
            fprintf(stderr, "%s:", __FILE__);          \
            fprintf(stderr, "%s:", __FUNCTION__);      \
            fprintf(stderr, #status);                  \
            fprintf(stderr, ":%s\n", strerror(errno)); \
            exit(~0);                                  \
        }                                              \
    }
#endif

#ifndef assert_get0
// raise error if status is not greater or equal than 0
#define assert_get0(status)                            \
    {                                                  \
        if ((status) < 0)                              \
        {                                              \
            fprintf(stderr, "%s:", __FILE__);          \
            fprintf(stderr, "%s:", __FUNCTION__);      \
            fprintf(stderr, #status);                  \
            fprintf(stderr, ":%s\n", strerror(errno)); \
            exit(~0);                                  \
        }                                              \
    }
#endif

int
main(int argc, char *argv[])
{
    struct sockaddr_in sock_in;
    int                sockfd;
    ssize_t            n;
    char               buff[MAXBUFFLEN];

    if (argc != 3)
    {
        fprintf(stderr, "Addr Port\n");
        exit(~0);
    }


    /* Se define la informacion del socket */
    sock_in.sin_family = AF_INET;              // IPv4
    sock_in.sin_port   = htons(atoi(argv[2])); // Puerto en orden de host
    inet_pton(AF_INET, argv[1], &sock_in.sin_addr); // Direccion ip

    /* Se crea el socket
     * Si el valor de retorno es <= 0 da error */
    assert_get0(sockfd = socket(AF_INET, SOCK_STREAM, 0));

    /* Se crea una conexion con el servidor
     * Devuelve 0 si no hay error o < 0 si hay error */
    assert_get0(connect(sockfd, (struct sockaddr *) &sock_in, sizeof(struct sockaddr_in)));

    /* duerme 2 segundos para que el servidor pueda
     * enviar dos mensajes. (Parte 2 practica) */
    // sleep(2);

    while (1)
    {
        // n = recv(sockfd, buff, MAXBUFFLEN, 0);
        while ((n = recv(sockfd, buff, MAXBUFFLEN, 0)) > 0)
        {
            if (n < 0)
            {
                perror("recv");
                exit(~0);
            }

            if (n == 0)
            {
                printf("Conexion cerrada\n");
                break;
            }

            printf("%s", buff);
            printf(" (%zd bytes recibidos)\n", n); // zd es para ssize_t

        }
    }
    close(sockfd);

    return 0;
}
