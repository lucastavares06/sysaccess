from typing import Any
from bindings.python import sysaccess
import os

lib: Any = sysaccess.lib

print("\n+-------------------------------+")
print("|     sysaccess diagnostics     |")
print("+-------------------------------+")

print(f"CPU           : {lib.sysaccess_get_cpu_info().decode()}")
print(f"CPU Usage     : {lib.sysaccess_cpu_usage():.2f}%")
print(f"CPU Cores     : {lib.sysaccess_get_cpu_cores()}")
print(f"User          : {lib.sysaccess_get_username().decode()}")
print(f"Host          : {lib.sysaccess_get_hostname().decode()}")
print(f"Privilege     : {'Administrator' if lib.sysaccess_is_admin() else 'Standard User'}")
print(f"Uptime        : {lib.sysaccess_get_uptime() / (1000 * 60 * 60):.2f} hours")
print(f"Process ID    : {lib.sysaccess_get_process_id()}")
print(f"Executable    : {lib.sysaccess_get_executable_path().decode()}")
print(f"Current Dir   : {lib.sysaccess_get_current_directory().decode()}")
print(f"Drives        : {lib.sysaccess_list_logical_drives().decode()}")
print(f"Drive C Type  : {lib.sysaccess_get_drive_type(b'C:\\').decode()}")

log_path = os.path.join(os.path.dirname(__file__), "sysaccess.log")
if sysaccess.log_to_file(log_path):
    print(f"\nLog saved to {log_path}")
else:
    print("\nFailed to save log")
