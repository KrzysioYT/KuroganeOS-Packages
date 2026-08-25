#include <kurogane/kurogane.h>
#include <stdio.h>
#include <string.h>

static const char* pass(int value) { return value ? "PASS" : "FAIL"; }

int main(void) {
    ku_system_snapshot system;
    ku_network_status network;
    ku_audio_state audio;
    int ok = 1;
    memset(&system, 0, sizeof(system));
    memset(&network, 0, sizeof(network));
    memset(&audio, 0, sizeof(audio));
    system.structure_size = sizeof(system);
    network.structure_size = sizeof(network);
    audio.structure_size = sizeof(audio);

    puts("KuroganeOS // health");
    if (ku_system_get_snapshot(&system) == KU_STATUS_OK) {
        puts("system     PASS");
        printf("cpu        %u%%\n", (unsigned)system.cpu_percent);
        printf("ram        %u%%\n", (unsigned)system.ram_percent);
    } else { puts("system     FAIL"); ok = 0; }

    if (ku_network_get_status(&network) == KU_STATUS_OK) {
        printf("nic        %s\n", pass(network.physical != 0U));
        printf("dhcp       %s\n", pass(network.dhcp != 0U));
        printf("network    %s\n", pass(network.ready != 0U));
        if (!network.ready) ok = 0;
    } else { puts("network    FAIL"); ok = 0; }

    if (ku_audio_get_state(&audio) == KU_STATUS_OK) {
        printf("audio      %s\n", audio.available ? "PASS" : "OFFLINE");
    } else {
        puts("audio      UNKNOWN");
    }
    printf("result     %s\n", ok ? "HEALTHY" : "DEGRADED");
    return ok ? 0 : 2;
}
