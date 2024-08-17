/*
 * Copyright (c) 2006-2022, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024-01-07     Meco Man     first version
 * 2024-08-17     Meco Man     add DHT11 sensor support
 */

#include <RTduino.h>
#include <DHT.h>

#define LED_RED         D12
#define LED_BLUE        D13
#define BUZZER          D5
#define BUTTON1         D2
#define BUTTON2         D3
#define RGB_RED         D9
#define RGB_GREEN       D10
#define RGB_BLUE        D11
#define ROTATION        A0
#define LIGHT_SENSOR    A1
#define LM35_SENSOR     A2
#define DHT11_SENSOR    D4

static int cnt = 0; /* counter variable */
static DHT dht11(DHT11_SENSOR, DHT11);

static void _9in1_setup(void)
{
    /* put your setup code here, to run once: */
    pinMode(LED_RED, OUTPUT);
    pinMode(LED_BLUE, OUTPUT);
    pinMode(BUZZER, OUTPUT);
    pinMode(BUTTON1, INPUT);
    pinMode(BUTTON2, INPUT);
    dht11.begin();
}

static void _9in1_loop(void)
{
    /* put your main code here, to run repeatedly: */

    /* read the rotation sensor value */
    int current_rotation_value = analogRead(ROTATION); 

    /* set the LEDs brightness, based on the rotation sensor value */
    analogWrite(RGB_RED, 255 - map(current_rotation_value, 0, 1023, 0, 255));
    analogWrite(RGB_GREEN, map(current_rotation_value, 0, 1023, 0, 255));
    analogWrite(RGB_BLUE, 255 - map(current_rotation_value, 0, 1023, 0, 255));

    /* set the buzzer, red LED and blue LED based on the button1 and button2 status */
    /* if button1 and button2 are pressed, the buzzer will sound */
    /* if button1 is pressed, the blue LED will light up */
    /* if button2 is pressed, the red LED will light up */
    digitalWrite(BUZZER, !digitalRead(BUTTON1) && !digitalRead(BUTTON2));
    digitalWrite(LED_BLUE, !digitalRead(BUTTON1));
    digitalWrite(LED_RED, !digitalRead(BUTTON2));

    /* print the sensor data regularly */
    if (++cnt % 30 == 0)
    {
        /* print the rotation sensor data */
        Serial.print("[Rotation] value: ");
        Serial.println(current_rotation_value);

        /* print the light sensor data */
        Serial.print("[Brightness] value: ");
        Serial.println(analogRead(LIGHT_SENSOR));

        /* print the LM35 sensor data */
        Serial.print("[LM35] Temperature: ");
        float lm35_voltage = analogRead(LM35_SENSOR) * (3.3 / 1024);
        float lm35_temperature = lm35_voltage * 100;
        Serial.println(lm35_temperature);

        /* print the DHT11 sensor data */
        /* Read DHT11 sensor data */
        float h = dht11.readHumidity();
        /* Read temperature as Celsius (the default) */
        float t = dht11.readTemperature();
        /* Read temperature as Fahrenheit (isFahrenheit = true) */
        float f = dht11.readTemperature(true);

        /* Check if any reads failed and exit early (to try again). */
        if (isnan(h) || isnan(t) || isnan(f)) {
            Serial.println(F("Failed to read from DHT sensor!"));
            return;
        }

        /* Compute heat index in Fahrenheit (the default) */
        float hif = dht11.computeHeatIndex(f, h);
        /* Compute heat index in Celsius (isFahreheit = false) */
        float hic = dht11.computeHeatIndex(t, h, false);

        /* Print DHT11 data */
        Serial.print(F("[DHT11] Humidity: "));
        Serial.print(h);
        Serial.print(F("%  Temperature: "));
        Serial.print(t);
        Serial.print(F("°C "));
        Serial.print(f);
        Serial.print(F("°F  Heat index: "));
        Serial.print(hic);
        Serial.print(F("°C "));
        Serial.print(hif);
        Serial.println(F("°F"));
    }

    delay(50);
}
RTDUINO_SKETCH_LOADER("9in1", _9in1_setup, _9in1_loop);
