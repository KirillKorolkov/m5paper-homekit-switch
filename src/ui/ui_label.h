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

#ifndef _UI_LABEL_H_
#define _UI_LABEL_H_

#include <M5EPD.h>
#include <Arduino.h>
#include <FS.h>

#include "../structs.h"

class ui_label
{

public:
    ui_label(int x, int y, uint8_t size, uint16_t color, uint16_t cache_size, String text);

    ui_pos_t get_pos();
    uint8_t get_size();
    uint16_t get_color();
    uint16_t get_cache_size();
    String get_text();

private:
    ui_pos_t pos_;
    uint8_t size_;
    uint16_t color_;
    uint16_t cache_size_;
    String text_;
};

#endif
