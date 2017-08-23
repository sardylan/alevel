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

#include <stdio.h>
#include <stdbool.h>
#include <signal.h>
#include <malloc.h>
#include <ncurses.h>

#include "alevel.h"
#include "cfg.h"
#include "ui.h"
#include "utility/audio.h"
#include "soundcard/soundcard.h"

#define ALEVEL_TAG ""

#define ACQUIRE_BUFFER_SIZE 512
#define AUDIO_LEVEL_SIZE 64

char *program_name;
cfg *conf;

static volatile bool keep_running = true;

int main(int argc, char **argv) {
    program_name = argv[0];

    signal(SIGINT, signal_handler);

    cfg_init();

    if (cfg_parse(argc, argv)) {
        cfg_print();
        main_program();
    }

    cfg_free();

    return 0;
}

void signal_handler(int signal) {
    if (signal == SIGINT) {
        keep_running = false;
    }
}

void main_program() {
    soundcard_ctx *ctx;
    uint8_t buffer[ACQUIRE_BUFFER_SIZE];
    uint8_t rms;
    int result;
    snd_pcm_sframes_t frames;
    char message[1024];
    char level[AUDIO_LEVEL_SIZE + 1];
    int value;
    int i;

    memset(level, '\0', sizeof(level));

    ui_message(UI_INFO, ALEVEL_TAG, "Configuring sound card");

    ctx = (soundcard_ctx *) malloc(sizeof(soundcard_ctx));
    soundcard_ctx_init(ctx);

    if (!soundcard_open(ctx, conf->sound_device)) {
        soundcard_deinit(ctx);
        free(ctx);
        return;
    }

    if (!soundcard_config(ctx)) {
        soundcard_deinit(ctx);
        free(ctx);
        return;
    }

    ui_message(UI_INFO, ALEVEL_TAG, "Starting sound acquire");

    if ((result = snd_pcm_prepare(ctx->in_handle)) < 0) {
        ui_message(UI_ERROR, ALEVEL_TAG, "Error preparing capture handler");
        soundcard_deinit(ctx);
        free(ctx);
        return;
    }

    while (keep_running) {
        frames = snd_pcm_readi(ctx->in_handle, buffer, ACQUIRE_BUFFER_SIZE);
        if (frames != ACQUIRE_BUFFER_SIZE) {
            ui_message(UI_ERROR, ALEVEL_TAG, "Read from audio interface failed (%s)", snd_strerror(result));
            soundcard_close(ctx);
            soundcard_deinit(ctx);
            free(ctx);
            return;
        }

        rms = audio_compute_rms(buffer, frames);
        value = rms / (256 / AUDIO_LEVEL_SIZE);

        for (i = 0; i < AUDIO_LEVEL_SIZE; i++)
            if (i < value)
                level[i] = '#';
            else
                level[i] = '-';

        memset(message, '\0', sizeof(message));
        sprintf(message, "Read %d audio frames - RMS: %03d/256 - Level: %s", (int) frames, (int) rms, level);

        printf("%s\n", message);
    }

    ui_message(UI_INFO, ALEVEL_TAG, "Closing sound card");
    soundcard_close(ctx);
    soundcard_deinit(ctx);

    free(ctx);
}
