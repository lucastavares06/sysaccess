#include <windows.h>
#include "sysaccess_ram.h"

float sysaccess_ram_usage(void) {
    MEMORYSTATUSEX memInfo;
    memInfo.dwLength = sizeof(memInfo);

    if (GlobalMemoryStatusEx(&memInfo)) {
        const DWORDLONG total = memInfo.ullTotalPhys;
        const DWORDLONG avail = memInfo.ullAvailPhys;
        return 100.0f * (1.0f - (float)avail / (float)total);
    }

    return -1.0f;
}

unsigned long long sysaccess_total_ram(void) {
    MEMORYSTATUSEX memInfo;
    memInfo.dwLength = sizeof(memInfo);

    if (GlobalMemoryStatusEx(&memInfo)) {
        return memInfo.ullTotalPhys;
    }

    return 0;
}
