#ifndef _FAST_LED_H
#define _FAST_LED_H

#include <Arduino.h>
#include <spi_dma.h>
/* Kiểu sắp xếp mã màu trong gói tin của 1 điểm ảnh */
#define COLOR_RGB         0   
#define COLOR_GRB         1
#define COLOR_BRG         2
#define COLOR_GBR         3
/* Giá trị mặc định của LED */
#define DEFAULT_BRIGHTNESS          (uint8_t)127
#define DEFAULT_MODE                (uint8_t)0
#define DEFAULT_SPEED               (uint8_t)128
#define DEFAULT_INTENSITY           (uint8_t)128
#define DEFAULT_COLOR               (uint32_t)0xFFAA00
/* Kiểu giao tiếp của LED */
#define PROTOCOL_1_WIRE             0
#define PROTOCOL_2_WIRE             1
/* Kiểu  gửi dữ liệu của LED */
#define TRANSFER_MSB                0
#define TRANSFER_LSB                1
/* Kiểu đi dây LED */
#define SERIAL_TYPE                 0
#define PARALEL_TYPE                1

#define NUM_COLORS       3 /* number of colors per segment */
#define MAX_LED          1000   // Cho phép sử dụng tối đa 1000 bóng Led

// Màu sắc cơ bản cho LED */
#define RED             (uint32_t)0xFF0000
#define GREEN           (uint32_t)0x00FF00
#define BLUE            (uint32_t)0x0000FF
#define WHITE           (uint32_t)0xFFFFFF
#define BLACK           (uint32_t)0x000000
#define YELLOW          (uint32_t)0xFFFF00
#define CYAN            (uint32_t)0x00FFFF
#define MAGENTA         (uint32_t)0xFF00FF
#define PURPLE          (uint32_t)0x400080
#define ORANGE          (uint32_t)0xFF3000
#define PINK             (uint32_t)0xFF1493
#define ULTRAWHITE      (uint32_t)0xFFFFFFFF

class fastLed
{
private:
    /* data */
public:
  
  typedef struct
  {
    uint8_t     _numberBitData;         // Số bit để quyết định màu của 1 điểm ảnh.
    uint8_t     _typeRGB;               // Kiểu sắp xếp R G B trong bản tin
    uint8_t     _typeProtocol;          // Giao thức giao tiếp của LED (1 dây, 2 dây)
    uint8_t     _typeTransferData;      // Kiểu gửi dữ liệu MSB hay LSB
    uint8_t     _numBitSPICreatBitLed;  // Số bit trong frame SPI tạo ra 1 bit của LED. Ví dụ: 4 bit SPI 1110 1000  ==> 2 bit LED: 1 0 
    uint32_t    _frequencyLed;          // Tần số hoạt động của LED.     
    uint32_t    _dataLED[MAX_LED];      // Dữ liệu của LED chưa xử lý.
    uint8_t*    _pointData;             // Con trỏ dữ liệu SPI
  }Led_Config;
  Led_Config    LEDS;

  typedef struct 
  {
    uint16_t    _indexStart;          //  Vị trí bắt đầu của 1 segment
    uint16_t    _indexStop;           //  Vị trí kết thúc của 1 segment
    uint16_t    _offset;
    uint8_t     _speed;               //  Tốc độ chạy hiệu ứng.
    uint8_t     _mode;                //  Chế độ chạy
    uint8_t     _options;             //  lựa chọn chế độ truyền bit: MSB or LSB
    uint8_t     _grouping, spacing;
    uint8_t     opacity;              //  độ mờ
    uint32_t    colors[NUM_COLORS];
    char        *name;                // Tên của segment
  }Segment;

  typedef struct 
  {
    uint16_t    _numberLedUse;        // Số Led sẽ sử dụng
    uint16_t    _numLedOnColum;       // Số Led trên 1 cột
    uint8_t     _numColum;            // Số Cột.
    uint8_t     _wiringType;          // Kiểu đi dây LED: Đi nối tiếp hoặc song song.     
  }_UserLed;
  _UserLed  UserLed;

  void LED_Init (fastLed *_led,const uint8_t __numColum,const uint16_t __numLedOnColum,const uint8_t __wiringType);
  void LED_Update(fastLed *_led,uint32_t *__data);
  void LED_Display(fastLed *_led);
};


#endif