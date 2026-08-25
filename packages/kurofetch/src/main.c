#include <kurogane/kurogane.h>
#include <stdio.h>
#include <string.h>
#include "kuro_util.h"

int main(void) {
    ku_system_snapshot system;
    ku_network_status network;
    memset(&system, 0, sizeof(system));
    memset(&network, 0, sizeof(network));
    system.structure_size = sizeof(system);
    network.structure_size = sizeof(network);

    puts("KuroganeOS // kurofetch");
    puts("--------------------");
    printf("PID       "); ku_pkg_print_u64(ku_process_id()); putchar('\n');

    if (ku_system_get_snapshot(&system) == KU_STATUS_OK) {
        printf("CPU       %u%%\n", (unsigned)system.cpu_percent);
        printf("GPU       %u%%\n", (unsigned)system.gpu_percent);
        printf("RAM       %u%%  ", (unsigned)system.ram_percent);
        ku_pkg_print_bytes(system.memory_total_bytes - system.memory_free_bytes);
        printf(" / "); ku_pkg_print_bytes(system.memory_total_bytes); putchar('\n');
        printf("Disk      %u%%\n", (unsigned)system.disk_percent);
        printf("Uptime    "); ku_pkg_print_duration(ku_system_uptime_seconds(&system)); putchar('\n');
    } else {
        puts("System    snapshot unavailable");
    }

    if (ku_network_get_status(&network) == KU_STATUS_OK) {
        printf("Network   %s / DHCP %s\n", network.ready ? "ONLINE" : "OFFLINE", network.dhcp ? "YES" : "NO");
        printf("IPv4      "); ku_pkg_print_ipv4(network.address); putchar('\n');
    } else {
        puts("Network   status unavailable");
    }
    return 0;
}
