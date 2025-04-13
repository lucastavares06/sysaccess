#ifndef SYSACCESS_NET_H
#define SYSACCESS_NET_H

#ifdef __cplusplus
extern "C" {
#endif

    /// Returns the local IPv4 address as a string (e.g., "192.168.1.100").
    /// The returned string is stored internally and must not be freed.
    const char* sysaccess_get_local_ip(void);

    /// Returns the MAC address of the first active adapter as a string (e.g., "00:1A:2B:3C:4D:5E").
    /// The returned string is stored internally and must not be freed.
    const char* sysaccess_get_mac_address(void);

    /// Returns 1 if the system is connected to the internet, 0 otherwise.
    int sysaccess_is_connected(void);

#ifdef __cplusplus
}
#endif

#endif
