#include "motor.h"

Motor::Motor(int  _in1, int _in2, int _ena){
  pinIN1 = _in1;
  pinIN2 = _in2;
  pinENA = _ena;
}


//Función para el control de PWM
void Motor::inicializar()
  {
  pinMode(pinIN1, OUTPUT);
  pinMode(pinIN2, OUTPUT);
  if (pinENA != -1)
  {
  pinMode(pinENA, OUTPUT);
  }
  frenar();
  }

void Motor::aplicarVelocidad(int velocidad)
	{
	if (pinENA != -1)
		{
			analogWrite(pinENA, velocidad);
		}
	}

  void Motor::avanzar(int velocidad)
  {
  digitalWrite(pinIN1, HIGH); //señal alta
  digitalWrite(pinIN2, LOW); //señal baja
  aplicarVelocidad(velocidad);
  }

  void Motor::retroceder(int velocidad)
  {
  digitalWrite(pinIN1, LOW);
  digitalWrite(pinIN2, HIGH);
  aplicarVelocidad(velocidad);
  }
 
  void Motor::frenar()
  {
  	digitalWrite(pinIN1, LOW);
  	digitalWrite(pinIN2, LOW);
  	aplicarVelocidad(0);
  }
