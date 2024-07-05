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

#include "ui_manager.h"

#include "../info_ui.h"

#include "../resources/binary_font.h"
#include "../resources/binary_image_home_kit_qr_code.h"
#include "../resources/binary_image_thermometer.h"
#include "../resources/binary_image_thermometer_humidity.h"
#include "../resources/binary_image_button_top.h"
#include "../resources/binary_image_button_middle.h"
#include "../resources/binary_image_button_bottom.h"

const int screen_width = 540;
const int screen_height = 960;

M5EPD_Canvas canvas(&M5.EPD);
M5EPD_Canvas extra_canvas(&M5.EPD);

ui_manager::ui_manager(system_utils *SYS, hap_manager *HAP)
{
    SYS_ = SYS;
    HAP_ = HAP;
}

void ui_manager::initialize()
{
    M5.TP.SetRotation(90);

    M5.EPD.SetRotation(90);
    M5.EPD.Clear(true);

    canvas.createCanvas(screen_width, screen_height);
    canvas.loadFont(binary_font, sizeof(binary_font));
    canvas.setTextDatum(TC_DATUM);

    extra_canvas.createCanvas(screen_width, 48);
    extra_canvas.loadFont(binary_font, sizeof(binary_font));
    extra_canvas.setTextDatum(TC_DATUM);

    // switch 1
    create_button(52, 92, 192, 160, binary_image_button_top, binary_image_button_top_active);
    create_object(52, 252, 192, 60, binary_image_button_middle);
    create_button(52, 312, 192, 160, binary_image_button_bottom, binary_image_button_bottom_active);
    create_label(145, 272, 22, 15, 256, switch_1_label);

    // switch 2
    create_button(296, 92, 192, 160, binary_image_button_top, binary_image_button_top_active);
    create_object(296, 252, 192, 60, binary_image_button_middle);
    create_button(296, 312, 192, 160, binary_image_button_bottom, binary_image_button_bottom_active);
    create_label(395, 272, 22, 15, 256, switch_2_label);

    // switch 3
    create_button(52, 92 + 420, 192, 160, binary_image_button_top, binary_image_button_top_active);
    create_object(52, 252 + 420, 192, 60, binary_image_button_middle);
    create_button(52, 312 + 420, 192, 160, binary_image_button_bottom, binary_image_button_bottom_active);
    create_label(145, 692, 22, 15, 256, switch_3_label);

    // switch 4
    create_button(296, 92 + 420, 192, 160, binary_image_button_top, binary_image_button_top_active);
    create_object(296, 252 + 420, 192, 60, binary_image_button_middle);
    create_button(296, 312 + 420, 192, 160, binary_image_button_bottom, binary_image_button_bottom_active);
    create_label(395, 692, 22, 15, 256, switch_4_label);
}

void ui_manager::switch_state(ui_state_t state)
{

    canvas.clear();

    switch (state)
    {
    case CONTROLS:
        Serial.println("State switch to CONTROLS\n");
        draw_state_controls(UPDATE_MODE_GC16);
        break;

    case PAIR:
        Serial.println("State switch to PAIR\n");
        draw_state_pair(UPDATE_MODE_GC16);
        break;

    case SENSORS:
        Serial.println("State switch to SENSORS\n");
        draw_state_sensors(UPDATE_MODE_GC16);
        break;

    case INFO:
        Serial.println("State switch to INFO\n");
        draw_state_info(UPDATE_MODE_GC16);
        break;

    case SLEEP:
        Serial.println("State switch to SLEEP\n");
        draw_state_sleep(UPDATE_MODE_GC16);
        break;

    case DEEP_SLEEP:
        Serial.println("State switch to DEEP_SLEEP\n");
        draw_state_deep_sleep(UPDATE_MODE_GC16);
        break;

    case RESET:
        Serial.println("State switch to RESET\n");
        draw_state_reset(UPDATE_MODE_GC16);
        break;

    default:
        Serial.println("State switch to LOADING\n");
        draw_state_loading(UPDATE_MODE_GC16);
    }

    // take_screenshot(canvas, "/" + String(state) + ".bmp", true, 0, 0, -1, -1);
}

void ui_manager::create_button(int x, int y, int w, int h, const uint8_t *default_state_image_data, const uint8_t *active_state_image_data)
{
    int id = buttons_.size();

    ui_object o(id, x, y, w, h, default_state_image_data, active_state_image_data);

    buttons_.push_back(o);
}

void ui_manager::create_object(int x, int y, int w, int h, const uint8_t *image_data)
{
    int id = -1;

    ui_object o(id, x, y, w, h, image_data);

    objects_.push_back(o);
}

void ui_manager::create_label(int x, int y, uint8_t size, uint16_t color, uint16_t cache_size, String title)
{
    ui_label l(x, y, size, color, cache_size, title);

    labels_.push_back(l);
}

