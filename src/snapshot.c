#include <stdio.h>
#include <string.h>
#include "sysaccess_snapshot.h"
#include "sysaccess_cpu.h"
#include "sysaccess_ram.h"
#include "sysaccess_user.h"
#include "sysaccess_system.h"
#include "sysaccess_disk.h"
#include "sysaccess_net.h"

int sysaccess_snapshot(SysSnapshot* out) {
    if (!out) return 0;
    memset(out, 0, sizeof(SysSnapshot));

    out->username               = sysaccess_get_username();
    out->hostname               = sysaccess_get_hostname();
    out->architecture           = sysaccess_get_architecture();
    out->cpu_model              = sysaccess_get_cpu_info();
    out->cpu_usage              = sysaccess_cpu_usage();
    out->cpu_cores              = sysaccess_get_cpu_cores();
    out->cpu_clock_mhz          = sysaccess_get_cpu_clock_mhz();
    out->cpu_clock_live_mhz     = sysaccess_get_cpu_clock_live_mhz();
    out->cpu_threads_per_core   = sysaccess_get_cpu_threads_per_core();
    out->ram_usage              = sysaccess_ram_usage();
    out->ram_total              = sysaccess_total_ram();
    out->ram_available          = sysaccess_ram_available_bytes();
    out->ram_used               = sysaccess_ram_used_bytes();

    const char* raw_drives = sysaccess_list_logical_drives();
    if (raw_drives) {
        char buffer[256];
        strncpy(buffer, raw_drives, sizeof(buffer));
        buffer[sizeof(buffer) - 1] = '\0';

        int i = 0;
        const char* token = strtok(buffer, " ");
        while (token && i < SYSACCESS_MAX_DRIVES) {
            strncpy(out->drives[i], token, 4);
            out->drives[i][3] = '\0';

            out->drive_usage_percent[i] = sysaccess_get_disk_usage_percent(token);
            out->drive_type[i] = sysaccess_get_drive_type(token);

            token = strtok(NULL, " ");
            ++i;
        }
    }

    out->ip_address   = sysaccess_get_local_ip();
    out->mac_address  = sysaccess_get_mac_address();
    out->is_connected = sysaccess_is_connected();

    out->uptime   = sysaccess_get_uptime();
    out->is_admin = sysaccess_is_admin();
    out->pid      = sysaccess_get_process_id();
    out->cwd      = sysaccess_get_current_directory();
    out->exe      = sysaccess_get_executable_path();

    return 1;
}

void sysaccess_print_snapshot(const SysSnapshot* s, FILE* out) {
    if (!s || !out) return;

    fprintf(out, "\n+-------------------------------+\n");
    fprintf(out, "|     sysaccess diagnostics     |\n");
    fprintf(out, "+-------------------------------+\n");

    fprintf(out, "User          : %s @ %s (%s)\n", s->username, s->hostname, s->is_admin ? "Administrator" : "Standard User");
    fprintf(out, "Architecture  : %s\n", s->architecture);

    fprintf(out, "CPU           : %s\n", s->cpu_model);
    fprintf(out, "CPU Usage     : %.2f%%\n", s->cpu_usage);
    fprintf(out, "CPU Cores     : %u\n", s->cpu_cores);
    fprintf(out, "CPU Clock     : %u MHz (base)\n", s->cpu_clock_mhz);
    fprintf(out, "CPU Live      : %u MHz (current)\n", s->cpu_clock_live_mhz);
    fprintf(out, "Threads/Core  : %u\n", s->cpu_threads_per_core);
    fprintf(out, "RAM Usage     : %.2f%%\n", s->ram_usage);
    fprintf(out, "Total RAM     : %.2f GB\n", (double)s->ram_total      / (1024 * 1024 * 1024));
    fprintf(out, "Used RAM      : %.2f GB\n", (double)s->ram_used       / (1024 * 1024 * 1024));
    fprintf(out, "Free RAM      : %.2f GB\n", (double)s->ram_available  / (1024 * 1024 * 1024));

    fprintf(out, "Uptime        : %.2f hours\n", (double)s->uptime / (1000 * 60 * 60));
    fprintf(out, "Process ID    : %u\n", s->pid);
    fprintf(out, "Executable    : %s\n", s->exe);
    fprintf(out, "Current Dir   : %s\n", s->cwd);

    for (int i = 0; i < SYSACCESS_MAX_DRIVES && s->drives[i][0]; ++i) {
        fprintf(out, "Drive %s     : %.1f%% used (%s)\n",
            s->drives[i],
            s->drive_usage_percent[i],
            s->drive_type[i] ? s->drive_type[i] : "Unknown");
    }

    fprintf(out, "IP Address    : %s\n", s->ip_address);
    fprintf(out, "MAC Address   : %s\n", s->mac_address);
    fprintf(out, "Network       : %s\n", s->is_connected ? "Connected" : "Disconnected");
}
