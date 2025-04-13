#ifndef SYSACCESS_SNAPSHOT_JSON_H
#define SYSACCESS_SNAPSHOT_JSON_H

#include "sysaccess_snapshot.h"

#ifdef __cplusplus
extern "C" {
#endif

    /// Exports the given SysSnapshot to a JSON file at the specified path.
    /// Returns 1 on success, 0 on failure.
    int sysaccess_snapshot_to_json(const SysSnapshot* snapshot, const char* filepath);

    /// Captures a SysSnapshot and writes it as JSON to the logs/ folder with a timestamped filename.
    /// Example: logs/snapshot_2025-04-13_185223.json
    /// Returns 1 on success, 0 on failure.
    int sysaccess_log_snapshot(void);

#ifdef __cplusplus
}
#endif

#endif
