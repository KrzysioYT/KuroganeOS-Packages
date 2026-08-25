#include <kurogane/kurogane.h>
#include <stdio.h>
#include <string.h>
#include "kuro_util.h"

int main(void) {
    ku_system_snapshot state;
    memset(&state, 0, sizeof(state));
    state.structure_size = sizeof(state);
    if (ku_system_get_snapshot(&state) != KU_STATUS_OK) {
        puts("kuro-uptime: system snapshot unavailable");
        return 1;
    }
    printf("uptime  ");
    ku_pkg_print_duration(ku_system_uptime_seconds(&state));
    putchar('\n');
    printf("load    cpu=%u%% gpu=%u%% disk=%u%%\n",
           (unsigned)state.cpu_percent,
           (unsigned)state.gpu_percent,
           (unsigned)state.disk_percent);
    return 0;
}
