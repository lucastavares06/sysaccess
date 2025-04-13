#include <windows.h>
#include <stdio.h>
#include <intrin.h>
#include <string.h>
#include <pdh.h>
#include "sysaccess_cpu.h"

#define CPUID_EXT_BASE   ((int)0x80000000)
#define CPUID_EXT_FN2    ((int)0x80000002)
#define CPUID_EXT_FN3    ((int)0x80000003)
#define CPUID_EXT_FN4    ((int)0x80000004)

// ────────────────────────────────
// Locale-aware counter support
// ────────────────────────────────

typedef struct {
    LANGID lang;
    const wchar_t* counter_mhz;
    const wchar_t* counter_percent;
} CpuCounterLocale;

static const CpuCounterLocale COUNTER_LOCALES[] = {
    // pt-BR
    { 0x0416, L"\\Informações do processador(_Total)\\Frequência do processador",
              L"\\Processador(_Total)\\% Desempenho do processador" },

    // en-US
    { 0x0409, L"\\Processor Information(_Total)\\Processor Frequency",
              L"\\Processor(_Total)\\% Processor Performance" }
};

#define COUNTER_LOCALE_COUNT (sizeof(COUNTER_LOCALES) / sizeof(COUNTER_LOCALES[0]))

static const CpuCounterLocale* find_locale_by_lang(const LANGID lang) {
    for (size_t i = 0; i < COUNTER_LOCALE_COUNT; ++i) {
        if (COUNTER_LOCALES[i].lang == lang)
            return &COUNTER_LOCALES[i];
    }
    return NULL;
}

// ────────────────────────────────
// Internal helpers
// ────────────────────────────────

static ULONGLONG filetime_to_uint64(const FILETIME ft) {
    return (ULONGLONG)ft.dwHighDateTime << 32 | ft.dwLowDateTime;
}

static unsigned int query_counter_mhz(const wchar_t* counter_path) {
    PDH_HQUERY hQuery = NULL;
    PDH_HCOUNTER hCounter;
    PDH_FMT_COUNTERVALUE counterVal;

    if (PdhOpenQueryW(NULL, 0, &hQuery) != ERROR_SUCCESS) return 0;
    if (PdhAddCounterW(hQuery, counter_path, 0, &hCounter) != ERROR_SUCCESS) {
        PdhCloseQuery(hQuery);
        return 0;
    }

    if (PdhCollectQueryData(hQuery) != ERROR_SUCCESS) {
        PdhCloseQuery(hQuery);
        return 0;
    }

    Sleep(100);

    if (PdhCollectQueryData(hQuery) != ERROR_SUCCESS) {
        PdhCloseQuery(hQuery);
        return 0;
    }

    if (PdhGetFormattedCounterValue(hCounter, PDH_FMT_LONG, NULL, &counterVal) != ERROR_SUCCESS) {
        PdhCloseQuery(hQuery);
        return 0;
    }

    PdhCloseQuery(hQuery);
    return counterVal.longValue;
}

// ────────────────────────────────
// Public API implementations
// ────────────────────────────────

const char* sysaccess_get_cpu_info(void) {
    static char cpu_info_buffer[64];
    int cpu_info[4] = {0};

    __cpuid(cpu_info, CPUID_EXT_BASE);
    const unsigned int max_id = cpu_info[0];

    if (max_id >= CPUID_EXT_FN4) {
        char brand[49] = {0};

        __cpuid(cpu_info, CPUID_EXT_FN2); memcpy(brand,       cpu_info, sizeof(cpu_info));
        __cpuid(cpu_info, CPUID_EXT_FN3); memcpy(brand + 16,  cpu_info, sizeof(cpu_info));
        __cpuid(cpu_info, CPUID_EXT_FN4); memcpy(brand + 32,  cpu_info, sizeof(cpu_info));

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

    const ULONGLONG idle_diff   = filetime_to_uint64(idle2) - filetime_to_uint64(idle1);
    const ULONGLONG kernel_diff = filetime_to_uint64(kernel2) - filetime_to_uint64(kernel1);
    const ULONGLONG user_diff   = filetime_to_uint64(user2) - filetime_to_uint64(user1);
    const ULONGLONG total       = kernel_diff + user_diff;

    return total == 0 ? -1.0f : 100.0f * (1.0f - (float)idle_diff / (float)total);
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
        case PROCESSOR_ARCHITECTURE_AMD64: strcpy(arch_str, "x64");   break;
        case PROCESSOR_ARCHITECTURE_INTEL: strcpy(arch_str, "x86");   break;
        case PROCESSOR_ARCHITECTURE_ARM:   strcpy(arch_str, "ARM");   break;
        case PROCESSOR_ARCHITECTURE_ARM64: strcpy(arch_str, "ARM64"); break;
        default: strcpy(arch_str, "Unknown"); break;
    }

    return arch_str;
}

unsigned int sysaccess_get_cpu_clock_mhz(void) {
    HKEY hKey;
    DWORD mhz = 0;
    DWORD size = sizeof(DWORD);

    if (RegOpenKeyExA(
            HKEY_LOCAL_MACHINE,
            "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0",
            0,
            KEY_READ,
            &hKey
        ) != ERROR_SUCCESS)
    {
        return 0;
    }

    if (RegQueryValueExA(
            hKey,
            "~MHz",
            NULL,
            NULL,
            (LPBYTE)&mhz,
            &size
        ) != ERROR_SUCCESS)
    {
        RegCloseKey(hKey);
        return 0;
    }

    RegCloseKey(hKey);
    return mhz;
}

unsigned int sysaccess_get_cpu_clock_live_mhz(void) {
    const LANGID lang = GetUserDefaultUILanguage();
    const CpuCounterLocale* locale = find_locale_by_lang(lang);

    if (!locale)
        return 0;

    const unsigned int mhz = query_counter_mhz(locale->counter_mhz);
    if (mhz > 0)
        return mhz;

    const unsigned int perc = query_counter_mhz(locale->counter_percent);
    if (perc > 0) {
        const unsigned int base = sysaccess_get_cpu_clock_mhz();
        if (base > 0)
            return perc * base / 100;
    }

    return 0;
}

unsigned int sysaccess_get_cpu_threads_per_core(void) {
    DWORD length = 0;
    GetLogicalProcessorInformation(NULL, &length);

    SYSTEM_LOGICAL_PROCESSOR_INFORMATION* buffer = malloc(length);
    if (!buffer) return 0;

    if (!GetLogicalProcessorInformation(buffer, &length)) {
        free(buffer);
        return 0;
    }

    const DWORD count = length / sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION);
    DWORD physical_cores = 0;
    DWORD logical_cores = 0;

    for (DWORD i = 0; i < count; ++i) {
        if (buffer[i].Relationship == RelationProcessorCore) {
            physical_cores++;
            KAFFINITY mask = buffer[i].ProcessorMask;
            while (mask) {
                logical_cores += mask & 1;
                mask >>= 1;
            }
        }
    }

    free(buffer);

    if (physical_cores == 0) return 0;
    return logical_cores / physical_cores;
}
