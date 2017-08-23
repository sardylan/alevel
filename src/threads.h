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

#ifndef __ALEVEL_THREADS_H
#define __ALEVEL_THREADS_H

#include <stdbool.h>

#include "soundcard/soundcard.h"

struct thread_acquire_data_t {
    soundcard_ctx *ctx;
};

typedef struct thread_acquire_data_t thread_acquire_data;

bool thread_acquire_start(pthread_t *, thread_acquire_data *);

void *thread_acquire(void *);

#endif
