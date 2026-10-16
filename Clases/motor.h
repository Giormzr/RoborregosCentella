#ifndef MOTOR_H
#define MOTOR_H

#include <Arduino.h>

class Motor {
private:
  int pinIN1;
  int pinIN2;
  int pinENA;

  void aplicarVelocidad(int velocidad);

public:
  Motor(int _in1, int _in2, int _ena = -1);

  //Funciones de la clase
  void inicializar();
  void avanzar(int velocidad = 200);
  void retroceder(int velocidad = 200);
  void frenar();
};

#endif
