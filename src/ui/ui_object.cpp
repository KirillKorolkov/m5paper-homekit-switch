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

#include "ui_object.h"

ui_object::ui_object(int id, int x, int y, int w, int h, const uint8_t *default_state_image_data)
{
    pos_.x = x;
    pos_.y = y;

    rect_.min_pos.x = x;
    rect_.min_pos.y = y;
    rect_.max_pos.x = x + w;
    rect_.max_pos.y = y + h;

    size_.w = w;
    size_.h = h;

    default_state_image_data_ = default_state_image_data;
    active_state_image_data_ = nullptr;

    info_.id = id;
    info_.type = STATIC;
}

ui_object::ui_object(int id, int x, int y, int w, int h, const uint8_t *default_state_image_data, const uint8_t *active_state_image_data)
{

    id_ = id;

    pos_.x = x;
    pos_.y = y;

    rect_.min_pos.x = x;
    rect_.min_pos.y = y;
    rect_.max_pos.x = x + w;
    rect_.max_pos.y = y + h;

    size_.w = w;
    size_.h = h;

    default_state_image_data_ = default_state_image_data;
    active_state_image_data_ = active_state_image_data;
}

int ui_object::get_id()
{
    return id_;
}

ui_pos_t ui_object::get_pos()
{
    return pos_;
}

ui_rect_t ui_object::get_rect()
{
    return rect_;
}

ui_size_t ui_object::get_size()
{
    return size_;
}

const uint8_t *ui_object::get_default_state_image_data()
{
    return default_state_image_data_;
}

const uint8_t *ui_object::get_active_state_image_data()
{
    return active_state_image_data_;
}
