#include <windows.h>
#include "sysaccess_disk.h"

int sysaccess_get_disk_usage(const char* drive, unsigned long long* total, unsigned long long* free) {
    ULARGE_INTEGER free_bytes, total_bytes, dummy;

    if (GetDiskFreeSpaceExA(drive, &free_bytes, &total_bytes, &dummy)) {
        *total = total_bytes.QuadPart;
        *free = free_bytes.QuadPart;
        return 1;
    }

    *total = 0;
    *free = 0;
    return 0;
}

const char* sysaccess_list_logical_drives(void) {
    static char formatted[256] = {0};
    char buffer[256] = {0};

    const DWORD result = GetLogicalDriveStringsA(sizeof(buffer) - 1, buffer);
    if (result == 0 || result > sizeof(buffer)) {
        return NULL;
    }

    const char* ptr = buffer;
    formatted[0] = '\0';

    while (*ptr) {
        strncat(formatted, ptr, strlen(ptr));
        strcat(formatted, " ");
        ptr += strlen(ptr) + 1;
    }

    const size_t len = strlen(formatted);
    if (len > 0 && formatted[len - 1] == ' ') {
        formatted[len - 1] = '\0';
    }

    return formatted;
}

const char* sysaccess_get_drive_type(const char* drive) {
    const UINT type = GetDriveTypeA(drive);
    switch (type) {
        case DRIVE_REMOVABLE:   return "Removable";
        case DRIVE_FIXED:       return "Fixed";
        case DRIVE_REMOTE:      return "Network";
        case DRIVE_CDROM:       return "CD-ROM";
        case DRIVE_RAMDISK:     return "RAM Disk";
        case DRIVE_NO_ROOT_DIR: return "Invalid";
        default:                return "Unknown";
    }
}
