/*
 * MIT License
 *
 * Copyright (c) 2020 Erriez
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
 */

/*!
 * \file ErriezBMX280.ino
 * \brief BMP280/BME280 sensor library for Arduino.
 * \details
 *      BMP280 supports temperature and pressure
 *      BME280 supports temperature, pressure and humidity
 * \details
 *     Source:          https://github.com/Erriez/ErriezBMX280
 *     Documentation:   https://erriez.github.io/ErriezBMX280
 */

#include <Wire.h>
#include <ErriezBMX280.h>

// Adjust sea level for altitude calculation
#define SEA_LEVEL_PRESSURE_HPA      1026.25

// Create BMX280 object I2C address 0x76 or 0x77
ErriezBMX280 bmx280 = ErriezBMX280(0x76);


void setup()
{
    // Initialize serial
    delay(500);
    Serial.begin(115200);
    while (!Serial) {
        ;
    }
    Serial.println(F("\nErriez BMP280/BMX280 example"));

    // Initialize I2C bus
    Wire.begin();
    Wire.setClock(400000);

    // Initialize sensor
    while (!bmx280.begin()) {
        Serial.println(F("Error: Could not detect sensor"));
        delay(3000);
    }

    // Print sensor type
    Serial.print(F("\nSensor type: "));
    switch (bmx280.getChipID()) {
        case CHIP_ID_BMP280:
            Serial.println(F("BMP280\n"));
            break;
        case CHIP_ID_BME280:
            Serial.println(F("BME280\n"));
            break;
        default:
            Serial.println(F("Unknown\n"));
            break;
    }

    // Set sampling - Recommended modes of operation
    //
    // Weather
    //  - forced mode, 1 sample / minute
    //  - pressure ×1, temperature ×1, humidity ×1
    //  - filter off
    //
    // Humidity sensing
    //  - forced mode, 1 sample / second
    //  - pressure ×0, temperature ×1, humidity ×1
    //  - filter off
    //
    // Indoor navigation
    //  - normal mode, t standby = 0.5 ms
    //  - pressure ×16, temperature ×2, humidity ×1
    //  - filter coefficient 16
    //
    // Gaming
    //  - forced mode, t standby = 0.5 ms
    //  - pressure ×1, temperature ×1, humidity ×1
    //  - filter off
    bmx280.setSampling(BMX280_MODE_NORMAL,    // SLEEP, FORCED, NORMAL
                       BMX280_SAMPLING_X16,   // Temp:  NONE, X1, X2, X4, X8, X16
                       BMX280_SAMPLING_X16,   // Press: NONE, X1, X2, X4, X8, X16
                       BMX280_SAMPLING_X16,   // Hum:   NONE, X1, X2, X4, X8, X16 (BME280)
                       BMX280_FILTER_X16,     // OFF, X2, X4, X8, X16
                       BMX280_STANDBY_MS_500);// 0_5, 10, 20, 62_5, 125, 250, 500, 1000
}

void loop()
{
    Serial.print(F("Temperature: "));
    Serial.print(bmx280.readTemperature());
    Serial.println(" C");

    if (bmx280.getChipID() == CHIP_ID_BME280) {
        Serial.print(F("Humidity:    "));
        Serial.print(bmx280.readHumidity());
        Serial.println(" %");
    }

    Serial.print(F("Pressure:    "));
    Serial.print(bmx280.readPressure() / 100.0F);
    Serial.println(" hPa");

    Serial.print(F("Altitude:    "));
    Serial.print(bmx280.readAltitude(SEA_LEVEL_PRESSURE_HPA));
    Serial.println(" m");

    Serial.println();

    delay(1000);
}