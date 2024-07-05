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

#include <M5EPD.h>

#include "ui/ui_manager.h"
#include "hap/hap_manager.h"
#include "system/system_utils.h"

#include "info_wifi.h"

system_utils SYS;
hap_manager HAP;

ui_manager UI(&SYS, &HAP);

bool isAwake = false;

time_t last_boot_time = 0;
time_t inactive_time = 0;

time_t last_update_time = 0;

const int sleep_interval = 150; // in seconds 150s - 2.5m
const int refresh_interval = 5;

ui_state_t currentState = IDLE;

void handle_wake_up()
{

    Serial.print(SYS.get_wifi_connected_status());

    isAwake = false;
}

void reset_inactivity_time()
{
    inactive_time = SYS.get_time();
}
int get_inactivity_time()
{
    return SYS.get_time() - inactive_time;
}

void switch_state(ui_state_t nextState)
{
    if (currentState == nextState)
    {
        return;
    }
    else if (currentState == PAIR && nextState == CONTROLS && !HAP.get_homekit_paired_status())
    {
        return;
    }

    currentState = nextState;

    // Actions BEFORE the UI switch -------------------------
    // ------------------------------------------------------

    if (nextState == CONTROLS && !HAP.get_homekit_paired_status())
    {
        nextState = PAIR;
        currentState = PAIR;
    }
    else if (nextState == CONTROLS)
    {
        M5.TP.flush();
    }
    else if (nextState == SENSORS)
    {
        M5.SHT30.UpdateData();
    }

    // UI switch --------------------------------------------
    // ------------------------------------------------------

    UI.switch_state(nextState);

    // Action AFTER the UI switch ---------------------------
    // ------------------------------------------------------

    if (nextState == SLEEP)
    {
        delay(500);

        SYS.sleep(handle_wake_up);
    }

    if (nextState == DEEP_SLEEP)
    {
        delay(500);

        SYS.deep_sleep();
    }

    if (nextState == RESET)
    {
        delay(500);

        HAP.initialize(UI.get_buttons_count(), true);
        SYS.reboot();
    }

    last_update_time = SYS.get_time();
}

void update()
{
    if (get_inactivity_time() >= sleep_interval)
    {
        reset_inactivity_time();

        switch_state(DEEP_SLEEP);
    }

    if (currentState == PAIR && HAP.get_homekit_paired_status())
    {
        switch_state(CONTROLS);
    }
    else if (currentState == CONTROLS && !HAP.get_homekit_paired_status())
    {
        switch_state(PAIR);
    }
    if (currentState != SLEEP && currentState != DEEP_SLEEP && currentState != LOADING)
    {
        if (SYS.get_time() - last_update_time >= refresh_interval)
        {
            if (UI.draw_notification("Battery: " + String(SYS.get_battery_level()) + "%"))
            {
                last_update_time = SYS.get_time();
            }
        }
    }
}

void setup()
{
    M5.begin(true, false, true, true, false);
    UI.initialize();

    switch_state(LOADING);

    M5.SHT30.Begin();
    M5.RTC.begin();

    SYS.set_time();

    last_boot_time = SYS.get_time();
    inactive_time = SYS.get_time();
    last_update_time = SYS.get_time();

    SYS.start_wifi(wifi_ssid, wifi_password);

    HAP.initialize(UI.get_buttons_count());

    switch_state(CONTROLS);

    UI.draw_notification("Battery: " + String(SYS.get_battery_level()) + "%");
}

void loop()
{
    M5.update();
    update();

    if (currentState == CONTROLS)
    {

        ui_touch_event_t event = UI.get_touch_event();

        if (event.id >= 0)
        {
            HAP.button_callback(event.id, event.type);
            reset_inactivity_time();
        }
    }

    if (!isAwake)
    {
        isAwake = true;

        switch_state(CONTROLS);
    }

    // ------------------------------------------------------------------------------------------------------------
    // ------------------------------------------------------------------------------------------------------------

    // Button L is pressed
    if (M5.BtnL.isPressed())
    {
        switch_state(CONTROLS);
    }

    if (M5.BtnL.pressedFor(5000))
    {
        switch_state(RESET);
    }

    if (M5.BtnR.isPressed())
    {
        switch_state(SENSORS);
    }

    if (M5.BtnP.pressedFor(3000))
    {
        switch_state(DEEP_SLEEP);
    }
    else if (M5.BtnP.wasReleased())
    {
        switch_state(INFO);
    }

    // ------------------------------------------------------------------------------------------------------------
    // ------------------------------------------------------------------------------------------------------------

    if (M5.BtnR.isPressed() || M5.BtnP.isPressed() || M5.BtnL.isPressed())
    {
        reset_inactivity_time();
    }
}
