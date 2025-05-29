#include "pulse.h"
#include "error.h"
#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pulse/error.h>

snd_pcm_t *init_pulse(int rate, int n_channels)
{
    static const pa_sample_spec ss = {
        .format = PA_SAMPLE_S16LE,
        .rate = 0,
        .channels = 0
    };

    snd_pcm_t *pcm_handle = malloc(sizeof(snd_pcm_t));
    if (!pcm_handle)
        error_exit("Memory allocation failed for snd_pcm_t\n");

    pa_sample_spec sample_spec = ss;
    sample_spec.rate = rate;
    sample_spec.channels = n_channels;

    int error;
    pcm_handle->s = pa_simple_new(NULL,
                                  "listener",
                                  PA_STREAM_RECORD,
                                  NULL,
                                  "record",
                                  &sample_spec,
                                  NULL,
                                  NULL,
                                  &error);
    if (!pcm_handle->s) {
        error_exit("pa_simple_new() failed: %s\n", pa_strerror(error));
    }

    pcm_handle->rate = rate;
    pcm_handle->channels = n_channels;

    return pcm_handle;
}

void get_audio_1s(snd_pcm_t *pcm_handle, short *buffer, int rate, int channels)
{
    int samples = rate * channels;
    int error;
    int index = 0;

    while (samples > 0) {
        int cur = samples;

        int ret = pa_simple_read(pcm_handle->s, &buffer[index], cur * sizeof(short), &error);
        if (ret < 0) {
            fprintf(stderr, "pa_simple_read() failed: %s\n", pa_strerror(error));
            error_exit("Stream error, exiting.\n");
        }

        index += cur;
        samples -= cur;
    }
}
