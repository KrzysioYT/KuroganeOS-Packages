#include <kurogane/kurogane.h>
#include <stdio.h>
#include <string.h>

int main(void) {
    ku_audio_state state;
    memset(&state, 0, sizeof(state));
    state.structure_size = sizeof(state);
    if (ku_audio_get_state(&state) != KU_STATUS_OK) {
        puts("kuro-audioinfo: audio status unavailable");
        return 1;
    }
    printf("Audio      %s\n", state.available ? "AVAILABLE" : "OFFLINE");
    printf("Muted      %s\n", state.muted ? "YES" : "NO");
    printf("Volume     %u%%\n", (unsigned)state.volume_percent);
    printf("Format     %u Hz / %u ch / %u bit\n",
           (unsigned)state.sample_rate,
           (unsigned)state.channels,
           (unsigned)state.bits_per_sample);
    return state.available ? 0 : 2;
}
