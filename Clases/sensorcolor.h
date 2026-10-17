#ifndef SENSORCOLOR_H
#define SENSORCOLOR_H

#include <Arduino.h>
#include <Wire.h>

class SensorColor {
private:
  int pinR, pinG, pinB;
  bool valido;
  uint16_t rawR, rawG, rawB, rawC;
  float rn, gn, bn;

  void escribir(uint8_t reg, uint8_t val);
  uint8_t leer8(uint8_t reg);
  uint16_t leer16(uint8_t reg);

public:
  SensorColor(int pinRojo, int pinVerde, int pinAzul);

  bool iniciar();
  void procesarColor();

  bool Rojo();
  bool Turquesa();
  bool Amarillo();
  bool Naranja();
  bool Rosa();
};

#endif
