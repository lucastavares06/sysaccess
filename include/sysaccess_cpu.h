#ifndef SYSACCESS_CPU_H
#define SYSACCESS_CPU_H

#ifdef __cplusplus
extern "C" {
#endif

    /// Returns a string containing the CPU model name.
    /// Example: "Intel(R) Core(TM) i7-9750H CPU @ 2.60GHz"
    /// The returned string is stored internally and must not be modified or freed by the caller.
    const char* sysaccess_get_cpu_info(void);

    /// Returns the current CPU usage as a percentage (0.0 to 100.0).
    /// Returns -1.0 on failure.
    float sysaccess_cpu_usage(void);

    /// Returns the number of logical CPU cores.
    unsigned int sysaccess_get_cpu_cores(void);

    /// Returns a string representing the system architecture (e.g., "x64", "x86", "ARM", "Unknown").
    /// The returned string is stored internally and must not be freed.
    const char* sysaccess_get_architecture(void);

    /// Returns the base CPU clock speed in MHz as reported by the system registry.
    /// This value is static and does not reflect real-time changes.
    /// Returns 0 on failure.
    unsigned int sysaccess_get_cpu_clock_mhz(void);

    /// Returns the current dynamic CPU frequency in MHz (averaged across all cores).
    /// Returns 0 on failure.
    unsigned int sysaccess_get_cpu_clock_live_mhz(void);

    /// Returns the number of threads per physical core (e.g., 2 for hyper-threading).
    /// Returns 0 on failure.
    unsigned int sysaccess_get_cpu_threads_per_core(void);
#ifdef __cplusplus
}
#endif

#endif
