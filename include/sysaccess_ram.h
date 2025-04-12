#ifndef SYSACCESS_RAM_H
#define SYSACCESS_RAM_H

#ifdef __cplusplus
extern "C" {
#endif

    /// Returns the current RAM usage in percentage (0.0 to 100.0).
    /// Returns -1.0 on error.
    float sysaccess_ram_usage(void);

    /// Returns the total physical memory in bytes.
    unsigned long long sysaccess_total_ram(void);

#ifdef __cplusplus
}
#endif

#endif