void ui_manager::draw_state_loading(m5epd_update_mode_t mode)
{
    canvas.createRender(32, 256);

    canvas.setTextSize(32);
    canvas.setTextColor(15);

    canvas.drawCentreString("starting up...", 270, 440, 2);

    canvas.pushCanvas(0, 0, mode);
}

void ui_manager::draw_state_controls(m5epd_update_mode_t mode)
{
    draw_title("Controls");

    for (auto &button : buttons_)
    {
        auto pos = button.get_pos();
        auto size = button.get_size();
        auto image = button.get_default_state_image_data();

        canvas.pushImage(pos.x, pos.y, size.w, size.h, image);
    }

    for (auto &object : objects_)
    {
        auto pos = object.get_pos();
        auto size = object.get_size();
        auto image = object.get_default_state_image_data();

        canvas.pushImage(pos.x, pos.y, size.w, size.h, image);
    }

    for (auto &label : labels_)
    {
        auto size = label.get_size();
        auto cache = label.get_cache_size();
        auto color = label.get_color();
        auto pos = label.get_pos();
        auto title = label.get_text();

        !canvas.isRenderExist(size) && canvas.createRender(size, cache);

        canvas.setTextSize(size);
        canvas.setTextColor(color);

        canvas.drawString(title, pos.x, pos.y);
    }

    canvas.pushCanvas(0, 0, mode);
}

void ui_manager::draw_state_pair(m5epd_update_mode_t mode)
{
    canvas.pushImage(120, 280, 300, 405, binary_image_home_kit_qr_code);

    canvas.pushCanvas(0, 0, mode);
}

void ui_manager::draw_state_info(m5epd_update_mode_t mode)
{
    draw_title("Info");

    // TODO: up time
    // --------------------------------------------------

    // font size 32 + padding 18
    draw_text("Network", 24, 106, false, 32, 15);

    // font size 24 + padding 18
    draw_text("Status", 24, 148, false, 24, 15);
    draw_text(SYS_->get_wifi_connected_status() ? "Connected" : "Disconnected", 24, 138, true, 24, 15);

    // font size 24 + padding 12
    draw_text("Address", 24, 184, false, 24, 15);
    draw_text(SYS_->get_wifi_address(), 24, 174, true, 24, 15);

    // --------------------------------------------------

    // font size 32 + padding 18
    draw_text("HomeKit", 24, 234, false, 32, 15);

    // font size 24 + padding 18
    draw_text("Status", 24, 276, false, 24, 15);
    draw_text(HAP_->get_homekit_paired_status() ? "Paired" : SYS_->get_wifi_connected_status() ? "Not paired"
                                                                                               : "Unavailable",
              24, 266, true, 24, 15);

    // --------------------------------------------------

    // font size 32 + padding 18
    draw_text("Battery", 24, 326, false, 32, 15);

    // font size 24 + padding 18
    draw_text("Status", 24, 368, false, 24, 15);
    draw_text(String(SYS_->get_battery_level()) + "%", 24, 358, true, 24, 15);

    // font size 24 + padding 12
    draw_text("Voltage", 24, 404, false, 24, 15);
    draw_text(String(M5.getBatteryVoltage()), 24, 394, true, 24, 15);

    canvas.pushCanvas(0, 0, mode);
}

void ui_manager::draw_state_sensors(m5epd_update_mode_t mode)
{
    char temperature[10];
    char humidity[10];

    dtostrf(M5.SHT30.GetTemperature(), 2, 0, temperature);
    dtostrf(M5.SHT30.GetRelHumidity(), 2, 0, humidity);

    draw_title("Sensors");

    canvas.pushImage(160, 192, 200, 200, binary_image_thermometer);
    canvas.pushImage(170, 544, 200, 200, binary_image_thermometer_humidity);

    draw_text(String(temperature), 280, 292, false, 80, 15);
    draw_text(String(humidity), 280, 644, false, 80, 15);

    canvas.pushCanvas(0, 0, mode);
}

void ui_manager::draw_state_sleep(m5epd_update_mode_t mode)
{
    !canvas.isRenderExist(32) && canvas.createRender(32, 256);

    canvas.setTextSize(32);
    canvas.setTextColor(15);

    canvas.drawCentreString("sleeping...", 270, 440, 2);

    canvas.pushCanvas(0, 0, mode);
}

void ui_manager::draw_state_deep_sleep(m5epd_update_mode_t mode)
{
    canvas.fillCanvas(0xFFFF);

    !canvas.isRenderExist(32) && canvas.createRender(32, 256);

    canvas.setTextSize(32);
    canvas.setTextColor(0);

    canvas.drawCentreString("sleeping...", 270, 440, 2);

    canvas.pushCanvas(0, 0, mode);

    Serial.print("SLEEPING.... \n");
}

