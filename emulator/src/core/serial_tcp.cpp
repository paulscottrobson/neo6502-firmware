// *******************************************************************************************************************************
// *******************************************************************************************************************************
//
//		Name:		serial_tcp.cpp
//		Purpose:	Serial Port emulation via TCP
//		Created:	6th Septemper 2024
//		Author:		Joonas Viskari (joonas.viskari@gmail.com)
//
// *******************************************************************************************************************************
// *******************************************************************************************************************************


#include "serial_emu.h"

#ifdef LINUX
// Only supported on Linux at the moment
#define TCPSERIAL_SUPPORT
#endif

#include <stdio.h>
#include <string.h>
#include <stdio.h>

#ifdef TCPSERIAL_SUPPORT

#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ctype.h> // isprint
#define TCP_PORT 25232
#define BUFFER_SIZE 1024

// IP232 (TCP client wrapper) implementation
static int sockfd = -1;
static struct sockaddr_in server_addr;
static char buffer[BUFFER_SIZE];
static int buffer_pos = 0;
static int buffer_size = 0;


static char debug_printable(char c) {
    if (isprint((unsigned char)c)) {
        return c;
    } else {
        return ' ';
    }
}


static bool tcpsByteAvailableImpl(void) {
    if (buffer_pos < buffer_size) {
        return true;
    }

    buffer_pos = 0;
    buffer_size = recv(sockfd, buffer, BUFFER_SIZE, MSG_DONTWAIT);
    if (buffer_size > 0) {
        return true;
    }
    return false;
}

static uint8_t tcpReadByteImpl(void) {

    if (tcpsByteAvailableImpl()) {
        if (buffer_pos < buffer_size) {
            char c = buffer[buffer_pos];
            printf("TCP(r):  %02X | %c \n", c, debug_printable(c));
            return (uint8_t)buffer[buffer_pos++];
        }
    }
    return 0;
}

static void tcpWriteByteImpl(uint8_t b) {
    if (sockfd >= 0) {
        char byte = (char)b;
        send(sockfd, &byte, 1, MSG_DONTWAIT);
    }
    printf("TCP(w): %02X | %c \n", b, debug_printable(b));
}

static void tcpSetSerialFormatImpl(uint32_t baudRate, uint32_t protocol) {
    printf("TCP Setting Serial to %d baud protocol %d.\n", baudRate, protocol);
}

static void tcpCleanupImpl(void) {
    if (sockfd >= 0) {
        close(sockfd);
        sockfd = -1;
    }
}

static bool openTcpSocket() {
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Error creating socket");
        return false;
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_port = htons(TCP_PORT);

    if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Error connecting to server");
        close(sockfd);
        sockfd = -1;
        return false;
    }

    int flags = fcntl(sockfd, F_GETFL, 0);
    fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);

    printf("TCP Connected to server on port %d\n", TCP_PORT);
    return true;
}


// Static SerialInterface instances
static const SerialInterface gSerialInterfaceIP232 = {
    .isByteAvailable = tcpsByteAvailableImpl,
    .readByte = tcpReadByteImpl,
    .writeByte = tcpWriteByteImpl,
    .setSerialFormat = tcpSetSerialFormatImpl,
    .cleanup = tcpCleanupImpl
};
#endif

// STUB implementation
static bool stubIsByteAvailableImpl(void) {
    return false;
}

static uint8_t stubReadByteImpl(void) {
    return 0;
}

static void stubWriteByteImpl(uint8_t b) {
    (void)b;
    printf("STUB Serial write %d\n", b);
}

static void stubSetSerialFormatImpl(uint32_t baudRate, uint32_t protocol) {
    (void)baudRate;
    (void)protocol;
    printf("STUB Setting Serial to %d baud protocol %d.\n", baudRate, protocol);
}

static void stubCleanupImpl(void) {
    // Nothing to clean up for stub
}

static const SerialInterface gSerialInterfaceStub = {
    .isByteAvailable = stubIsByteAvailableImpl,
    .readByte = stubReadByteImpl,
    .writeByte = stubWriteByteImpl,
    .setSerialFormat = stubSetSerialFormatImpl,
    .cleanup = stubCleanupImpl
};


// Function to open and return the appropriate SerialInterface
const SerialInterface* SerialInterfaceOpen(enum SerialType type) {
    switch (type) {
        case SER_TCP:
#ifdef TCPSERIAL_SUPPORT        
            if (openTcpSocket()) {
                return &gSerialInterfaceIP232;
            }
#endif            
            printf("Failed to open TCP socket. Falling back to STUB implementation.\n");
            // Fall through to SER_STUB if socket opening fails
        case SER_STUB:
            return &gSerialInterfaceStub;
        default:
            return NULL;
    }
}
