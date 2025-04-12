#include "sysaccess_system.h"

int main(void) {
    sysaccess_print_system_info();

    if (sysaccess_log_to_file("diagnostic.log")) {
        printf("\nLog saved to diagnostic.log\n");
    } else {
        printf("\nFailed to save log\n");
    }

    return 0;
}
