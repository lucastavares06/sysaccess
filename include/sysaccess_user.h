#ifndef SYSACCESS_USER_H
#define SYSACCESS_USER_H

#ifdef __cplusplus
extern "C" {
#endif

    /// Returns the current user's name (e.g., "Lucas Tavares").
    /// The returned string is stored internally and must not be freed.
    const char* sysaccess_get_username(void);

    /// Returns the computer name (hostname).
    /// The returned string is stored internally and must not be freed.
    const char* sysaccess_get_hostname(void);

#ifdef __cplusplus
}
#endif

#endif
