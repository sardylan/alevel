/*
 * alevel
 * Copyright (C) 2017  Luca Cireddu
 * sardylan@gmail.com
 * http://www.lucacireddu.it
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "soundcard.h"
#include "../ui.h"

#define SOUNDCARD_TAG "soundcard"

void soundcard_ctx_init(soundcard_ctx *ctx) {
    ui_message(UI_INFO, SOUNDCARD_TAG, "Initializing Audio context");

    ctx->in_rate = 8000;
    ctx->in_format = SND_PCM_FORMAT_U8;

    ctx->out_rate = 8000;
    ctx->out_format = SND_PCM_FORMAT_U8;
}

bool soundcard_open(soundcard_ctx *ctx, char *sound_device) {
    int result;

    ui_message(UI_INFO, SOUNDCARD_TAG, "Opening streams");

    result = snd_pcm_open(&(ctx->in_handle), sound_device, SND_PCM_STREAM_CAPTURE, 0);
    if (result < 0) {
        ui_message(UI_ERROR, "Unable to open pcm device \"%s\": %s\n", sound_device, (char *) snd_strerror(result));
        return false;
    }

    result = snd_pcm_open(&(ctx->out_handle), sound_device, SND_PCM_STREAM_PLAYBACK, 0);
    if (result < 0) {
        ui_message(UI_ERROR, "Unable to open pcm device \"%s\": %s\n", sound_device, (char *) snd_strerror(result));
        return false;
    }

    return true;
}

bool soundcard_config(soundcard_ctx *ctx) {
    ui_message(UI_INFO, SOUNDCARD_TAG, "Configuring streams");

    if (!soundcard_config_stream(ctx->in_handle, &(ctx->in_rate)))
        return false;

    if (!soundcard_config_stream(ctx->out_handle, &(ctx->out_rate)))
        return false;

    return true;
}

bool soundcard_config_stream(snd_pcm_t *handle, unsigned int *rate) {
    snd_pcm_hw_params_t *params;
    int result;

    ui_message(UI_INFO, SOUNDCARD_TAG, "Configuring stream");

    snd_pcm_hw_params_malloc(&params);
    snd_pcm_hw_params_any(handle, params);
    snd_pcm_hw_params_set_access(handle, params, SND_PCM_ACCESS_RW_INTERLEAVED);
    snd_pcm_hw_params_set_format(handle, params, SND_PCM_FORMAT_S8);
    snd_pcm_hw_params_set_channels(handle, params, 1);
    snd_pcm_hw_params_set_rate_near(handle, params, rate, NULL);
    result = snd_pcm_hw_params(handle, params);
    snd_pcm_hw_params_free(params);

    if (result < 0) {
        ui_message(UI_ERROR, SOUNDCARD_TAG, "unable to set hw parameters: %s\n", (char *) snd_strerror(result));
        return false;
    }

    return true;
}

void soundcard_close(soundcard_ctx *ctx) {
    ui_message(UI_INFO, SOUNDCARD_TAG, "Closing streams");

    snd_pcm_close(ctx->in_handle);
    snd_pcm_close(ctx->out_handle);
}

void soundcard_deinit(soundcard_ctx *ctx) {
    ui_message(UI_INFO, SOUNDCARD_TAG, "Deinitializing Audio context");
}

/*
    int rc;
    snd_pcm_t *handle;
    snd_pcm_hw_params_t *params;
    unsigned int val, val2;
    int dir;
    snd_pcm_uframes_t frames;

    rc = snd_pcm_open(&handle, conf->sound_device, SND_PCM_STREAM_PLAYBACK, 0);
    if (rc < 0) {
        ui_message(UI_ERROR, "unable to open pcm device: %s\n", (char *) snd_strerror(rc));
        return;
    }

    snd_pcm_hw_params_alloca(&params);
    snd_pcm_hw_params_any(handle, params);
    snd_pcm_hw_params_set_access(handle, params, SND_PCM_ACCESS_RW_INTERLEAVED);
    snd_pcm_hw_params_set_format(handle, params, SND_PCM_FORMAT_S16_LE);
    snd_pcm_hw_params_set_channels(handle, params, 2);

    val = 44100;
    snd_pcm_hw_params_set_rate_near(handle, params, &val, &dir);

    rc = snd_pcm_hw_params(handle, params);
    if (rc < 0) {
        ui_message(UI_ERROR, "unable to set hw parameters: %s\n", (char *) snd_strerror(rc));
        return;
    }

    printf("PCM handle name = '%s'\n", snd_pcm_name(handle));
    printf("PCM state = %s\n", snd_pcm_state_name(snd_pcm_state(handle)));

    snd_pcm_hw_params_get_access(params, (snd_pcm_access_t *) &val);
    printf("access type = %s\n", snd_pcm_access_name((snd_pcm_access_t) val));

    snd_pcm_hw_params_get_format(params, &val);
    printf("format = '%s' (%s)\n", snd_pcm_format_name((snd_pcm_format_t) val),
           snd_pcm_format_description((snd_pcm_format_t) val));

    snd_pcm_hw_params_get_subformat(params, (snd_pcm_subformat_t *) &val);
    printf("subformat = '%s' (%s)\n", snd_pcm_subformat_name((snd_pcm_subformat_t) val),
           snd_pcm_subformat_description((snd_pcm_subformat_t) val));

    snd_pcm_hw_params_get_channels(params, &val);
    printf("channels = %d\n", val);

    snd_pcm_hw_params_get_rate(params, &val, &dir);
    printf("rate = %d bps\n", val);

    snd_pcm_hw_params_get_period_time(params, &val, &dir);
    printf("period time = %d us\n", val);

    snd_pcm_hw_params_get_period_size(params, &frames, &dir);
    printf("period size = %d frames\n", (int) frames);

    snd_pcm_hw_params_get_buffer_time(params, &val, &dir);
    printf("buffer time = %d us\n", val);

    snd_pcm_hw_params_get_buffer_size(params, (snd_pcm_uframes_t *) &val);
    printf("buffer size = %d frames\n", val);

    snd_pcm_hw_params_get_periods(params, &val, &dir);
    printf("periods per buffer = %d frames\n", val);

    snd_pcm_hw_params_get_rate_numden(params, &val, &val2);
    printf("exact rate = %d/%d bps\n", val, val2);

    val = snd_pcm_hw_params_get_sbits(params);
    printf("significant bits = %d\n", val);

    snd_pcm_hw_params_get_tick_time(params, &val, &dir);
    printf("tick time = %d us\n", val);

    val = snd_pcm_hw_params_is_batch(params);
    printf("is batch = %d\n", val);

    val = snd_pcm_hw_params_is_block_transfer(params);
    printf("is block transfer = %d\n", val);

    val = snd_pcm_hw_params_is_double(params);
    printf("is double = %d\n", val);

    val = snd_pcm_hw_params_is_half_duplex(params);
    printf("is half duplex = %d\n", val);

    val = snd_pcm_hw_params_is_joint_duplex(params);
    printf("is joint duplex = %d\n", val);

    val = snd_pcm_hw_params_can_overrange(params);
    printf("can overrange = %d\n", val);

    val = snd_pcm_hw_params_can_mmap_sample_resolution(params);
    printf("can mmap = %d\n", val);

    val = snd_pcm_hw_params_can_pause(params);
    printf("can pause = %d\n", val);

    val = snd_pcm_hw_params_can_resume(params);
    printf("can resume = %d\n", val);

    val = snd_pcm_hw_params_can_sync_start(params);
    printf("can sync start = %d\n", val);

    snd_pcm_close(handle);
 */
