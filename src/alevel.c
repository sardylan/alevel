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
#include <unistd.h>
#include <malloc.h>
#include <pthread.h>

#include "alevel.h"
#include "cfg.h"
#include "ui.h"
#include "threads.h"
#include "soundcard/soundcard.h"

#define ALEVEL_TAG ""

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
    pthread_t thread_acquire;
    thread_acquire_data thread_data_acquire;

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

    ui_message(UI_INFO, ALEVEL_TAG, "Starting threads");

    thread_data_acquire.ctx = ctx;
    thread_acquire_start(&thread_acquire, &thread_data_acquire);

    ui_message(UI_INFO, ALEVEL_TAG, "Joining threads");
    pthread_join(thread_acquire, NULL);

    ui_message(UI_INFO, ALEVEL_TAG, "Closing sound card");
    soundcard_close(ctx);
    soundcard_deinit(ctx);

    free(ctx);
}
