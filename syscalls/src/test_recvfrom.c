#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 12345

void test_recvmsg() {
    int sockfd;
    struct sockaddr_in servaddr, cliaddr;
    char buffer[1024];
    // Create socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket creation failed");
        return;
    }
    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));
    // Fill server information
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);
    // Bind the socket with the server address
    if (bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("bind failed");
        return;
    }
    // Create message structure
    struct msghdr msg;
    memset(&msg, 0, sizeof(msg));
    // Set source address
    msg.msg_name = &cliaddr;
    msg.msg_namelen = sizeof(cliaddr);
    // Set message buffer
    struct iovec iov;
    iov.iov_base = buffer;
    iov.iov_len = sizeof(buffer);
    msg.msg_iov = &iov;
    msg.msg_iovlen = 1;
    // Receive message from client
    ssize_t n = recvmsg(sockfd, &msg, 0);
    buffer[n] = '\0';
    printf("Received message: %s\n", buffer);
    close(sockfd);
}

void test_sendmsg() {
    int sockfd;
    struct sockaddr_in servaddr;
    char buffer[1024] = "Hello from client";
    // Create socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket creation failed");
        return;
    }
    memset(&servaddr, 0, sizeof(servaddr));
    // Fill server information
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = INADDR_ANY;
    // Create message structure
    struct msghdr msg;
    memset(&msg, 0, sizeof(msg));
    // Set destination address
    msg.msg_name = &servaddr;
    msg.msg_namelen = sizeof(servaddr);
    // Set message buffer
    struct iovec iov;
    iov.iov_base = buffer;
    iov.iov_len = strlen(buffer);
    msg.msg_iov = &iov;
    msg.msg_iovlen = 1;
    // Send message to server
    ssize_t n = sendmsg(sockfd, &msg, 0);
    if (n < 0) {
        perror("sendmsg failed");
        return;
    }
    printf("Hello message sent.\n");
    close(sockfd);
}

void test_recvmmsg() {
    int sockfd;
    struct sockaddr_in servaddr, cliaddr;
    char buffer[1024];
    // Create socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket creation failed");
        return;
    }
    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));
    // Fill server information
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);
    // Bind the socket with the server address
    if (bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("bind failed");
        return;
    }
    // Create message structure
    struct mmsghdr msg[1];
    memset(&msg, 0, sizeof(msg));
    // Set source address
    msg[0].msg_hdr.msg_name = &cliaddr;
    msg[0].msg_hdr.msg_namelen = sizeof(cliaddr);
    // Set message buffer
    struct iovec iov;
    iov.iov_base = buffer;
    iov.iov_len = sizeof(buffer);
    msg[0].msg_hdr.msg_iov = &iov;
    msg[0].msg_hdr.msg_iovlen = 1;
    // Receive message from client
    ssize_t n = recvmmsg(sockfd, msg, 1, 0, NULL);
    buffer[n] = '\0';
    printf("Received message: %s\n", buffer);
    close(sockfd);
}

void test_sendmmsg() {
    int sockfd;
    struct sockaddr_in servaddr;
    char buffer[1024] = "Hello from client";
    // Create socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket creation failed");
        return;
    }
    memset(&servaddr, 0, sizeof(servaddr));
    // Fill server information
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = INADDR_ANY;
    // Create message structure
    struct mmsghdr msg[1];
    memset(&msg, 0, sizeof(msg));
    // Set destination address
    msg[0].msg_hdr.msg_name = &servaddr;
    msg[0].msg_hdr.msg_namelen = sizeof(servaddr);
    // Set message buffer
    struct iovec iov;
    iov.iov_base = buffer;
    iov.iov_len = strlen(buffer);
    msg[0].msg_hdr.msg_iov = &iov;
    msg[0].msg_hdr.msg_iovlen = 1;
    // Send message to server
    ssize_t n = sendmmsg(sockfd, msg, 1, 0);
    if (n < 0) {
        perror("sendmmsg failed");
        return;
    }
    printf("Hello message sent.\n");
    close(sockfd);
}

void test_recvfrom() {
    int sockfd;
    struct sockaddr_in servaddr, cliaddr;
    char buffer[1024];
    socklen_t len;

    // Create socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket creation failed");
        return;
    }

    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));

    // Fill server information
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);

    // Bind the socket with the server address
    if (bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("bind failed");
        return;
    }

    len = sizeof(cliaddr);

    // Receive message from client
    ssize_t n = recvfrom(sockfd, (char *)buffer, sizeof(buffer), MSG_WAITALL, (struct sockaddr *)&cliaddr, &len);
    buffer[n] = '\0';

    printf("Received message: %s\n", buffer);

    close(sockfd);
}

void test_sendto() {
    int sockfd;
    struct sockaddr_in servaddr;
    char buffer[1024] = "Hello from client";

    // Create socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket creation failed");
        return;
    }

    memset(&servaddr, 0, sizeof(servaddr));

    // Fill server information
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = INADDR_ANY;

    // Send message to server
    sendto(sockfd, (const char *)buffer, strlen(buffer), MSG_CONFIRM, (const struct sockaddr *)&servaddr, sizeof(servaddr));

    printf("Hello message sent.\n");

    close(sockfd);
}

struct test_case {
    void (*recv_func)();
    void (*send_func)();
    const char *test_name;
} test_cases[] = {
    {test_recvmsg, test_sendmsg, "recvmsg/sendmsg"},
    {test_recvmmsg, test_sendmmsg, "recvmmsg/sendmmsg"},
    {test_recvfrom, test_sendto, "recvfrom/sendto"},
};

void show_usage(char *name) {
    printf("Usage: %s <server|client> <test_case>\n", name);
}

// parse args and run test
int main(int argc, char *argv[]) {
    printf("Available test cases:\n");
    for (int i = 0; i < sizeof(test_cases) / sizeof(test_cases[0]); i++) {
        printf("%d. %s\n", i, test_cases[i].test_name);
    }

    printf("\n");
    printf("cmdline args: %d\n", argc);
    if (argc < 2) {
        show_usage(argv[0]);
        return -1;
    }

    int idx = 0;
    printf("Selected test case: %s\n", argv[2]);
    idx = atoi(argv[2]);

    printf("Selected test case: %d\n", idx);
    printf("Running test case: %s\n", test_cases[idx].test_name);
    if (strcmp(argv[1], "server") == 0)
        test_cases[idx].send_func();
    else if (strcmp(argv[1], "client") == 0)
        test_cases[idx].recv_func();
    else
        show_usage(argv[0]);

    return 0;
}