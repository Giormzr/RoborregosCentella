#include "sensorcolor.h"

#define TCS_ADDR 0x29
#define LUZ_MINIMA 20

SensorColor::SensorColor(int pinRojo, int pinVerde, int pinAzul) {
  pinR = pinRojo;
  pinG = pinVerde;
  pinB = pinAzul;
  valido = false;
  rawR = rawG = rawB = rawC = 0;
  rn = gn = bn = 0;
}

void SensorColor::escribir(uint8_t reg, uint8_t val) {
  Wire.beginTransmission(TCS_ADDR);
  Wire.write(0x80 | reg);
  Wire.write(val);
  Wire.endTransmission();
}

uint8_t SensorColor::leer8(uint8_t reg) {
  Wire.beginTransmission(TCS_ADDR);
  Wire.write(0x80 | reg);
  Wire.endTransmission();
  Wire.requestFrom((uint8_t)TCS_ADDR, (uint8_t)1);
  return Wire.available() ? Wire.read() : 0xEE;
}

uint16_t SensorColor::leer16(uint8_t reg) {
  Wire.beginTransmission(TCS_ADDR);
  Wire.write(0xA0 | reg);
  Wire.endTransmission();
  Wire.requestFrom((uint8_t)TCS_ADDR, (uint8_t)2);
  uint16_t lo = Wire.available() ? Wire.read() : 0;
  uint16_t hi = Wire.available() ? Wire.read() : 0;
  return (hi << 8) | lo;
}

bool SensorColor::iniciar() {
  pinMode(pinR, OUTPUT);
  pinMode(pinG, OUTPUT);
  pinMode(pinB, OUTPUT);

  Wire.begin(21, 22);
  Wire.setClock(100000);
  delay(500);

  for (int intento = 0; intento < 3; intento++) {
    uint8_t id = leer8(0x12);
    Serial.print("Sensor color ID: 0x");
    Serial.println(id, HEX);

    if (id == 0x44 || id == 0x4D || id == 0x10) {
      escribir(0x01, 0xEB);
      escribir(0x0F, 0x01);
      escribir(0x00, 0x01);
      delay(10);
      escribir(0x00, 0x03);
      delay(700);

      if (leer8(0x00) == 0x03) {
        Serial.println("Sensor de color OK");
        return true;
      }
      Serial.println("El sensor no acepta escrituras");
    }
    delay(300);
  }
  Serial.println("Sensor de color NO responde, revisa conexiones");
  return false;
}

void SensorColor::procesarColor() {
  rawC = leer16(0x14);
  rawR = leer16(0x16);
  rawG = leer16(0x18);
  rawB = leer16(0x1A);

  uint32_t suma = (uint32_t)rawR + rawG + rawB;
  valido = (rawC >= LUZ_MINIMA && suma > 0);

  if (valido) {
    rn = (float)rawR / suma;
    gn = (float)rawG / suma;
    bn = (float)rawB / suma;
  } else {
    rn = gn = bn = 0;
  }

  analogWrite(pinR, (int)(rn * 255));
  analogWrite(pinG, (int)(gn * 255));
  analogWrite(pinB, (int)(bn * 255));

  Serial.printf("R:%u G:%u B:%u C:%u | rn=%.2f gn=%.2f bn=%.2f\n",
                rawR, rawG, rawB, rawC, rn, gn, bn);
}

// Umbrales APROXIMADOS: calibrar con lo que imprime el monitor
bool SensorColor::Rojo()     { return valido && rn > 0.60 && gn < 0.22; }
bool SensorColor::Naranja()  { return valido && rn > 0.50 && gn >= 0.22 && gn < 0.38 && bn < 0.20; }
bool SensorColor::Amarillo() { return valido && rn > 0.35 && gn > 0.35 && bn < 0.20; }
bool SensorColor::Rosa()     { return valido && rn > 0.38 && bn > 0.25; }
bool SensorColor::Turquesa() { return valido && rn < 0.25 && gn > 0.30 && bn > 0.30; }
