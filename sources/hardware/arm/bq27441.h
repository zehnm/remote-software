/******************************************************************************
 *
 * Copyright (C) 2018-2019 Marton Borzak <hello@martonborzak.com>
 *
 * This file is part of the YIO-Remote software project.
 *
 * YIO-Remote software is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * YIO-Remote software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with YIO-Remote software. If not, see <https://www.gnu.org/licenses/>.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *****************************************************************************/

#pragma once

#include <QObject>
#include <QtDebug>

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>

#include "../batteryfuelgauge.h"

/*****************************************************************************/
// BQ27441 Device definitions
//
// Default I2C Address
#define BQ27441_I2C_ADDRESS 0x55
// Default Device ID -- expect this value to be returned from readDeviceID()
#define BQ27441_DEVICE_ID 0x0421
// Registers (aka commands) that can be used with readRegister()
// See sections 4.2 - 4.20 and 5.1 - 5.2 in Technical Reference (SLUUAC9A)
#define BQ27441_COMMAND_TEMP 0x02            // 0.1 Kelvins
#define BQ27441_COMMAND_VOLTAGE 0x04         // mV
#define BQ27441_COMMAND_FLAGS 0x06           // See section 4.4
#define BQ27441_COMMAND_NOM_CAPACITY 0x08    // mAh
#define BQ27441_COMMAND_AVAIL_CAPACITY 0x0A  // mAh
#define BQ27441_COMMAND_REM_CAPACITY 0x0C    // mAh
#define BQ27441_COMMAND_FULL_CAPACITY 0x0E   // mAh
#define BQ27441_COMMAND_AVG_CURRENT 0x10     // mA
#define BQ27441_COMMAND_STDBY_CURRENT 0x12   // mA
#define BQ27441_COMMAND_MAX_CURRENT 0x14     // mA
#define BQ27441_COMMAND_AVG_POWER 0x18       // mW
#define BQ27441_COMMAND_SOC 0x1C             // %
#define BQ27441_COMMAND_INT_TEMP 0x1E        // 0.1 Kelvins
#define BQ27441_COMMAND_SOH 0x20             // %
#define BQ27441_COMMAND_REM_CAP_UNFL 0x28    // mAh
#define BQ27441_COMMAND_REM_CAP_FIL 0x2A     // mAh
#define BQ27441_COMMAND_FULL_CAP_UNFL 0x2C   // mAh
#define BQ27441_COMMAND_FULL_CAP_FIL 0x2E    // mAh
#define BQ27441_COMMAND_SOC_UNFL 0x30        // %
#define BQ27441_EXTENDED_OPCONFIG 0x3A       // See section 5.1
#define BQ27441_EXTENDED_CAPACITY 0x3C       // See section 5.2
// Command opcode. Used internally by  getControlWord()
// See section 4.1 in Technical Reference (SLUUAC9A)
#define BQ27441_COMMAND_CONTROL 0x00  //
// Subcommands used with getControlWord
// See sections 4.1.1 - 4.1.6 in Technical Reference (SLUUAC9A)
#define BQ27441_CONTROL_STATUS 0x00         // See section 4.1.1
#define BQ27441_CONTROL_DEVICE_TYPE 0x01    // Should always return 0x0421
#define BQ27441_CONTROL_FW_VERSION 0x02     // See section 4.1.3
#define BQ27441_CONTROL_DM_CODE 0x04        // See section 4.1.4
#define BQ27441_CONTROL_PREV_MACWRITE 0x07  // See section 4.1.5
#define BQ27441_CONTROL_CHEM_ID 0x08        // bq27441-G1A should return 0x0128 and bq27441-G1B should return 0x0312.
/*****************************************************************************/

class BQ27441 : public BatteryFuelGauge {
    Q_OBJECT

 public:
    explicit BQ27441(const QString &i2cDevice = "/dev/i2c-3", int i2cDeviceId = BQ27441_I2C_ADDRESS,
                     QObject *parent = nullptr);
    ~BQ27441() override;

    Q_INVOKABLE void begin() override;
    Q_INVOKABLE int  getVoltage() override;
    Q_INVOKABLE int  getFullChargeCapacity() override;
    Q_INVOKABLE int  getAverageCurrent() override;
    Q_INVOKABLE int  getAveragePower() override;
    Q_INVOKABLE int  getStateOfCharge() override;
    Q_INVOKABLE int16_t getInternalTemperatureC() override;  // Result in 0.1 Celsius
    Q_INVOKABLE int     getStateOfHealth() override;
    Q_INVOKABLE int     getFullAvailableCapacity() override;
    Q_INVOKABLE int     getRemainingCapacity() override;
    Q_INVOKABLE int     getDesignCapacity() override;
    Q_INVOKABLE void    changeCapacity(int newCapacity) override;

    int      getTemperatureC();  // Result in 1 Celsius
    uint16_t getFlags();
    uint16_t getNominalAvailableCapacity();
    int16_t  getStandbyCurrent();
    int16_t  getMaxLoadCurrent();
    uint16_t getInternalTemperature();   // Result in 0.1 Kelvins
    int16_t  getInternalTemperatureF();  // Result in 0.1 Fahrenheit
    uint16_t getRemainingCapacityUnfiltered();
    uint16_t getRemainingCapacityFiltered();
    uint16_t getFullChargeCapacityUnfiltered();
    uint16_t getFullChargeCapacityFiltered();
    uint16_t getStateOfChargeUnfiltered();
    // Extended Data Commands
    uint16_t getOpConfig();

    // Control Subcommands
    uint16_t getControlStatus();
    uint16_t getDeviceType();
    //    uint16_t getFWVersion();
    //    uint16_t getDMCode();
    //    uint16_t getPrevMacwrite();
    uint16_t getChemID();
    void     reset();

    // Device interface
 public:
    bool open() override;
    void close() override;

 private:
    QString m_i2cDevice;
    int     m_i2cDeviceId;
    int     m_i2cFd;
};
