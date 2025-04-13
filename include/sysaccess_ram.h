#ifndef SYSACCESS_RAM_H
#define SYSACCESS_RAM_H

#ifdef __cplusplus
extern "C" {
#endif

    /// Returns the current RAM usage in percentage (0.0 to 100.0).
    /// Returns -1.0 on failure.
    float sysaccess_ram_usage(void);

    /// Returns the total physical memory in bytes.
    unsigned long long sysaccess_total_ram(void);

    /// Returns the amount of physical memory currently in use (in bytes).
    /// Returns 0 on failure.
    unsigned long long sysaccess_ram_used_bytes(void);

    /// Returns the amount of physical memory currently available (in bytes).
    /// Returns 0 on failure.
    unsigned long long sysaccess_ram_available_bytes(void);

#ifdef __cplusplus
}
#endif

#endif
