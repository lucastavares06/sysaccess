#ifndef SYSACCESS_SNAPSHOT_H
#define SYSACCESS_SNAPSHOT_H

#ifdef __cplusplus
extern "C" {
#endif

#define SYSACCESS_MAX_CORES 64
#define SYSACCESS_MAX_DRIVES 8

    typedef struct {
        const char* username;
        const char* hostname;
        const char* architecture;

        const char* cpu_model;
        float cpu_usage;
        float cpu_per_core[SYSACCESS_MAX_CORES];
        unsigned int cpu_cores;
        unsigned int cpu_clock_mhz;
        unsigned int cpu_clock_live_mhz;
        unsigned int cpu_threads_per_core;

        float ram_usage;
        unsigned long long ram_total;
        unsigned long long ram_used;
        unsigned long long ram_available;

        char drives[SYSACCESS_MAX_DRIVES][4];
        float drive_usage_percent[SYSACCESS_MAX_DRIVES];
        const char* drive_type[SYSACCESS_MAX_DRIVES];


        const char* ip_address;
        const char* mac_address;
        int is_connected;

        unsigned long long uptime;
        int is_admin;
        unsigned int pid;
        const char* cwd;
        const char* exe;
    } SysSnapshot;

    /// Collects a full system snapshot.
    /// Returns 1 on success, 0 on failure.
    int sysaccess_snapshot(SysSnapshot* out);

    /// Prints a formatted summary of the snapshot to the given stream.
    void sysaccess_print_snapshot(const SysSnapshot* snapshot, FILE* out);

#ifdef __cplusplus
}
#endif

#endif
