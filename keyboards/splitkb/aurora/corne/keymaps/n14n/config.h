/*
Copyright 2022 n14n
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#ifdef OLED_ENABLE
#   define OLED_FONT_H "keyboards/splitkb/aurora/corne/keymaps/n14n/glcdfont.c"
#   define OLED_FONT_WIDTH 4
#   define OLED_FONT_HEIGHT 8
#   define OLED_FONT_END 255
#endif
#define RGBLIGHT_SPLIT
#define SPLIT_TRANSACTION_IDS_USER USER_SYNC_MENU
