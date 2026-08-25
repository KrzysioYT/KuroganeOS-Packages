#include <kurogane/kurogane.h>
#include <stdio.h>
#include <string.h>

static int16_t g_samples[KU_AUDIO_PCM_MAX_FRAMES * KU_AUDIO_PCM_CHANNELS];

static void fill_tone(unsigned phase) {
    size_t frame;
    for (frame = 0U; frame < KU_AUDIO_PCM_MAX_FRAMES; ++frame) {
        const unsigned position = (phase + (unsigned)frame) % 109U;
        const int16_t value = position < 54U ? (int16_t)5000 : (int16_t)-5000;
        g_samples[frame * 2U] = value;
        g_samples[frame * 2U + 1U] = value;
    }
}

int main(void) {
    ku_audio_state state;
    unsigned chunk;
    unsigned phase = 0U;
    memset(&state, 0, sizeof(state));
    state.structure_size = sizeof(state);
    if (ku_audio_get_state(&state) != KU_STATUS_OK || !state.available) {
        puts("kuro-audiotest: audio device unavailable");
        return 1;
    }
    printf("audio %u Hz / %u ch / %u bit / volume %u%%\n",
           (unsigned)state.sample_rate,
           (unsigned)state.channels,
           (unsigned)state.bits_per_sample,
           (unsigned)state.volume_percent);
    puts("playing short test tone...");

    for (chunk = 0U; chunk < 8U; ++chunk) {
        ku_status_t status;
        fill_tone(phase);
        phase = (phase + (unsigned)KU_AUDIO_PCM_MAX_FRAMES) % 109U;
        status = ku_audio_play_pcm16_stereo(g_samples, KU_AUDIO_PCM_MAX_FRAMES);
        if (status != KU_STATUS_OK) {
            printf("playback submit failed status=%d\n", (int)status);
            return 2;
        }
        do {
            status = ku_audio_poll();
            if (status == KU_STATUS_WOULD_BLOCK) (void)kuro_yield();
        } while (status == KU_STATUS_WOULD_BLOCK);
        if (status != KU_STATUS_OK) {
            printf("playback failed status=%d\n", (int)status);
            return 3;
        }
    }
    puts("audio playback PASS");
    return 0;
}
