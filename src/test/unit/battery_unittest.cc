/*
 * This file is part of Cleanflight.
 *
 * Cleanflight is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Cleanflight is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Cleanflight.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <stdint.h>

#include <limits.h>

extern "C" {
    #include "sensors/battery.h"
}

#include "unittest_macros.h"
#include "gtest/gtest.h"

typedef struct batteryAdcToVoltageExpectation_s {
    uint16_t adcReading;
    uint16_t expectedVoltageInDeciVoltSteps;
    uint8_t scale;
} batteryAdcToVoltageExpectation_t;

#define ELEVEN_TO_ONE_VOLTAGE_DIVIDER 110 // (10k:1k) * 10 for 0.1V

TEST(BatteryTest, BatteryADCToVoltage)
{
    // given

    batteryConfig_t batteryConfig;

    batteryInit(&batteryConfig);

    batteryAdcToVoltageExpectation_t batteryAdcToVoltageExpectations[] = {
            {1420, 126 /*125.88*/, ELEVEN_TO_ONE_VOLTAGE_DIVIDER},
            {1430, 127 /*126.76*/, ELEVEN_TO_ONE_VOLTAGE_DIVIDER},
            {1440, 128 /*127.65*/, ELEVEN_TO_ONE_VOLTAGE_DIVIDER},
            {1890, 168 /*167.54*/, ELEVEN_TO_ONE_VOLTAGE_DIVIDER},
            {1900, 168 /*168.42*/, ELEVEN_TO_ONE_VOLTAGE_DIVIDER},
            {1910, 169 /*169.31*/, ELEVEN_TO_ONE_VOLTAGE_DIVIDER},
            {   0,   0 /*  0.00*/, VBAT_SCALE_MAX},
            {4096, 842 /*841.71*/, VBAT_SCALE_MAX}
    };
    uint8_t testIterationCount = sizeof(batteryAdcToVoltageExpectations) / sizeof(batteryAdcToVoltageExpectation_t);

    // expect

    for (uint8_t index = 0; index < testIterationCount; index ++) {
        batteryAdcToVoltageExpectation_t *batteryAdcToVoltageExpectation = &batteryAdcToVoltageExpectations[index];
        batteryConfig.vbatscale = batteryAdcToVoltageExpectation->scale;
        printf("adcReading: %d, vbatscale: %d\n",
                batteryAdcToVoltageExpectation->adcReading,
                batteryAdcToVoltageExpectation->scale
        );

        uint16_t pointOneVoltSteps = batteryAdcToVoltage(batteryAdcToVoltageExpectation->adcReading);

        EXPECT_EQ(batteryAdcToVoltageExpectation->expectedVoltageInDeciVoltSteps, pointOneVoltSteps);
    }
}

// STUBS

extern "C" {

uint8_t armingFlags = 0;
int16_t rcCommand[4] = {0,0,0,0};

uint16_t adcGetChannel(uint8_t channel)
{
    UNUSED(channel);
    return 0;
}

void delay(uint32_t ms)
{
    UNUSED(ms);
    return;
}
}
