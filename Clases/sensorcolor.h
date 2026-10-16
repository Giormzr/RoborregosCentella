#ifndef SENSORCOLOR_H
#define SENSORCOLOR_H

#include "Adafruit_TCS34725.h"

class SensorColor {
private:
  Adafruit_TCS34725 tcs;
  int pinR, pinG, pinB; //Pines para las salidas
  uint16_t redVal, greenVal, blueVal; //Guarda la intensidad del color que rebota

public:
    SensorColor(int pinRojo, int pinVerde, int pinAzul); 

  //inicializar el sensor
  bool iniciar() ;
  void procesarColor();

  bool Rojo(); 
  bool Turquesa ();
  bool Amarillo ();
  bool Naranja ();
  bool Rosa();
  

};

#endif
