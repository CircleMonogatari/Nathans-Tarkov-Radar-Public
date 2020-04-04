/*
Looking Glass - KVM FrameRelay (KVMFR) Client
Copyright (C) 2017-2019 Geoffrey McRae <geoff@hostfission.com>
https://looking-glass.hostfission.com

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
this program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place, Suite 330, Boston, MA 02111-1307 USA
*/

#pragma once

#include <stdbool.h>

#include "interface/font.h"

typedef struct EGL_ESPObject EGL_ESPObject;

bool egl_espobject_init(EGL_ESPObject ** alert, const LG_Font * font, LG_FontObj fontObj);
void egl_espobject_free(EGL_ESPObject ** alert);

void egl_espobject_set_color(EGL_ESPObject * alert, const uint32_t color);
void egl_espobject_set_text (EGL_ESPObject * alert, const char * str);
void egl_espobject_set_screenpos (EGL_ESPObject * alert, float x, float y, float distance);
void egl_espobject_render   (EGL_ESPObject * alert, const float scaleX, const float scaleY);