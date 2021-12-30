#ifndef WS2812_H
#define WS2812_H
#include "fast_led.h"

#define HIGH_BIT      0x0E
#define LOW_BIT       0x08

class ws2812
{
  private:

      /* data */
  public:
    void WS2812_Init (fastLed *_led);
    void WS2812_Convert(fastLed *_led);
    void WS2812_Update(fastLed *_led, uint32_t  *_data);
    void WS2812_Display(fastLed *_led);
};

#endif