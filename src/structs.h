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

#ifndef _STRUCTS_H_
#define _STRUCTS_H_

typedef enum
{
    STATIC,
    BUTTON,
} ui_type_t;

typedef enum
{
    SINGLE_PRESS,
    DOUBLE_PRESS,
    LONG_PRESS,
} ui_event_t;

typedef enum
{
    IDLE,
    LOADING,
    CONTROLS,
    PAIR,
    INFO,
    SENSORS,
    RESET,
    SLEEP,
    DEEP_SLEEP,
} ui_state_t;

typedef struct
{
    int id;
    ui_event_t type;
} ui_touch_event_t;

typedef struct
{
    int x;
    int y;
} ui_pos_t;

typedef struct
{
    int w;
    int h;
} ui_size_t;

typedef struct
{
    ui_pos_t min_pos;
    ui_pos_t max_pos;
} ui_rect_t;

typedef struct
{
    int id;
    ui_type_t type;
} ui_info_t;

#endif