void ui_manager::draw_state_reset(m5epd_update_mode_t mode)
{
    !canvas.isRenderExist(32) && canvas.createRender(32, 256);

    canvas.setTextSize(32);
    canvas.setTextColor(15);

    canvas.drawCentreString("resetting...", 270, 440, 2);

    canvas.pushCanvas(0, 0, mode);
}

bool ui_manager::draw_notification(String text)
{

    if (is_touch_event_started)
    {
        return false;
    }

    Serial.print(text + "\n");

    extra_canvas.clear();

    !extra_canvas.isRenderExist(22) && extra_canvas.createRender(22, 256);

    extra_canvas.setTextSize(22);
    extra_canvas.setTextColor(15);

    extra_canvas.drawRightString(text, screen_width - 16, 0, 1);

    M5.EPD.WritePartGram4bpp(0, screen_height - 48, screen_width, 48, (uint8_t *)extra_canvas.frameBuffer());
    M5.EPD.UpdateArea(0, screen_height - 48, screen_width, 48, UPDATE_MODE_GLD16);

    return true;
}

void ui_manager::draw_title(String title)
{
    draw_text(title, 24, 24, false, 48, 15);
}

void ui_manager::draw_text(String text, int x, int y, bool alignRight, uint8_t size, uint16_t color)
{
    !canvas.isRenderExist(size) && canvas.createRender(size, 256);

    canvas.setTextSize(size);
    canvas.setTextColor(color);

    if (alignRight)
    {
        int screenWidth = 540;

        canvas.drawRightString(text, screenWidth - x, y, 2);
    }
    else
    {
        int stringWidth = canvas.drawString(text, 0, 1000);

        canvas.drawString(text, stringWidth / 2 + x, y, 2);
    }
}

void ui_manager::press_button(int id)
{
    if (0 <= id && id < buttons_.size())
    {
        ui_object button = buttons_[id];

        auto pos = button.get_pos();
        auto size = button.get_size();

        M5.EPD.WritePartGram4bpp(pos.x, pos.y, size.w, size.h, button.get_active_state_image_data());
        M5.EPD.UpdateArea(pos.x, pos.y, size.w, size.h, UPDATE_MODE_DU);
    }
}

void ui_manager::release_button(int id)
{
    if (0 <= id && id < buttons_.size())
    {
        ui_object button = buttons_[id];

        auto pos = button.get_pos();
        auto size = button.get_size();

        M5.EPD.WritePartGram4bpp(pos.x, pos.y, size.w, size.h, button.get_default_state_image_data());
        M5.EPD.UpdateArea(pos.x, pos.y, size.w, size.h, UPDATE_MODE_DU);
    }
}

int ui_manager::get_touched_button_id(int x, int y)
{
    for (ui_object &button : buttons_)
    {
        auto pos = button.get_rect();

        if (pos.min_pos.x <= x &&
            x <= pos.max_pos.x &&
            pos.min_pos.y <= y &&
            y <= pos.max_pos.y)
        {
            return button.get_id();
        }
    }

    return -1;
}

ui_touch_event_t ui_manager::get_touch_event()
{
    ui_touch_event_t event;
    event.id = -1;
    event.type = SINGLE_PRESS;

    if (M5.TP.available())
    {
        M5.TP.update();

        if (!M5.TP.isFingerUp())
        {
            if (is_touch_event_started == false)
            {
                tp_finger_t touch = M5.TP.readFinger(0);

                touched_button_id = get_touched_button_id(touch.x, touch.y);

                if (touched_button_id >= 0)
                {
                    is_touch_event_started = true;
                    touch_event_start_time = SYS_->get_time();

                    press_button(touched_button_id);

                    Serial.printf("Touch Start: (%d, %d), Button Id: %d \r\n", touch.x, touch.y, touched_button_id);
                }
            }
        }
        else
        {
            if (is_touch_event_started)
            {
                is_touch_event_started = false;

                release_button(touched_button_id);

                tp_finger_t touch = M5.TP.readFinger(0);

                if (touched_button_id == get_touched_button_id(touch.x, touch.y))
                {
                    Serial.printf("Touch End (%d, %d), Button Id: %d \r\n", touch.x, touch.y, touched_button_id);

                    event.id = touched_button_id;
                    event.type = SYS_->get_time() - touch_event_start_time <= 1 ? SINGLE_PRESS : LONG_PRESS;

                    return event;
                }
                else
                {
                    Serial.printf("Touch End: (%d, %d)\r\n", touch.x, touch.y);
                }

                touch_event_start_time = 0;
            }
        }
    }

    return event;
}

int ui_manager::get_buttons_count()
{
    return buttons_.size();
}
