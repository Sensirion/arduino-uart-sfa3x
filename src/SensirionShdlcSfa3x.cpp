/*
 * THIS FILE IS AUTOMATICALLY GENERATED AND MUST NOT BE EDITED MANUALLY!
 *
 * SHDLC-Generator: 0.8.2
 * Yaml Version: 0.1.0
 * Template Version: 0.2.0
 */
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

#include "SensirionShdlcSfa3x.h"
#include "Arduino.h"
#include "SensirionCoreArduinoLibrary.h"

#define SFA3X_UART_ADDRESS 0x00

SensirionShdlcSfa3x::SensirionShdlcSfa3x() {
}

void SensirionShdlcSfa3x::begin(Stream& serial) {
    _serial = &serial;
}

uint16_t SensirionShdlcSfa3x::startContinuousMeasurement() {
    uint16_t error;
    uint8_t buffer[12];
    SensirionShdlcTxFrame txFrame(buffer, 12);
    SensirionShdlcRxFrame rxFrame(buffer, 12);

    error = txFrame.begin(0x00, SFA3X_UART_ADDRESS, 1);
    error |= txFrame.addUInt8(0x00);
    error |= txFrame.finish();
    if (error) {
        return error;
    }

    error = SensirionShdlcCommunication::sendAndReceiveFrame(*_serial, txFrame,
                                                             rxFrame, 10000);
    if (error) {
        return error;
    }

    return error;
}

uint16_t SensirionShdlcSfa3x::stopMeasurement() {
    uint16_t error;
    uint8_t buffer[12];
    SensirionShdlcTxFrame txFrame(buffer, 12);
    SensirionShdlcRxFrame rxFrame(buffer, 12);

    error = txFrame.begin(0x01, SFA3X_UART_ADDRESS, 0);
    error |= txFrame.finish();
    if (error) {
        return error;
    }

    error = SensirionShdlcCommunication::sendAndReceiveFrame(*_serial, txFrame,
                                                             rxFrame, 10000);
    if (error) {
        return error;
    }

    return error;
}

uint16_t SensirionShdlcSfa3x::readMeasuredValuesOutputFormat2(
    int16_t& hcho, int16_t& relativeHumidity, int16_t& temperature) {
    uint16_t error;
    uint8_t buffer[24];
    SensirionShdlcTxFrame txFrame(buffer, 24);
    SensirionShdlcRxFrame rxFrame(buffer, 24);

    error = txFrame.begin(0x03, SFA3X_UART_ADDRESS, 1);
    error |= txFrame.addUInt8(0x02);
    error |= txFrame.finish();
    if (error) {
        return error;
    }

    error = SensirionShdlcCommunication::sendAndReceiveFrame(*_serial, txFrame,
                                                             rxFrame, 100000);
    if (error) {
        return error;
    }

    error |= rxFrame.getInt16(hcho);
    error |= rxFrame.getInt16(relativeHumidity);
    error |= rxFrame.getInt16(temperature);
    return error;
}

uint16_t SensirionShdlcSfa3x::getDeviceMarking(uint8_t deviceMarking[],
                                               uint8_t deviceMarkingSize) {
    uint16_t error;
    uint8_t buffer[522];
    SensirionShdlcTxFrame txFrame(buffer, 522);
    SensirionShdlcRxFrame rxFrame(buffer, 522);

    error = txFrame.begin(0xD0, SFA3X_UART_ADDRESS, 1);
    error |= txFrame.addUInt8(0x06);
    error |= txFrame.finish();
    if (error) {
        return error;
    }

    error = SensirionShdlcCommunication::sendAndReceiveFrame(*_serial, txFrame,
                                                             rxFrame, 10000);
    if (error) {
        return error;
    }

    error |= rxFrame.getBytes(deviceMarking, deviceMarkingSize);
    return error;
}

uint16_t SensirionShdlcSfa3x::deviceReset() {
    uint16_t error;
    uint8_t buffer[12];
    SensirionShdlcTxFrame txFrame(buffer, 12);
    SensirionShdlcRxFrame rxFrame(buffer, 12);

    error = txFrame.begin(0xD3, SFA3X_UART_ADDRESS, 0);
    error |= txFrame.finish();
    if (error) {
        return error;
    }

    error = SensirionShdlcCommunication::sendAndReceiveFrame(*_serial, txFrame,
                                                             rxFrame, 200000);
    if (error) {
        return error;
    }

    delay(200);
    return error;
}
