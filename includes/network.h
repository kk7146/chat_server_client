#ifndef NETWORK_H
#define NETWORK_H

#include <stdint.h>
#include "packet.h"
#include <sys/types.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 12345

int connect_to_server();  // 소켓 연결
int send_packet(int sockfd, PacketHeader *header, const void *payload);
int recv_packet(int sockfd, PacketHeader *header, void *buf, size_t bufsize);

#endif