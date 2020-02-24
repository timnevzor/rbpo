#pragma once

#include "config.h"

struct clientInfo {
    int Socket;
    sockaddr_in * pSocAddr;
};

extern atomic_int threadCounter;

int CreateListenSocket ( uint32_t addr, uint16_t port );
clientInfo WaitConnect ( int Socket );
void ServerWorker ( clientInfo cl );
void SendString ( int Socket, string s );
