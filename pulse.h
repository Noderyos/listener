#define BUFFER_SIZE 8192

#include <pulse/simple.h>

typedef struct {
    pa_simple *s;
    int rate;
    int channels;
} snd_pcm_t;

snd_pcm_t *init_pulse(int rate, int n_channels);
void get_audio_1s(snd_pcm_t *pcm_handle, short *buffer, int rate, int channels);
