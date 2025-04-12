#include <stdio.h>
#include <windows.h>
#include "sysaccess_cpu.h"
#include "sysaccess_ram.h"
#include "sysaccess_user.h"
#include "sysaccess_system.h"
#include "sysaccess_disk.h"

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
    if (len == 0 || len >= MAX_PATH) {
        return "Unknown";
    }

    return path;
}

const char* sysaccess_get_executable_path(void) {
    static char exe_path[MAX_PATH] = {0};

    const DWORD len = GetModuleFileNameA(NULL, exe_path, MAX_PATH);
    if (len == 0 || len == MAX_PATH) {
        return "Unknown";
    }

    return exe_path;
}

void sysaccess_print_system_info(void) {
    sysaccess_print_system_info_to(stdout);
}

int sysaccess_log_to_file(const char* filepath) {
    FILE* f = fopen(filepath, "w");
    if (!f) return 0;

    sysaccess_print_system_info_to(f);
    fclose(f);
    return 1;
}

void sysaccess_print_system_info_to(FILE* out) {
    const char* cpu = sysaccess_get_cpu_info();
    const float cpu_usage = sysaccess_cpu_usage();
    const unsigned int cores = sysaccess_get_cpu_cores();
    const char* arch = sysaccess_get_architecture();

    const float ram_usage = sysaccess_ram_usage();
    const unsigned long long total_ram = sysaccess_total_ram();

    const char* user = sysaccess_get_username();
    const char* host = sysaccess_get_hostname();
    const unsigned long long uptime = sysaccess_get_uptime();
    const int is_admin = sysaccess_is_admin();
    const unsigned int pid = sysaccess_get_process_id();
    const char* cwd = sysaccess_get_current_directory();
    const char* exe = sysaccess_get_executable_path();
    const char* drives = sysaccess_list_logical_drives();
    const char* drive_type = sysaccess_get_drive_type("C:\\");

    unsigned long long total_disk = 0, free_disk = 0;
    const int disk_ok = sysaccess_get_disk_usage("C:\\", &total_disk, &free_disk);

    fprintf(out, "\n+-------------------------------+\n");
    fprintf(out, "|     sysaccess diagnostics     |\n");
    fprintf(out, "+-------------------------------+\n");
    fprintf(out, "CPU           : %s\n",                  cpu);
    fprintf(out, "CPU Usage     : %.2f%%\n",              cpu_usage);
    fprintf(out, "CPU Cores     : %u\n",                  cores);
    fprintf(out, "Architecture  : %s\n",                  arch);
    fprintf(out, "RAM Usage     : %.2f%%\n",              ram_usage);
    fprintf(out, "Total RAM     : %.2f GB\n",             (double)total_ram / (1024 * 1024 * 1024));
    fprintf(out, "User          : %s @ %s (%s)\n",        user, host, is_admin ? "Administrator" : "Standard User");
    fprintf(out, "Uptime        : %.2f hours\n",          (double)uptime / (1000 * 60 * 60));
    fprintf(out, "Process ID    : %u\n",                  pid);
    fprintf(out, "Executable    : %s\n",                  exe);
    fprintf(out, "Current Dir   : %s\n",                  cwd);
    fprintf(out, "Drives        : %s\n",                  drives ? drives : "Unavailable");
    fprintf(out, "Drive C: Type : %s\n",                  drive_type);

    if (disk_ok) {
        fprintf(out, "Disk (C:)     : %.2f GB free / %.2f GB total\n",
                (double)free_disk / (1024 * 1024 * 1024),
                (double)total_disk / (1024 * 1024 * 1024));
    } else {
        fprintf(out, "Disk (C:)     : Failed to retrieve.\n");
    }
}
