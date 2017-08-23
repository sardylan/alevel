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

#include <pthread.h>

#include "threads.h"
#include "cfg.h"
#include "ui.h"
#include "utility/audio.h"

#define THREADS_TAG "threads"
#define THREADS_TAG_ACQUIRE "threads-acquire"

#define THREAD_ACQUIRE_BUFFER_SIZE 512

extern cfg *conf;

bool thread_acquire_start(pthread_t *thread, thread_acquire_data *data) {
    int result;
    pthread_attr_t thread_attrs;

    ui_message(UI_INFO, THREADS_TAG, "Starting acquire thread");

    pthread_attr_init(&thread_attrs);
    result = pthread_create(thread, NULL, thread_acquire, data);
    pthread_attr_destroy(&thread_attrs);

    if (result != 0) {
        fprintf(stderr, "Error creating thread\n");
        return false;
    }

    return true;
}

void *thread_acquire(void *raw_data) {
    thread_acquire_data *data;
    uint8_t buffer[THREAD_ACQUIRE_BUFFER_SIZE];
    uint8_t rms;
    int result;
    snd_pcm_sframes_t frames;

    ui_message(UI_INFO, THREADS_TAG_ACQUIRE, "Acquire thread start");

    data = (thread_acquire_data *) raw_data;

    if ((result = snd_pcm_prepare(data->ctx->in_handle)) < 0) {
        ui_message(UI_ERROR, THREADS_TAG_ACQUIRE, "Error preparing capture handler");
        return NULL;
    }

    while (true) {
        frames = snd_pcm_readi(data->ctx->in_handle, buffer, THREAD_ACQUIRE_BUFFER_SIZE);
        if (frames != THREAD_ACQUIRE_BUFFER_SIZE) {
            ui_message(UI_ERROR, THREADS_TAG_ACQUIRE, "Read from audio interface failed (%s)", snd_strerror(result));
            break;
        }

        rms = audio_compute_rms(buffer, frames);
        ui_message(UI_DEBUG, THREADS_TAG_ACQUIRE, "Read %d audio frames - RMS: %d", frames, rms);


    }

    ui_message(UI_INFO, THREADS_TAG_ACQUIRE, "Acquire thread end");

    return NULL;
}
