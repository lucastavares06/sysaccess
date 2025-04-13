"""
Example: Python usage of sysaccess.dll

Run from the project root or from 'tests_python/' to test DLL bindings.
"""

import sys
import os
sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), '..')))

from bindings.python import sysaccess

def fmt_bool(value: int) -> str:
    return "Administrator" if value else "Standard User"

def bytes_to_gb(bytes_val: int) -> float:
    return bytes_val / (1024 ** 3)

def ms_to_hours(ms: int) -> float:
    return ms / (1000 * 60 * 60)

lib = sysaccess.lib

print("+-------------------------------+")
print("|     sysaccess diagnostics     |")
print("+-------------------------------+")

print(f"CPU           : {lib.sysaccess_get_cpu_info().decode()}")
print(f"CPU Usage     : {lib.sysaccess_cpu_usage():.2f}%")
print(f"CPU Cores     : {lib.sysaccess_get_cpu_cores()}")
print(f"CPU Clock     : {lib.sysaccess_get_cpu_clock_mhz()} MHz (base)")
print(f"CPU Live      : {lib.sysaccess_get_cpu_clock_live_mhz()} MHz (current)")
print(f"Threads/Core  : {lib.sysaccess_get_cpu_threads_per_core()}")
print(f"Architecture  : {lib.sysaccess_get_architecture().decode()}")
print(f"RAM Usage     : {lib.sysaccess_ram_usage():.2f}%")
print(f"Total RAM     : {bytes_to_gb(lib.sysaccess_total_ram()):.2f} GB")
print(f"User          : {lib.sysaccess_get_username().decode()}")
print(f"Host          : {lib.sysaccess_get_hostname().decode()}")
print(f"Privilege     : {fmt_bool(lib.sysaccess_is_admin())}")
print(f"Uptime        : {ms_to_hours(lib.sysaccess_get_uptime()):.2f} hours")
print(f"Process ID    : {lib.sysaccess_get_process_id()}")
print(f"Executable    : {lib.sysaccess_get_executable_path().decode()}")
print(f"Current Dir   : {lib.sysaccess_get_current_directory().decode()}")
print(f"Drives        : {lib.sysaccess_list_logical_drives().decode()}")
print(f"Drive C Type  : {lib.sysaccess_get_drive_type(b'C:\\').decode()}")

ok, total_gb, free_gb = sysaccess.get_disk_usage("C:\\")
if ok:
    print(f"Disk (C:)     : {free_gb:.2f} GB free / {total_gb:.2f} GB total")
else:
    print("Disk (C:)     : Failed to retrieve.")

log_path = os.path.join(os.path.dirname(__file__), "sysaccess.log")
if sysaccess.log_to_file(log_path):
    print(f"\nLog saved to {log_path}")
else:
    print("\nFailed to save log")
