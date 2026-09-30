#include "Adafruit_TCS34725.h"

#ifndef SENSORCOLOR_H
#define SENSORCOLOR_H

class SensorColor {
private:
  Adafruit_TCS34725 tcs;
  int pinR, pinG, pinB; //Pines para las salidas

public:
    SensorColor(int pinRojo, int pinVerde, int pinAzul) 

  //inicializar el sensor
  bool iniciar() ;
void procesarColor();
}

#endif


