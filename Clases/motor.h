#include <Arduino.h>

#ifndef MOTOR_H
#define MOTOR_H

class Motor
{
  private:
    int pinIN1;
    int pinIN2;
    int pinENA;

    void aplicarVelocidad(int velocidad)
    {
      if (pinENA != -1)
      {
        analogWrite(pinENA, velocidad);
      }
    }

  public:
    Motor(int _in1, int _in2, int _ena = -1)
    {
      pinIN1 = _in1;
      pinIN2 = _in2;
      pinENA = _ena;
    }

  void inicializar()
  {
    pinMode(pinIN1, OUTPUT);
    pinMode(pinIN2, OUTPUT);
    if (pinENA != -1)
    {
      pinMode(pinENA, OUTPUT);
    }
    frenar();
  }

  void avanzar(int velocidad = 200) 
  {
    digitalWrite(pinIN1, HIGH); //senal alta
    digitalWrite(pinIN2, LOW); //senal baja
    aplicarVelocidad(velocidad);
  }

  void retroceder(int velocidad=200)
  {
    digitalWrite(pinIN1, LOW);
    digitalWrite(pinIN2, HIGH);
    aplicarVelocidad(velocidad);
  }
  
  void frenar()
  {
  	digitalWrite(pinIN1, LOW);
  	dgitalWrite(pinIN2, LOW);
  	aplicarVelocidad(0);
  }

}

#endif
