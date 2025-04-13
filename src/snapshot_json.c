#include <stdio.h>
#include <time.h>
#include <direct.h>
#include "sysaccess_snapshot.h"

static void write_json_string_indent(FILE* f, const char* key, const char* value, const int with_comma, const int indent_level) {
    for (int i = 0; i < indent_level; ++i) fputs("    ", f);
    fprintf(f, "\"%s\": \"", key);

    if (value) {
        for (const char* p = value; *p; ++p) {
            switch (*p) {
                case '\\': fputs("\\\\", f); break;
                case '\"': fputs("\\\"", f); break;
                case '\n': fputs("\\n", f); break;
                case '\r': fputs("\\r", f); break;
                case '\t': fputs("\\t", f); break;
                default: fputc(*p, f); break;
            }
        }
    }

    fprintf(f, "\"%s\n", with_comma ? "," : "");
}

static void write_json_number(FILE* f, const char* key, const double value, const int with_comma) {
    fprintf(f, "    \"%s\": %.2f%s\n", key, value, with_comma ? "," : "");
}

static void write_json_uint64(FILE* f, const char* key, const unsigned long long value, const int with_comma) {
    fprintf(f, "    \"%s\": %llu%s\n", key, value, with_comma ? "," : "");
}

int sysaccess_snapshot_to_json(const SysSnapshot* s, const char* filepath) {
    if (!s || !filepath) return 0;

    FILE* f = fopen(filepath, "w");
    if (!f) return 0;

    fprintf(f, "{\n");

    const int field_count = 22;
    int i = 0;

    write_json_string_indent(f, "username",        s->username,        ++i < field_count, 1);
    write_json_string_indent(f, "hostname",        s->hostname,        ++i < field_count, 1);
    write_json_string_indent(f, "architecture",    s->architecture,    ++i < field_count, 1);
    write_json_string_indent(f, "cpu_model",       s->cpu_model,       ++i < field_count, 1);
    write_json_number(f, "cpu_usage",             s->cpu_usage,       ++i < field_count);
    write_json_number(f, "ram_usage",             s->ram_usage,       ++i < field_count);
    write_json_uint64(f, "ram_total",             s->ram_total,       ++i < field_count);
    write_json_uint64(f, "ram_used",              s->ram_used,        ++i < field_count);
    write_json_uint64(f, "ram_available",         s->ram_available,   ++i < field_count);
    write_json_string_indent(f, "ip_address",      s->ip_address,      ++i < field_count, 1);
    write_json_string_indent(f, "mac_address",     s->mac_address,     ++i < field_count, 1);
    write_json_number(f, "cpu_clock_mhz",         s->cpu_clock_mhz,   ++i < field_count);
    write_json_number(f, "cpu_clock_live_mhz",    s->cpu_clock_live_mhz, ++i < field_count);
    write_json_uint64(f, "uptime_ms",             s->uptime,          ++i < field_count);
    write_json_string_indent(f, "cwd",             s->cwd,             ++i < field_count, 1);
    write_json_string_indent(f, "exe",             s->exe,             ++i < field_count, 1);
    write_json_number(f, "cpu_cores",             s->cpu_cores,       ++i < field_count);
    write_json_number(f, "threads_per_core",      s->cpu_threads_per_core, ++i < field_count);
    write_json_number(f, "pid",                   s->pid,             ++i < field_count);
    write_json_string_indent(f, "admin",           s->is_admin ? "true" : "false", ++i < field_count, 1);
    write_json_string_indent(f, "network",         s->is_connected ? "Connected" : "Disconnected", ++i < field_count, 1);

    fprintf(f, "    \"drives\": [\n");
    for (int j = 0; j < 8 && s->drives[j][0]; ++j) {
        fprintf(f, "        {\n");
        write_json_string_indent(f, "letter", s->drives[j], 1, 3);
        fprintf(f, "            \"usage\": %.2f,\n", s->drive_usage_percent[j]);
        fprintf(f, "            \"type\": \"%s\"\n", s->drive_type[j]);
        fprintf(f, "        }%s\n", s->drives[j + 1][0] ? "," : "");
    }
    fprintf(f, "    ]\n");

    fprintf(f, "}\n");
    fclose(f);
    return 1;
}

int sysaccess_log_snapshot(void) {
    SysSnapshot s;
    if (!sysaccess_snapshot(&s)) return 0;

    _mkdir("logs");

    const time_t t = time(NULL);
    const struct tm* tm_info = localtime(&t);
    char filename[128];
    strftime(filename, sizeof(filename), "logs/snapshot_%Y-%m-%d_%H%M%S.json", tm_info);

    return sysaccess_snapshot_to_json(&s, filename);
}
