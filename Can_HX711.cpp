#include "Can_HX711.h"


// Constructor
Can_HX711::Can_HX711(uint8_t DATA, uint8_t CLOCK): _DATA(DATA), _CLOCK(CLOCK) {
  pinMode(DATA, INPUT); // DATA pini giriş olarak ayarlanır.
  pinMode(CLOCK, OUTPUT); // CLOCK pini çıkış olarak ayarlanır.
  digitalWrite(DATA, LOW); // Veri pini güç kapalı.
  digitalWrite(CLOCK, LOW); // Clock pini güç kapalı.
  kalibreEt();
}

// Destructor
Can_HX711::~Can_HX711() {}

// Deger alma metodu
int32_t Can_HX711::degerAl() {
  // 24-bitlik veriyi tutmak için 32-bit değer.
  int32_t Veri = 0;

  // DATA'da güç varken veri hazır değil. O zamana kadar bekle.
  while(digitalRead(_DATA)) {;}

  // Sensör veri vermeye hazır, her adımda bir bitlik veri alınır.
  // HX711 varsayılan modda 24 clock vuruşu alır.
  for (uint8_t i=0; i < 24; i++) {
    digitalWrite(_CLOCK, HIGH); // Pozitif clock vuruşu.
    digitalWrite(_CLOCK, LOW); // Negatif clock vuruşu.

    Veri = Veri << 1; // Sıradaki bit için veri sola ötelenir.
    // İlk ötelemede zaten 0.

    // Bit değeri yazılır.
    if(digitalRead(_DATA)) { 
      Veri = Veri + 1;
    }

  }

  // HX711 varsayılan modda 25. clock vuruşunda DATA pinini tekrar aktileştirir.
  digitalWrite(_CLOCK, HIGH);
  digitalWrite(_CLOCK, LOW);

  // Veri signed 32 bit olarak genişletilir.
  if (Veri & 0x00800000) {
    return Veri | 0xFF800000;
  }

  return Veri;
}


int32_t Can_HX711::ortalamaDegerAl(uint8_t defa = 8) {
  int32_t toplam = 0;
  for (uint8_t i = 0; i < defa; i++) {
    toplam = toplam + degerAl();
  }
  return toplam / defa;
}

void Can_HX711::kalibreEt() {
  _KALIBRASYON = ortalamaDegerAl() / _GRAMLIKDEGISIM;
}

int32_t Can_HX711::gramOlc() {
  int32_t kalibrasyonsuzgram = ortalamaDegerAl() / _GRAMLIKDEGISIM;
  return kalibrasyonsuzgram - _KALIBRASYON;
}


