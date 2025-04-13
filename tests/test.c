#include <stdio.h>
#include "sysaccess_snapshot.h"

int main(void) {
    SysSnapshot snapshot;

    if (!sysaccess_snapshot(&snapshot)) {
        fprintf(stderr, "Failed to collect system snapshot.\n");
        return 1;
    }

    sysaccess_print_snapshot(&snapshot, stdout);
    return 0;
}
