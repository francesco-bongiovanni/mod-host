/*
 * This file is part of mod-host.
 *
 * mod-host is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * mod-host is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with mod-host.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef MOD_LINK_H_INCLUDED
#define MOD_LINK_H_INCLUDED

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#else
#include <stdbool.h>
#endif

typedef struct LINK_T link_t;

typedef struct LINK_TIME_INFO_T {
    double bpm, beats, phase;
} link_time_info_t;

link_t* link_create(double bpm, uint32_t buffer_size, uint32_t sample_rate);
void link_enable(link_t* link, bool on, double bpm);
void link_process(link_t* link, uint32_t frames, link_time_info_t* info);
void link_set_tempo(link_t* link, double tempo);
void link_cleanup(link_t* link);

#ifdef __cplusplus
}
#endif

#endif // MOD_LINK_H_INCLUDED
