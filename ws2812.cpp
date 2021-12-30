#include "ws2812.h"

ESP32DMASPI::Master master;
static uint16_t _numByteLedUse = 0; // số byte SPI sử dụng cho LED

fastLed f_Led;

void ws2812::WS2812_Init (fastLed *_led)
{
  uint32_t _frequency=0;
  /* Cấu hình dữ liệu LED */
  _led->LEDS._frequencyLed = 800000;   // 800.000 kHz
  _led->LEDS._numberBitData = 24;      // 24 bit color frame
  _led->LEDS._numBitSPICreatBitLed = 4; // 4 bit SPI tạo được 1 bit LED
  _led->LEDS._typeProtocol = PROTOCOL_1_WIRE;  
  _led->LEDS._typeRGB = COLOR_GBR;
  _led->LEDS._typeTransferData = TRANSFER_MSB;
  /* Cấu hình DMA SPI */
  _numByteLedUse = _led->UserLed._numberLedUse*(_led->LEDS._numberBitData / (8/_led->LEDS._numBitSPICreatBitLed));
  _led->LEDS._pointData = master.allocDMABuffer(_numByteLedUse); // cấp phát bộ nhớ theo số LED sử dụng
  master.setDataMode(SPI_MODE3);
  // Tính tần số cho SPI
  _frequency = (uint32_t)(_led->LEDS._frequencyLed * _led->LEDS._numBitSPICreatBitLed);
  master.setFrequency(_frequency);
  master.setMaxTransferSize(_numByteLedUse);   // Kích thước dữ liệu gửi đi
  master.setDMAChannel(1);  // 1 or 2 only
  master.setQueueSize(1);   // transaction queue size
  // HSPI = CS: 15, CLK: 14, MOSI: 13, MISO: 12
  master.begin();  // default SPI is HSPI
}

void ws2812::WS2812_Convert(fastLed *_led)
{
  uint32_t _mask=0x00000001;
  int _index=0,_indexPoint=0,_indexColum=0;
  uint8_t _indexBit=0,_indexFrame=0;
  uint8_t _data=0;
  uint32_t __buffer=0;
  if(_led->LEDS._typeTransferData == TRANSFER_MSB)
    _mask=0x00800000;
  else
    _mask=0x00000001;
  
  for(_indexColum=0;_indexColum<_led->UserLed._numColum;_indexColum++)
  {
    /**
     * Xử lý dữ liệu riêng của cột.
     * Tùy thuộc vào cách đi dây sẽ xly dữ liệu khác nhau
     */
    for(_index=0;_index<_led->UserLed._numLedOnColum;_index++)// Chuyển đổi dữ liệu của các LED
    {
      uint32_t _sumLed= _indexColum *_led->UserLed._numLedOnColum; 
      for(_indexBit=0;_indexBit<12;_indexBit++)
      {
        /**
         * Nếu giá trị là cột lẻ
         * Đảo chiều nếu đi dây kiểu nối tiếp. Nếu song song sẽ giữ nguyên
         */
        if((_indexColum%2)!=0)
        {
          if(_led->UserLed._wiringType == SERIAL_TYPE)
            __buffer = _sumLed + (_led->UserLed._numLedOnColum - _index -1);
          else
            __buffer = _sumLed + _index;
        }
        else
            __buffer = _sumLed + _index;
        /************************************/    
        for(uint8_t _bit=0;_bit<2;_bit++)
        {
          if((_led->LEDS._dataLED[__buffer]&_mask)==_mask)
          {
            _data<<=4;
            _data|=HIGH_BIT;  
          }
          else
          {
            _data<<=4;
            _data|=LOW_BIT;
          }

        if(_led->LEDS._typeTransferData == TRANSFER_MSB)
          _mask>>=1;
        else
          _mask<<=1;
        }
        _led->LEDS._pointData[_indexPoint] = _data;
        _indexPoint++;
        _data=0;
      }
      if(_led->LEDS._typeTransferData == TRANSFER_MSB)
      _mask=0x00800000;
      else
      _mask=0x00000001;
    }
  }
}
void ws2812::WS2812_Update(fastLed *_led, uint32_t  *_data)
{
  for(uint16_t _index=0;_index<_led->UserLed._numberLedUse;_index++)
    _led->LEDS._dataLED[_index] = _data[_index];
}
void ws2812::WS2812_Display(fastLed *_led)
{
  master.transfer(_led->LEDS._pointData,_numByteLedUse);
}