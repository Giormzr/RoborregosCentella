#include "Adafruit_TCS34725.h"

#ifndef SENSORCOLOR_H
#define SENSORCOLOR_H

class SensorColor {
private:
  Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_1X);
  
  //Pines para las salidas
  int pinR, pinG, pinB;

public:
    SensorColor(int pinRojo, int pinVerde, int pinAzul) 

  //inicializar el sensor
  bool iniciar() ;
void procesarColor();
}

#endif


