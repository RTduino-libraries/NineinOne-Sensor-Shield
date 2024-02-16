/*
 * Copyright (c) 2006-2022, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024-01-07     Meco Man     first version
 */

#include <RTduino.h>

#define LED_RED    D12
#define LED_BLUE   D13
#define BUZZER     D5
#define BUTTON1    D2
#define BUTTON2    D3
#define RGB_RED    D9
#define RGB_GREEN  D10
#define RGB_BLUE   D11
#define ROTATION   A0
#define LIGHT_SENSOR A1
#define LM35       A2

static int cnt = 0;

static void _9in1_setup(void)
{
    /* put your setup code here, to run once: */
    pinMode(LED_RED, OUTPUT);
    pinMode(LED_BLUE, OUTPUT);
    pinMode(BUZZER, OUTPUT);
    pinMode(BUTTON1, INPUT);
    pinMode(BUTTON2, INPUT);
}

static void _9in1_loop(void)
{
    /* put your main code here, to run repeatedly: */
    int current_rotation_value = analogRead(ROTATION);
    analogWrite(RGB_RED, 255 - map(current_rotation_value, 0, 1023, 0, 255));
    analogWrite(RGB_GREEN, map(current_rotation_value, 0, 1023, 0, 255));
    analogWrite(RGB_BLUE, 255 - map(current_rotation_value, 0, 1023, 0, 255));
    
    digitalWrite(BUZZER, !digitalRead(BUTTON1) && !digitalRead(BUTTON2));
    digitalWrite(LED_BLUE, !digitalRead(BUTTON1));
    digitalWrite(LED_RED, !digitalRead(BUTTON2));

    cnt ++;
    if (cnt == 10)
    {
        cnt = 0;
        Serial.print("Current light value: ");
        Serial.println(analogRead(LIGHT_SENSOR));
        Serial.print("Current temperature value: ");
        Serial.println(analogRead(LM35));
    }

    delay(50);
}
RTDUINO_SKETCH_LOADER("9in1", _9in1_setup, _9in1_loop);
