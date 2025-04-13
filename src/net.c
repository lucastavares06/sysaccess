#include <winsock2.h>
#include <windows.h>
#include <iphlpapi.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <wininet.h>
#include "sysaccess_net.h"

const char* sysaccess_get_local_ip(void) {
    static char ip_str[INET_ADDRSTRLEN] = "Unavailable";

    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        return ip_str;

    char hostname[256];
    if (gethostname(hostname, sizeof(hostname)) == SOCKET_ERROR) {
        WSACleanup();
        return ip_str;
    }

    struct addrinfo hints = {0}, *res = NULL;
    hints.ai_family = AF_INET; // IPv4
    if (getaddrinfo(hostname, NULL, &hints, &res) != 0) {
        WSACleanup();
        return ip_str;
    }

    struct sockaddr_in* addr = (struct sockaddr_in*)res->ai_addr;
    inet_ntop(AF_INET, &(addr->sin_addr), ip_str, sizeof(ip_str));

    freeaddrinfo(res);
    WSACleanup();
    return ip_str;
}

const char* sysaccess_get_mac_address(void) {
    static char mac_str[18] = "Unavailable";
    IP_ADAPTER_INFO adapter_info[16];
    DWORD buflen = sizeof(adapter_info);

    if (GetAdaptersInfo(adapter_info, &buflen) != ERROR_SUCCESS)
        return mac_str;

    PIP_ADAPTER_INFO adapter = adapter_info;
    while (adapter) {
        if (adapter->Type == MIB_IF_TYPE_ETHERNET && adapter->AddressLength == 6) {
            sprintf(mac_str, "%02X:%02X:%02X:%02X:%02X:%02X",
                    adapter->Address[0], adapter->Address[1], adapter->Address[2],
                    adapter->Address[3], adapter->Address[4], adapter->Address[5]);
            return mac_str;
        }
        adapter = adapter->Next;
    }

    return mac_str;
}

int sysaccess_is_connected(void) {
    DWORD flags = 0;
    return InternetGetConnectedState(&flags, 0);
}
