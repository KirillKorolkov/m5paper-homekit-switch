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

#include "system_utils.h"

rtc_time_t RTCtime;
rtc_date_t RTCDate;

const int battery_voltage_max = 4350;
const int battery_voltage_min = 3300;

system_utils::system_utils()
{
}

bool system_utils::start_wifi(const char *wifi_ssid, const char *wifi_password)
{
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();

    WiFi.setAutoConnect(true);
    WiFi.setAutoReconnect(true);

    Serial.print("WiFi Connecting to ");
    Serial.println(wifi_ssid);

    delay(100);

    WiFi.begin(wifi_ssid, wifi_password);

    for (int retry = 0; WiFi.status() != WL_CONNECTED && retry < 30; retry++)
    {
        delay(500);

        Serial.print(".");
    }

    Serial.println("\n");

    if (WiFi.status() == WL_CONNECTED)
    {
        Serial.println("WiFi Connected");

        return true;
    }
    else
    {
        Serial.println("WiFi Connection is Failed");

        return false;
    }
}

bool system_utils::get_wifi_connected_status()
{
    return WiFi.status() == WL_CONNECTED;
}

String system_utils::get_wifi_address()
{
    return WiFi.localIP().toString();
}

int system_utils::get_battery_level()
{
    float voltage = M5.getBatteryVoltage();
    // voltage = max((int)voltage, battery_voltage_max);
    // voltage = min((int)voltage, battery_voltage_min);

    float charge = (voltage - 3300) / (battery_voltage_max - 3300);

    charge = max(charge, (float)0.01);
    charge = min(charge, (float)1.0);

    return int(charge * 100);
}

void system_utils::reboot()
{
    Serial.println("reboot");

    esp_restart();
}

void system_utils::sleep(void (*wake_up_handler)())
{
    Serial.flush();
    Serial.println("before sleep");

    delay(500);

    esp_sleep_enable_ext0_wakeup(GPIO_NUM_36, LOW);
    esp_light_sleep_start();

    Serial.println("after sleep");

    wake_up_handler();
}

void system_utils::deep_sleep()
{
    Serial.flush();

    M5.update();

    delay(500);

    esp_sleep_enable_ext0_wakeup(GPIO_NUM_36, LOW);
    esp_deep_sleep_start();
}

void system_utils::set_time()
{
    RTCtime.hour = 00;
    RTCtime.min = 00;
    RTCtime.sec = 00;

    M5.RTC.setTime(&RTCtime);

    RTCDate.year = 2020;
    RTCDate.mon = 1;
    RTCDate.day = 29;

    M5.RTC.setDate(&RTCDate);
}

time_t system_utils::get_time()
{
    M5.RTC.getTime(&RTCtime);
    M5.RTC.getDate(&RTCDate);

    struct tm currentTime;

    currentTime.tm_year = RTCDate.year - 1900;
    currentTime.tm_mon = RTCDate.mon - 1;
    currentTime.tm_mday = RTCDate.day;
    currentTime.tm_hour = RTCtime.hour;
    currentTime.tm_min = RTCtime.min;
    currentTime.tm_sec = RTCtime.sec;

    return mktime(&currentTime);
}

