import ctypes
import os
import sys

def _find_dll() -> str:
    possible_paths = [
        "../../cmake-build-debug/libsysaccess.dll",
        "../../build/libsysaccess.dll",
        "../../libsysaccess.dll",
        os.path.join(os.getcwd(), "libsysaccess.dll"),
    ]
    for rel_path in possible_paths:
        abs_path = os.path.abspath(os.path.join(os.path.dirname(__file__), rel_path))
        if os.path.exists(abs_path):
            return abs_path
    raise FileNotFoundError("libsysaccess.dll not found in any known location.")

DLL_PATH = _find_dll()
try:
    lib = ctypes.CDLL(DLL_PATH)
except OSError as e:
    print(f"[sysaccess] Failed to load DLL: {e}", file=sys.stderr)
    sys.exit(1)

# ─────────────────────────────
# String-returning functions
# ─────────────────────────────
for fn in [
    "sysaccess_get_cpu_info",
    "sysaccess_get_username",
    "sysaccess_get_hostname",
    "sysaccess_get_current_directory",
    "sysaccess_get_executable_path",
    "sysaccess_list_logical_drives",
    "sysaccess_get_architecture",
    "sysaccess_get_local_ip",
    "sysaccess_get_mac_address",
]:
    getattr(lib, fn).restype = ctypes.c_char_p

lib.sysaccess_get_drive_type.argtypes = [ctypes.c_char_p]
lib.sysaccess_get_drive_type.restype = ctypes.c_char_p

# ─────────────────────────────
# Numeric-returning functions
# ─────────────────────────────
lib.sysaccess_cpu_usage.restype = ctypes.c_float
lib.sysaccess_ram_usage.restype = ctypes.c_float
lib.sysaccess_total_ram.restype = ctypes.c_ulonglong
lib.sysaccess_get_cpu_cores.restype = ctypes.c_uint
lib.sysaccess_get_cpu_clock_mhz.restype = ctypes.c_uint
lib.sysaccess_get_cpu_clock_live_mhz.restype = ctypes.c_uint
lib.sysaccess_get_uptime.restype = ctypes.c_ulonglong
lib.sysaccess_get_process_id.restype = ctypes.c_uint
lib.sysaccess_is_admin.restype = ctypes.c_int
lib.sysaccess_get_cpu_threads_per_core.restype = ctypes.c_uint
lib.sysaccess_is_connected.restype = ctypes.c_int

# ─────────────────────────────
# Disk usage helper
# ─────────────────────────────
lib.sysaccess_get_disk_usage.argtypes = [
    ctypes.c_char_p,
    ctypes.POINTER(ctypes.c_ulonglong),
    ctypes.POINTER(ctypes.c_ulonglong),
]
lib.sysaccess_get_disk_usage.restype = ctypes.c_int

def get_disk_usage(drive: str) -> tuple[int, float, float]:
    total = ctypes.c_ulonglong()
    free = ctypes.c_ulonglong()
    ok = lib.sysaccess_get_disk_usage(drive.encode(), ctypes.byref(total), ctypes.byref(free))
    return (ok, total.value / (1024**3), free.value / (1024**3)) if ok else (0, 0.0, 0.0)

# ─────────────────────────────
# Logging (auto + snapshot json)
# ─────────────────────────────
lib.sysaccess_log_snapshot.restype = ctypes.c_int

def log_snapshot() -> bool:
    return bool(lib.sysaccess_log_snapshot())
