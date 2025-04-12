#ifndef SYSACCESS_SYSTEM_H
#define SYSACCESS_SYSTEM_H

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

    /// Returns the system uptime in milliseconds.
    /// The value represents the time since the system started.
    unsigned long long sysaccess_get_uptime(void);

    /// Returns 1 if the current process is running as administrator, 0 otherwise.
    int sysaccess_is_admin(void);

    /// Returns the process ID (PID) of the current running process.
    unsigned int sysaccess_get_process_id(void);

    /// Returns the current working directory as a string.
    /// The returned string is stored internally and must not be freed.
    const char* sysaccess_get_current_directory(void);

    /// Returns the full path to the running executable.
    /// The returned string is stored internally and must not be freed.
    const char* sysaccess_get_executable_path(void);

    /// Prints system diagnostics to the specified FILE* (e.g. stdout or a log file).
    void sysaccess_print_system_info_to(FILE* out);

    /// Logs system diagnostics to a file at the specified path.
    /// Returns 1 on success, 0 on failure.
    int sysaccess_log_to_file(const char* filepath);

    /// Prints system diagnostics to the terminal (stdout).
    void sysaccess_print_system_info(void);

#ifdef __cplusplus
}
#endif

#endif
