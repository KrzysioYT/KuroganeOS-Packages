#include <kurogane/kurogane.h>
#include <stdio.h>
#include <string.h>
#include "kuro_util.h"

int main(void) {
    ku_system_snapshot state;
    uint64_t used;
    memset(&state, 0, sizeof(state));
    state.structure_size = sizeof(state);
    if (ku_system_get_snapshot(&state) != KU_STATUS_OK) {
        puts("kuro-meminfo: system snapshot unavailable");
        return 1;
    }
    used = state.memory_total_bytes >= state.memory_free_bytes
        ? state.memory_total_bytes - state.memory_free_bytes : 0U;
    printf("Memory pressure  %u%%\n", (unsigned)state.ram_percent);
    printf("Total            "); ku_pkg_print_bytes(state.memory_total_bytes); putchar('\n');
    printf("Used             "); ku_pkg_print_bytes(used); putchar('\n');
    printf("Free             "); ku_pkg_print_bytes(state.memory_free_bytes); putchar('\n');
    return 0;
}
