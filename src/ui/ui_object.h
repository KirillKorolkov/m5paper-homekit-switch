/*
 * M5Paper HomeKit Switch
 * Copyright (c) 2018 Kirill Korolkov
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

#ifndef _UI_OBJECT_H_
#define _UI_OBJECT_H_

#include <M5EPD.h>
#include <Arduino.h>
#include <FS.h>
#include "../structs.h"

class ui_object
{
public:
    ui_object(int id, int x, int y, int w, int h, const uint8_t *default_state_image_data);
    ui_object(int id, int x, int y, int w, int h, const uint8_t *default_state_image_data, const uint8_t *active_state_image_data);

    int get_id();

    ui_pos_t get_pos();
    ui_rect_t get_rect();
    ui_size_t get_size();

    const uint8_t *get_default_state_image_data();
    const uint8_t *get_active_state_image_data();

private:
    int id_;

    ui_pos_t pos_;
    ui_rect_t rect_;
    ui_size_t size_;
    ui_info_t info_;

    const uint8_t *default_state_image_data_;
    const uint8_t *active_state_image_data_;
};

#endif
