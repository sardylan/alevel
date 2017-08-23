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
#include <string.h>

#include "utils.h"

void truncate_string(char *input, unsigned int max_length) {
    size_t ln;
    int i;

    ln = strlen(input);

    if (ln > 3 && ln > max_length + 4) {
        i = 0;

        while (*input != '\n' && *input != '\r' && i < max_length) {
            input++;
            i++;
        }

        *(input + 0) = '.';
        *(input + 1) = '.';
        *(input + 2) = '.';
        *(input + 3) = '\0';
    }
}
