#include <windows.h>
#include <intrin.h>
#include <string.h>
#include "sysaccess_cpu.h"

#define CPUID_EXT_BASE   ((int)0x80000000)
#define CPUID_EXT_FN2    ((int)0x80000002)
#define CPUID_EXT_FN3    ((int)0x80000003)
#define CPUID_EXT_FN4    ((int)0x80000004)

static ULONGLONG filetime_to_uint64(const FILETIME ft) {
    return (ULONGLONG)ft.dwHighDateTime << 32 | ft.dwLowDateTime;
}

static char cpu_info_buffer[64];

const char* sysaccess_get_cpu_info(void) {
    int cpu_info[4] = {0};

    __cpuid(cpu_info, CPUID_EXT_BASE);
    const unsigned int max_id = cpu_info[0];

    if (max_id >= CPUID_EXT_FN4) {
        char brand[49] = {0};

        __cpuid(cpu_info, CPUID_EXT_FN2);
        memcpy(brand, cpu_info, sizeof(cpu_info));

        __cpuid(cpu_info, CPUID_EXT_FN3);
        memcpy(brand + 16, cpu_info, sizeof(cpu_info));

        __cpuid(cpu_info, CPUID_EXT_FN4);
        memcpy(brand + 32, cpu_info, sizeof(cpu_info));

        brand[48] = '\0';

        strncpy(cpu_info_buffer, brand, sizeof(cpu_info_buffer) - 1);
        cpu_info_buffer[sizeof(cpu_info_buffer) - 1] = '\0';
        return cpu_info_buffer;
    }

    return "Unknown CPU";
}

float sysaccess_cpu_usage(void) {
    FILETIME idle1, kernel1, user1;
    FILETIME idle2, kernel2, user2;

    GetSystemTimes(&idle1, &kernel1, &user1);
    Sleep(100);

    GetSystemTimes(&idle2, &kernel2, &user2);

    const ULONGLONG idle_diff = filetime_to_uint64(idle2) - filetime_to_uint64(idle1);
    const ULONGLONG kernel_diff = filetime_to_uint64(kernel2) - filetime_to_uint64(kernel1);
    const ULONGLONG user_diff = filetime_to_uint64(user2) - filetime_to_uint64(user1);

    const ULONGLONG total = kernel_diff + user_diff;

    if (total == 0) return -1.0f;

    return 100.0f * (1.0f - (float)idle_diff / (float)total);
}

unsigned int sysaccess_get_cpu_cores(void) {
    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);
    return sysInfo.dwNumberOfProcessors;
}

const char* sysaccess_get_architecture(void) {
    static char arch_str[16] = "Unknown";
    SYSTEM_INFO sysInfo;
    GetNativeSystemInfo(&sysInfo);

    switch (sysInfo.wProcessorArchitecture) {
        case PROCESSOR_ARCHITECTURE_AMD64:
            strcpy(arch_str, "x64");
        break;
        case PROCESSOR_ARCHITECTURE_INTEL:
            strcpy(arch_str, "x86");
        break;
        case PROCESSOR_ARCHITECTURE_ARM:
            strcpy(arch_str, "ARM");
        break;
        case PROCESSOR_ARCHITECTURE_ARM64:
            strcpy(arch_str, "ARM64");
        break;
        default:
            strcpy(arch_str, "Unknown");
        break;
    }

    return arch_str;
}

unsigned int sysaccess_get_cpu_clock_mhz(void) {
    HKEY hKey;
    DWORD mhz = 0;
    DWORD size = sizeof(DWORD);

    LONG result = RegOpenKeyExA(
        HKEY_LOCAL_MACHINE,
        "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0",
        0,
        KEY_READ,
        &hKey
    );

    if (result != ERROR_SUCCESS)
        return 0;

    result = RegQueryValueExA(
        hKey,
        "~MHz",
        NULL,
        NULL,
        (LPBYTE)&mhz,
        &size
    );

    RegCloseKey(hKey);

    if (result != ERROR_SUCCESS)
        return 0;

    return mhz;
}
