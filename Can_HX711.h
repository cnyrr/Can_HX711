#ifndef Can_HX711_
#define Can_HX711_
#include "Arduino.h"

class Can_HX711{
  public:
    Can_HX711(uint8_t DATA, uint8_t CLOCK);
    ~Can_HX711();

    int32_t degerAl(); 

    int32_t ortalamaDegerAl(uint8_t defa = 8);

    void kalibreEt();

    int32_t gramOlc();
  
  private:
    uint8_t _DATA;
    uint8_t _CLOCK;
    int32_t _KALIBRASYON;
    int32_t _GRAMLIKDEGISIM = 350; // Onceden hesapla.

};
#endif