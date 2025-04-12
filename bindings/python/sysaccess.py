import ctypes
import os
import sys
from typing import Optional

def _find_dll():
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

# Load the DLL
DLL_PATH = _find_dll()
try:
    lib = ctypes.CDLL(DLL_PATH)
except OSError as e:
    print(f"[sysaccess] Failed to load DLL: {e}", file=sys.stderr)
    sys.exit(1)

# --- String functions ---
lib.sysaccess_get_cpu_info.restype = ctypes.c_char_p
lib.sysaccess_get_username.restype = ctypes.c_char_p
lib.sysaccess_get_hostname.restype = ctypes.c_char_p
lib.sysaccess_get_current_directory.restype = ctypes.c_char_p
lib.sysaccess_get_executable_path.restype = ctypes.c_char_p
lib.sysaccess_list_logical_drives.restype = ctypes.c_char_p
lib.sysaccess_get_drive_type.argtypes = [ctypes.c_char_p]
lib.sysaccess_get_drive_type.restype = ctypes.c_char_p

# --- Numeric functions ---
lib.sysaccess_cpu_usage.restype = ctypes.c_float
lib.sysaccess_get_cpu_cores.restype = ctypes.c_uint
lib.sysaccess_get_uptime.restype = ctypes.c_ulonglong
lib.sysaccess_get_process_id.restype = ctypes.c_uint
lib.sysaccess_is_admin.restype = ctypes.c_int

# --- Logging ---
lib.sysaccess_log_to_file.argtypes = [ctypes.c_char_p]
lib.sysaccess_log_to_file.restype = ctypes.c_int

def log_to_file(path: str) -> bool:
    """Save system diagnostics to the given file path."""
    return bool(lib.sysaccess_log_to_file(path.encode()))
