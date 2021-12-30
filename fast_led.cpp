#include <fast_led.h>
#include <ws2812.h>
ws2812 LED_WS2812;

void fastLed::LED_Init(fastLed *_led,const uint8_t __numColum,const uint16_t __numLedOnColum,const uint8_t __wiringType)
{
  _led->UserLed._numColum = __numColum;
  _led->UserLed._numLedOnColum = __numLedOnColum;
  _led->UserLed._wiringType = __wiringType;
  _led->UserLed._numberLedUse = __numColum * __numLedOnColum;
  LED_WS2812.WS2812_Init(_led);
}
void fastLed::LED_Update(fastLed *_led,uint32_t *__data)
{
  LED_WS2812.WS2812_Update(_led,__data);
  LED_WS2812.WS2812_Convert(_led);
}
void fastLed::LED_Display(fastLed *_led)
{
  LED_WS2812.WS2812_Display(_led);
}