#include "stdafx.h"

#include "WinsockInitializer.h"

WinsockInitializer::~WinsockInitializer()
{
    WSACleanup();
}

int WinsockInitializer::Initialize()
{
    WSADATA wsaData;
    return WSAStartup(MAKEWORD(2, 2), &wsaData);
}