bool system_utils::take_screenshot(M5EPD_Canvas &canvas, String filename, bool bFull24bpp = true, int x = 0, int y = 0, int w = -1, int h = -1)
{
    delay(500);

    File bmpFile = SD.open(filename, FILE_WRITE);
    if (!bmpFile)
    {
        log_e("Failed to open '%s' for write", filename);
        return false;
    }

    w = max(1, min(w == -1 ? canvas.width() : w, canvas.width() - x));
    h = max(1, min(h == -1 ? canvas.height() : h, canvas.height() - y));
    if (!bFull24bpp && w % 2 == 1)
        w++;

    log_d("Taking %d x %d screenshot to '%s'", w, h, filename);

    size_t bytes_written = 0;
    auto write8 = [&file = bmpFile, &bytes_written](uint8_t data)
    {
        bytes_written += file.write(data);
    };

    auto write16 = [&file = bmpFile, &bytes_written](uint16_t data)
    {
        bytes_written += file.write(((uint8_t *)&data)[0]);
        bytes_written += file.write(((uint8_t *)&data)[1]);
    };

    auto write32 = [&file = bmpFile, &bytes_written](uint32_t data)
    {
        bytes_written += file.write(((uint8_t *)&data)[0]);
        bytes_written += file.write(((uint8_t *)&data)[1]);
        bytes_written += file.write(((uint8_t *)&data)[2]);
        bytes_written += file.write(((uint8_t *)&data)[3]);
    };

    auto writeN = [&file = bmpFile, &bytes_written](uint8_t *data, size_t N)
    {
        bytes_written += file.write(data, N);
    };

    uint8_t bpp = bFull24bpp ? 24 : 4;
    size_t bmpHeaderSize = 14;
    size_t dibHeaderSize = 40;
    size_t colourTableSize = bpp == 24 ? 0 : pow(2, bpp) * 4;
    size_t gap1Size = 2;
    size_t rowSize = ceil(bpp * w / 32.0) * 4;
    size_t rowGap = rowSize - (bpp * w / 32.0) * 4;
    size_t pixelArrah = rowSize * h;
    size_t pixelArrayOffset = bmpHeaderSize + dibHeaderSize + colourTableSize + gap1Size;
    size_t expectedSize = pixelArrayOffset + pixelArrah;

    //  log_d("Expected size %d, row size %d, row gap %d, pixel array offset %d, pixel array size = %d",expectedSize, rowSize, rowGap, pixelArrayOffset, pixelArrah);

    write16(0x4D42);
    write32(expectedSize);
    write32(0);
    write32(pixelArrayOffset);

    write32(dibHeaderSize);
    write32(w);
    write32(h);
    write16(1);
    write16(bpp);
    write32(0);
    write32(0);
    write32(9252);
    write32(9252);
    write32(0);
    write32(0);

    // From M5EPD_Canvas.cpp
    const uint8_t alphamap[16] = {0, 17, 34, 51, 68, 85, 102, 119, 136, 153, 170, 187, 204, 221, 238, 255};
    if (bpp == 4)
    {
        uint8_t buffer[16 * 4];
        int iCount = 0;
        for (int i = 0; i < 16; i++)
        {
            uint8_t value = 255 - alphamap[i];
            buffer[iCount++] = value;
            buffer[iCount++] = value;
            buffer[iCount++] = value;
            buffer[iCount++] = 0;
        }
        writeN(buffer, iCount);
    }
    for (int i = 0; i < gap1Size; i++)
        write8(0);

    uint8_t buffer[960 * 3];
    //  log_d("After headers, bytes_written = %d",bytes_written);
    for (int y = h + y - 1; y >= y; y--)
    {
        int iCount = 0;
        for (int x = x; x < w + x; x++)
        {
            uint16_t pixel = canvas.readPixel(x, y);
            uint8_t value = min(pixel, (uint16_t)15);
            switch (bpp)
            {
            case 24:
            default:
                value = alphamap[15 - min(value, (uint8_t)15)];
                buffer[iCount++] = value;
                buffer[iCount++] = value;
                buffer[iCount++] = value;
                break;
            case 4:
                if ((x - x) % 2 == 0)
                    buffer[iCount] = (value << 4) & 0xf0;
                else
                {
                    buffer[iCount] |= value & 0x0f;
                    iCount++;
                }
                break;
            }
        }
        for (int gap = 0; gap < rowGap; gap++)
            buffer[iCount++] = 0;
        writeN(buffer, iCount);
    }

    bmpFile.close();

    if (bytes_written != expectedSize)
        log_e("Total bytes written %d vs expected %d", bytes_written, expectedSize);

    log_d("Screenshot done");

    return true;
}
