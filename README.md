# Sysaccess

`Sysaccess` is a C library for accessing system information on Windows. It exposes a simple API to retrieve data about CPU, RAM, disk, network, processes, and user environment, as well as snapshotting complete system state in JSON format.

**Note**: This library is currently Windows-only. It does not support Linux or macOS.

## Installation

```bash
git clone https://github.com/lucastavares06/sysaccess
```

To use the library, include the headers in your project and link against `libsysaccess.dll`. The C-style API is compatible with any language that supports dynamic linking.

---

## Headers and Functions

### CPU — `sysaccess_cpu.h`

- `const char* sysaccess_get_cpu_info(void);` — Returns CPU model string.
- `float sysaccess_cpu_usage(void);` — Returns current CPU usage percentage.
- `unsigned int sysaccess_get_cpu_cores(void);` — Logical core count.
- `const char* sysaccess_get_architecture(void);` — Architecture string.
- `unsigned int sysaccess_get_cpu_clock_mhz(void);` — Base clock speed (MHz).
- `unsigned int sysaccess_get_cpu_clock_live_mhz(void);` — Live average clock (MHz).
- `unsigned int sysaccess_get_cpu_threads_per_core(void);` — Threads per core.

### RAM — `sysaccess_ram.h`

- `float sysaccess_ram_usage(void);` — RAM usage percentage.
- `unsigned long long sysaccess_total_ram(void);` — Total RAM in bytes.
- `unsigned long long sysaccess_ram_used_bytes(void);` — Used RAM.
- `unsigned long long sysaccess_ram_available_bytes(void);` — Available RAM.

### Disk — `sysaccess_disk.h`

- `int sysaccess_get_disk_usage(const char* drive, ...)` — Gets total/free disk space.
- `float sysaccess_get_disk_usage_percent(const char* drive);` — Disk usage percentage.
- `const char* sysaccess_list_logical_drives(void);` — Lists logical drives.
- `const char* sysaccess_get_drive_type(const char* drive);` — Drive type string.

### Network — `sysaccess_net.h`

- `const char* sysaccess_get_local_ip(void);` — Returns local IPv4 address.
- `const char* sysaccess_get_mac_address(void);` — Returns MAC address of first adapter.
- `int sysaccess_is_connected(void);` — Returns 1 if online, 0 otherwise.

### System — `sysaccess_system.h`

- `unsigned long long sysaccess_get_uptime(void);` — Uptime in ms.
- `int sysaccess_is_admin(void);` — Whether the process has admin rights.
- `unsigned int sysaccess_get_process_id(void);` — Current PID.
- `const char* sysaccess_get_current_directory(void);` — Current working dir.
- `const char* sysaccess_get_executable_path(void);` — Full path to executable.

### User — `sysaccess_user.h`

- `const char* sysaccess_get_username(void);` — Current user name.
- `const char* sysaccess_get_hostname(void);` — Host machine name.

### Snapshot — `sysaccess_snapshot.h`

- `int sysaccess_snapshot(SysSnapshot* out);` — Fills a SysSnapshot struct.
- `void sysaccess_print_snapshot(const SysSnapshot* snapshot, FILE* out);` — Pretty-print to a file.

### Snapshot JSON — `sysaccess_snapshot_json.h`

- `int sysaccess_snapshot_to_json(const SysSnapshot* snapshot, const char* filepath);` — Save snapshot to JSON file.
- `int sysaccess_log_snapshot(void);` — Save snapshot in `logs/` folder with timestamp.

---

## Python Bindings

This repository includes a `bindings/python/` folder with:

- `sysaccess.py` — `ctypes` wrapper for the DLL.
- `test.py` — Full diagnostic snapshot and usage example.

### Example usage

```python
from bindings.python import sysaccess

lib = sysaccess.lib

print("User         :", lib.sysaccess_get_username().decode())
print("CPU Usage    :", lib.sysaccess_cpu_usage())
print("RAM Usage    :", lib.sysaccess_ram_usage())

ok, total, free = sysaccess.get_disk_usage("C:\\")
if ok:
    print("Disk C: Usage:", 100 * (1 - free / total), "%")

if sysaccess.log_snapshot():
    print("Snapshot saved.")
else:
    print("Snapshot failed.")
```

Place the DLL in a directory accessible to Python. No installation is needed—just clone and use.

---

## License

This project is licensed under the [MIT License](https://github.com/lucastavares06/sysaccess/blob/main/LICENSE).

---

Thanks for checking out Sysaccess! If you have questions or suggestions, feel free to open an issue. 🚀
