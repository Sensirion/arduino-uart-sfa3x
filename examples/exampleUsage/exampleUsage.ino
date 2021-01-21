/*
 * Copyright (c) 2021, Sensirion AG
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * * Neither the name of Sensirion AG nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <Arduino.h>
#include <SensirionShdlcSfa3x.h>

SensirionShdlcSfa3x sfa3x;

void setup() {
    uint16_t error;
    char errorMessage[256];

    Serial.begin(115200);
    while (!Serial) {
        delay(100);
    }

    Serial2.begin(115200);
    while (!Serial2) {
        delay(100);
    }

    sfa3x.begin(Serial2);

    error = sfa3x.deviceReset();
    if (error) {
        Serial.print("Error trying to execute deviceReset(): ");
        errorToString(error, errorMessage, 256);
        Serial.println(errorMessage);
    }
    uint8_t deviceMarking[32];
    uint8_t deviceMarkingSize = 32;
    error = sfa3x.getDeviceMarking(deviceMarking, deviceMarkingSize);
    if (error) {
        Serial.print("Error trying to execute getDeviceMarking(): ");
        errorToString(error, errorMessage, 256);
        Serial.println(errorMessage);
    } else {
        Serial.print("Device Marking:");
        Serial.println((char*)deviceMarking);
    }

    // Start Measurement
    error = sfa3x.startContinuousMeasurement();
    if (error) {
        Serial.print("Error trying to execute startContinuousMeasurement(): ");
        errorToString(error, errorMessage, 256);
        Serial.println(errorMessage);
    }
}

void loop() {
    uint16_t error;
    char errorMessage[256];

    // Read Measurement
    int16_t hcho;
    int16_t relativeHumidity;
    int16_t temperature;
    error = sfa3x.readMeasuredValuesOutputFormat2(hcho, relativeHumidity,
                                                  temperature);
    if (error) {
        Serial.print(
            "Error trying to execute readMeasuredValuesOutputFormat2(): ");
        errorToString(error, errorMessage, 256);
        Serial.println(errorMessage);
    } else {
        Serial.print("Hcho:");
        Serial.print(hcho);
        Serial.print("\t");
        Serial.print("Relative Humidity:");
        Serial.print(relativeHumidity);
        Serial.print("\t");
        Serial.print("Temperature:");
        Serial.println(temperature);
    }
}
