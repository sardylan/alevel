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
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <stdbool.h>

#include "ui.h"
#include "cfg.h"
#include "config.h"

#define CFG_TAG "cfg"

extern cfg *conf;

void cfg_init() {
    size_t ln;

    conf = (cfg *) malloc(sizeof(cfg));

    conf->debug_level = DEFAULT_UI_DEBUG_LEVEL;

    ln = strlen(DEFAULT_SOUND_DEVICE) + 1;
    conf->sound_device = (char *) calloc(sizeof(char), ln);
    strcpy(conf->sound_device, DEFAULT_SOUND_DEVICE);
}

void cfg_print() {
    ui_message(UI_INFO, CFG_TAG, "debug-level = %d", conf->debug_level);
    ui_message(UI_INFO, CFG_TAG, "sound-device = %s", conf->sound_device);
}

void cfg_free() {
    free(conf->sound_device);

    free(conf);
}

bool cfg_parse(int argc, char **argv) {
    bool ret;
    int option_index;
    int c;
    size_t ln;

    option_index = 0;
    ret = 1;

    static struct option long_options[] = {
            {"help",         no_argument,       0, 'h'},
            {"version",      no_argument,       0, 'V'},
            {"quiet",        no_argument,       0, 'q'},
            {"verbose",      no_argument,       0, 'v'},
            {"debug-level",  required_argument, 0, 'd'},
            {"sound-device", required_argument, 0, 'a'},
            {0, 0,                              0, 0}
    };

    while (true) {
        c = getopt_long(argc, argv, "hVqf:vd:l:k:m:sch:p:", long_options, &option_index);

        if (c == -1) {
            break;
        }

        if (c == '?' || c == 'h') {
            ui_help();
            ret = false;
            break;
        }

        if (c == 'V') {
            ui_version();
            ret = 0;
            break;
        }

        if (c == 'q') {
            conf->debug_level = 0;
            continue;
        }

        if (c == 'v') {
            conf->debug_level = 4;
            continue;
        }

        if (c == 'd') {
            conf->debug_level = atoi(optarg);
            continue;
        }

        if (c == 'a') {
            ln = strlen(optarg) + 1;
            conf->sound_device = (char *) realloc((void *) conf->sound_device, sizeof(char) * ln);
            strcpy(conf->sound_device, optarg);
            continue;
        }

        ret = 0;
        ui_message(UI_ERROR, "Unknown parameter %s", argv[optind]);
    }

    return ret;
}
