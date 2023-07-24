#pragma once

#include <Windows.h>
#include <WS2tcpip.h>
#include <WinSock2.h>
#include <iphlpapi.h>

#include <fcntl.h>
#include <filesystem>
#include <format>
#include <fstream>
#include <io.h>
#include <iostream>
#include <mutex>
#include <queue>
#include <random>
#include <sstream>

#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Iphlpapi.lib")
