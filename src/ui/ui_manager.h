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

#ifndef _UI_MANAGER_H_
#define _UI_MANAGER_H_

#include <M5EPD.h>
#include <Arduino.h>
#include <FS.h>
#include <vector>

#include "../structs.h"
#include "../hap/hap_manager.h"
#include "../system/system_utils.h"

#include "ui_object.h"
#include "ui_label.h"

class ui_manager
{
public:
    ui_manager(system_utils *SYS, hap_manager *HAP);

    void initialize();

    void switch_state(ui_state_t state);

    ui_touch_event_t get_touch_event();

    int get_buttons_count();

    bool draw_notification(String text);

private:
    system_utils *SYS_;
    hap_manager *HAP_;

    std::vector<ui_object> buttons_;
    std::vector<ui_object> objects_;
    std::vector<ui_label> labels_;

    bool is_touch_event_started = false;

    time_t touch_event_start_time = 0;

    int touched_button_id = -1;

    int get_touched_button_id(int x, int y);

    void create_button(int x, int y, int w, int h, const uint8_t *default_state_image_data, const uint8_t *active_state_image_data);
    void create_object(int x, int y, int w, int h, const uint8_t *image_data);
    void create_label(int x, int y, uint8_t size, uint16_t color, uint16_t cache_size, String title);

    void press_button(int id);
    void release_button(int id);

    void draw_state_loading(m5epd_update_mode_t mode);
    void draw_state_sleep(m5epd_update_mode_t mode);
    void draw_state_deep_sleep(m5epd_update_mode_t mode);
    void draw_state_reset(m5epd_update_mode_t mode);
    void draw_state_pair(m5epd_update_mode_t mode);
    void draw_state_controls(m5epd_update_mode_t mode);
    void draw_state_info(m5epd_update_mode_t mode);
    void draw_state_sensors(m5epd_update_mode_t mode);

    void draw_title(String title);
    void draw_text(String text, int x, int y, bool alignRight, uint8_t size, uint16_t color);
};

#endif
