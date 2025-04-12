#ifndef SYSACCESS_DISK_H
#define SYSACCESS_DISK_H

#ifdef __cplusplus
extern "C" {
#endif

    /// Gets the total and free space of a drive (e.g., "C:\\").
    /// Returns 1 on success, 0 on failure.
    int sysaccess_get_disk_usage(const char* drive, unsigned long long* total, unsigned long long* free);

    /// Lists all logical drives as a space-separated string (e.g., "C:\\ D:\\").
    /// The result is stored internally and must not be freed.
    /// Returns NULL on failure.
    const char* sysaccess_list_logical_drives(void);

    /// Returns the type of the given drive as a string (e.g. "Fixed", "Removable").
    /// The returned string is stored internally and must not be freed.
    const char* sysaccess_get_drive_type(const char* drive);

#ifdef __cplusplus
}
#endif

#endif
