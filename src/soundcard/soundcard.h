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

#ifndef __ALEVEL_SOUNDCARD_H
#define __ALEVEL_SOUNDCARD_H

#include <alsa/asoundlib.h>
#include <stdbool.h>

struct soundcard_ctx_t {
    snd_pcm_t *in_handle;
    snd_pcm_format_t in_format;
    unsigned int in_rate;

    snd_pcm_t *out_handle;
    snd_pcm_format_t out_format;
    unsigned int out_rate;
};

typedef struct soundcard_ctx_t soundcard_ctx;

void soundcard_ctx_init(soundcard_ctx *);

bool soundcard_open(soundcard_ctx *, char *);

bool soundcard_config(soundcard_ctx *ctx);

bool soundcard_config_stream(snd_pcm_t *, unsigned int *);

void soundcard_close(soundcard_ctx *);

void soundcard_deinit(soundcard_ctx *);

#endif