#include <Arduino.h>
#include "Adafruit_TCS34725.h"

#ifndef SENSORCOLOR_H
#define SENSORCOLOR_H

class SensorColor {
private:
  Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_1X);
  
  //Pines para las salidas
  int pinR, pinG, pinB;

public:
    SensorColor(int pinRojo, int pinVerde, int pinAzul) {
    pinR = pinRojo;
    pinG = pinVerde;
    pinB = pinAzul;
  }

  //inicializar el sensor
  bool iniciar() {
    pinMode(pinR, OUTPUT);
    pinMode(pinG, OUTPUT);
    pinMode(pinB, OUTPUT);

    if (tcs.begin()) {
      Serial.println("Inicializando");
      delay(2000);
      return true;
    } else {
      Serial.println("Error");
      Serial.println("Debe de revisar las conexiones e iniciar nuevamente");
      return false;
    }
  }

  //Leer y procesar color
  void procesarColor() {
    uint16_t R, G, B, C;
    tcs.getRawData(&R, &G, &B, &C);

    if (R > G && R > B) {
      R += 30;
      if (G >= 20) G -= 20; else G = 0;
    }

    if (G > R && G > B) {
      if (R >= 20) R -= 20; else R = 0;
      if (G >= 20) G -= 20; else G = 0;
      B += 30;
    }

    //Límites
    if (R > 255) { R = 255; }
    if (G > 255) { G = 255; }
    if (B > 255) { B = 255; }

    //Salida analógica a los pines
    analogWrite(pinR, R);
    analogWrite(pinG, G);
    analogWrite(pinB, B);

    Serial.println(" R = " + String(R) + " G = " + String(G) + " B = " + String(B));
  }
};

#endif



