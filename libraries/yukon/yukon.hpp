#pragma once

#include "pico_graphics.hpp"
#include "common/pimoroni_common.hpp"
#include "drivers/tca9555/tca9555.hpp"

namespace pimoroni {

  struct TCA {
    uint CHIP;
    uint GPIO;
  };

  struct SLOT {
    uint ID;
    uint FAST1;
    uint FAST2;
    uint FAST3;
    uint FAST4;
    TCA SLOW1;
    TCA SLOW2;
    TCA SLOW3;
    uint ADC1_ADDR;
    uint ADC2_TEMP_ADDR;
  };

  class Yukon {
  public:
    static const SLOT SLOT1;
    static const SLOT SLOT2;
    static const SLOT SLOT3;
    static const SLOT SLOT4;
    static const SLOT SLOT5;
    static const SLOT SLOT6;

    static const uint NUM_SLOTS = 6;

    static const uint SDA = 24;
    static const uint SCL = 25;

    static const uint GP26 = 26;
    static const uint GP27 = 27;

    static const uint INT = 28;

    static const uint SHARED_ADC = 29;

    static const TCA MAIN_EN;  // {0, 6}
    static const TCA USER_SW;  // {0, 7}

    static const TCA ADC_ADDR_1;  // {0, 12}
    static const TCA ADC_ADDR_2;  // {0, 13}
    static const TCA ADC_ADDR_3;  // {0, 14}
    static const TCA ADC_MUX_EN_1;  // {0, 15}
    static const TCA ADC_MUX_EN_2;  // {0, 11}

    static const TCA SW_A;  // {1, 1}
    static const TCA SW_B;  // {1, 2}
    static const TCA LED_A;  // {1, 3}
    static const TCA LED_B;  // {1, 4}

    static const TCA LCD_BL;  // {1, 0}
    static const TCA LCD_DC;  // {1, 8}
    static const TCA LCD_CS;  // {1, 9}

    static const uint CURRENT_SENSE_ADDR = 12;  // 0b1100
    static const uint TEMP_SENSE_ADDR = 13;     // 0b1101
    static const uint VOLTAGE_SENSE_ADDR = 14;  // 0b1110
    static const uint EX_ADC_ADDR = 15;         // 0b1111
  private:
    I2C i2c;
    TCA9555 tca0;
    TCA9555 tca1;

  public:
    Yukon() :
      i2c(24, 25),
      tca0(&i2c, 0x20),
      tca1(&i2c, 0x26) {}

    ~Yukon();

    void init();
    void reset();
  };

}