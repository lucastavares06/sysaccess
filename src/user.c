#include <windows.h>
#include "sysaccess_user.h"

static char user_buffer[256];
static char host_buffer[256];

const char* sysaccess_get_username(void) {
    DWORD size = sizeof(user_buffer);
    if (GetUserNameA(user_buffer, &size)) {
        return user_buffer;
    }
    return "Unknown User";
}

const char* sysaccess_get_hostname(void) {
    DWORD size = sizeof(host_buffer);
    if (GetComputerNameA(host_buffer, &size)) {
        return host_buffer;
    }
    return "Unknown Host";
}
