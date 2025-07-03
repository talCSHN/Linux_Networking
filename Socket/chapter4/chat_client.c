#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/select.h>

#define BUF_SIZE 1024
void error_handling(char *message);

int main(int argc, char *argv[])
{
    int sock;
    char message[BUF_SIZE];
    int str_len;
    struct sockaddr_in serv_addr;

    fd_set reads, cpy_reads;
    int fd_max;

    if (argc != 3) {
        printf("Usage: %s <IP> <PORT>\n", argv[0]);
        exit(1);
    }

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock == -1)
        error_handling("socket() error");

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
        error_handling("connect() error");
    else
        puts("Connected to server.");

    FD_ZERO(&reads);
    FD_SET(0, &reads);      // 표준입력(stdin)
    FD_SET(sock, &reads);   // 소켓

    fd_max = sock > 0 ? sock : 0;

    while (1) {
        cpy_reads = reads;
        if (select(fd_max + 1, &cpy_reads, NULL, NULL, NULL) == -1)
            break;

        // 사용자 입력 감지
        if (FD_ISSET(0, &cpy_reads)) {
            fgets(message, BUF_SIZE, stdin);

            if (!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
                break;

            write(sock, message, strlen(message));
        }

        // 서버 메시지 수신 감지
        if (FD_ISSET(sock, &cpy_reads)) {
            str_len = read(sock, message, BUF_SIZE - 1);
            if (str_len == 0) {
                puts("Server closed connection.");
                break;
            }

            message[str_len] = '\0';
            printf("Message from another client: %s", message);
        }
    }

    close(sock);
    return 0;
}

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
