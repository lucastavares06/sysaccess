"""
Example: Python usage of sysaccess.dll (updated snapshot version)

Run from the project root or from 'tests_python/'.
"""

import sys
import os
import json
import glob

sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), '..')))

from bindings.python import sysaccess

def fmt_bool(value: int) -> str:
    return "Administrator" if value else "Standard User"

def bytes_to_gb(bytes_val: int) -> float:
    return bytes_val / (1024 ** 3)

def ms_to_hours(ms: int) -> float:
    return ms / (1000 * 60 * 60)

def get_latest_snapshot_json():
    files = glob.glob("logs/snapshot_*.json")
    if not files:
        return None
    latest = max(files, key=os.path.getmtime)
    with open(latest, "r", encoding="utf-8") as f:
        return json.load(f)

lib = sysaccess.lib

print("+-------------------------------+")
print("|     sysaccess diagnostics     |")
print("+-------------------------------+")

print(f"User          : {lib.sysaccess_get_username().decode()} @ {lib.sysaccess_get_hostname().decode()} ({fmt_bool(lib.sysaccess_is_admin())})")
print(f"Architecture  : {lib.sysaccess_get_architecture().decode()}")
print(f"CPU           : {lib.sysaccess_get_cpu_info().decode()}")
print(f"CPU Usage     : {lib.sysaccess_cpu_usage():.2f}%")
print(f"CPU Cores     : {lib.sysaccess_get_cpu_cores()}")
print(f"CPU Clock     : {lib.sysaccess_get_cpu_clock_mhz()} MHz (base)")
print(f"CPU Live      : {lib.sysaccess_get_cpu_clock_live_mhz()} MHz (current)")
print(f"Threads/Core  : {lib.sysaccess_get_cpu_threads_per_core()}")
print(f"RAM Usage     : {lib.sysaccess_ram_usage():.2f}%")
print(f"Total RAM     : {bytes_to_gb(lib.sysaccess_total_ram()):.2f} GB")
print(f"Uptime        : {ms_to_hours(lib.sysaccess_get_uptime()):.2f} hours")
print(f"Process ID    : {lib.sysaccess_get_process_id()}")
print(f"Executable    : {lib.sysaccess_get_executable_path().decode()}")
print(f"Current Dir   : {lib.sysaccess_get_current_directory().decode()}")
print(f"Drives        : {lib.sysaccess_list_logical_drives().decode()}")
print(f"Drive C Type  : {lib.sysaccess_get_drive_type(b'C:\\').decode()}")

ok, total_gb, free_gb = sysaccess.get_disk_usage("C:\\")
if ok:
    used_pct = (1 - free_gb / total_gb) * 100
    print(f"Disk (C:)     : {used_pct:.1f}% used ({total_gb:.2f} GB total)")
else:
    print("Disk (C:)     : Failed to retrieve.")

print(f"IP Address    : {lib.sysaccess_get_local_ip().decode()}")
print(f"MAC Address   : {lib.sysaccess_get_mac_address().decode()}")
print(f"Network       : {'Connected' if lib.sysaccess_is_connected() else 'Disconnected'}")

# Run and parse snapshot
print("\n[+] Creating snapshot JSON...")
if sysaccess.log_snapshot():
    latest_data = get_latest_snapshot_json()
    if latest_data:
        print(f"[✓] Snapshot saved at latest timestamped JSON.")
    else:
        print("[x] Snapshot saved, but couldn't find the JSON file.")
else:
    print("[x] Failed to generate snapshot JSON.")
