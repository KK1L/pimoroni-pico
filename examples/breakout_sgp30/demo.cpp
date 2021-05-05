// SGP30 Air Quality Sensor demo
// Initalises connection, retrieves unique chip ID, starts measurement.
// Call returns immediately so that air quality can be measured every second.
// After 30 seconds, resets the chip, and restarts measurement,
// but that call waits up to 20 seconds for the readings to start being useful.
// Reports status and results to Serial connection on GPIO 0 and 1.

#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"

#include "breakout_sgp30.hpp"

using namespace pimoroni;

BreakoutSGP30 sgp30;

int main() {
  uint8_t prd;
  uint16_t eCO2, TVOC;
  uint16_t raweCO2, rawTVOC;
  uint16_t baseCO2, baseTVOC;

  gpio_init(PICO_DEFAULT_LED_PIN);
  gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);

  stdio_init_all();

  bool init_ok = sgp30.init();
  if(init_ok) {
    printf("SGP30 initialised - about to start measuring without waiting\n");
    sgp30.start_measurement(false);
    printf("Started measuring for id %012llx\n", sgp30.get_unique_id());
    if(sgp30.get_air_quality(&eCO2, &TVOC)) {
      prd = 1;
    }
    else {
      printf("SGP30 not found when reading air quality\n");
      prd = 2;
    }
  }
  else {
    printf("SGP30 not found when initialising\n");
    prd = 3;
  }

  uint16_t j = 0;
  while(true) {
    j++;
    for(uint8_t i=0; i<prd; i++) {
      gpio_put(PICO_DEFAULT_LED_PIN, true);
      sleep_ms(50);
      gpio_put(PICO_DEFAULT_LED_PIN, false);
      sleep_ms(50);
    }
    sleep_ms(1000 - (100 * prd));
    if(prd == 1) {
      sgp30.get_air_quality(&eCO2, &TVOC);
      sgp30.get_air_quality_raw(&raweCO2, &rawTVOC);
      printf("%3d: CO2 %d TVOC %d, raw %d %d\n", j, eCO2, TVOC, raweCO2, rawTVOC);
      if(j == 30) {
        printf("Resetting device\n");
        sgp30.soft_reset();
        sleep_ms(500);
        printf("Restarting measurement, waiting 15 secs before returning\n");
        sgp30.start_measurement(true);
        printf("Measurement restarted, now read every second\n");
      }
    }
  }

  return 0;
}
