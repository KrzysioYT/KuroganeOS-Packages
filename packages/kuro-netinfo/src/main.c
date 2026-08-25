#include <kurogane/kurogane.h>
#include <stdio.h>
#include <string.h>
#include "kuro_util.h"

int main(void) {
    ku_network_status state;
    memset(&state, 0, sizeof(state));
    state.structure_size = sizeof(state);
    if (ku_network_get_status(&state) != KU_STATUS_OK) {
        puts("kuro-netinfo: network status unavailable");
        return 1;
    }
    puts("KuroganeOS // network");
    printf("Physical  %s\n", state.physical ? "YES" : "NO");
    printf("DHCP      %s\n", state.dhcp ? "BOUND" : "NO LEASE");
    printf("Ready     %s\n", state.ready ? "ONLINE" : "OFFLINE");
    printf("Address   "); ku_pkg_print_ipv4(state.address); putchar('\n');
    printf("Gateway   "); ku_pkg_print_ipv4(state.gateway); putchar('\n');
    printf("DNS       "); ku_pkg_print_ipv4(state.dns); putchar('\n');
    printf("RX        "); ku_pkg_print_bytes(state.bytes_received); putchar('\n');
    printf("TX        "); ku_pkg_print_bytes(state.bytes_transmitted); putchar('\n');
    return state.ready ? 0 : 2;
}
