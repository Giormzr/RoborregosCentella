CPP

#include <Arduino.h>
#include "Adafruit_TCS34725.h"



SensorUltrasonico sensorIzquierda(7,8);
SensorUltrasonico sensorDerecha(12,13);
SensorUltrasonico sensorFrontal(2,4);

                                                 int salida = 3; //activa el motor

void setup() 
{
  Serial.begin(115200);
  Serial.println("sensores listos");
}

void loop() 
{
  bool paredIzquierda = sensorIzquierda.obstaculo();
  bool paredDerecha = sensorDerecha.obstaculo();
  bool paredFrontal = sensorFrontal.obstaculo();

  if (paredIzquierda)
  {
    Serial.println("[Izquierda] Pared cerca")
  }

  if (paredDerecha)
  {
    Serial.println("[Derecha] Pared cerca")
  }

  if (paredFrontal)
  {
    Setial.println("[Frontal] Pared cerca")
  }

  delay (100);
}


class SensorColor {
  private:

  public:
    int pinR = 9;
    int pinG = 10;
    int pinB = 11;
