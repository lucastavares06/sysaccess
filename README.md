# sysaccess

System diagnostics toolkit written in C.  
Exports a DLL for use in C, Python, and other environments.

---

## Features

- CPU info, usage, and core count
- RAM usage and total memory
- Username, hostname, and admin check
- Process ID, uptime, current dir and executable path
- Disk usage and drive type
- Prints to terminal or logs to file

---

## Build

```bash
mkdir build && cd build
cmake ..
mingw32-make
