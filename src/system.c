#include <windows.h>
#include <stdio.h>
#include "sysaccess_system.h"

unsigned long long sysaccess_get_uptime(void) {
    return GetTickCount64();
}

int sysaccess_is_admin(void) {
    BOOL is_admin = FALSE;
    PSID admin_group = NULL;

    SID_IDENTIFIER_AUTHORITY nt_authority = SECURITY_NT_AUTHORITY;
    if (AllocateAndInitializeSid(&nt_authority, 2,
                                 SECURITY_BUILTIN_DOMAIN_RID,
                                 DOMAIN_ALIAS_RID_ADMINS,
                                 0, 0, 0, 0, 0, 0,
                                 &admin_group)) {
        CheckTokenMembership(NULL, admin_group, &is_admin);
        FreeSid(admin_group);
                                 }

    return is_admin ? 1 : 0;
}

unsigned int sysaccess_get_process_id(void) {
    return GetCurrentProcessId();
}

const char* sysaccess_get_current_directory(void) {
    static char path[MAX_PATH] = {0};
    const DWORD len = GetCurrentDirectoryA(MAX_PATH, path);
    return len == 0 || len >= MAX_PATH ? "Unknown" : path;
}

const char* sysaccess_get_executable_path(void) {
    static char exe_path[MAX_PATH] = {0};
    const DWORD len = GetModuleFileNameA(NULL, exe_path, MAX_PATH);
    return len == 0 || len == MAX_PATH ? "Unknown" : exe_path;
}
