CPP

#include "ultrasonico.h"
#include "motor.h"
#include "sensorcolor.h"

//motor A
const int IN1 = 5;
const int IN2 = 6;
const int ENA = 7;

//motor B
const int IN3 = 15;
const int IN4 = 16;
const int ENB = 17;

//sensor ultrasonico 1
const int TRIGGER_1 = 8;
const int ECHO_1 = 9;

//sensor ultrasonico 2
const int TRIGGER_2 = 10;
const int ECHO_2 = 11;

//sensor ultrasonico 3
const int TRIGGER_3 = 12;
const int ECHO_3 = 13;

//sensor de color
const int PINR = __;
const int PING = __;
const int PINB = __;

//creacmos aquí los objetos

Motor motorA(IN1, IN2, ENA);
Motor motorB(IN3, IN4, ENB);

SensorUltrasonico sensor1(TRIGGER_1, ECHO_1);
SensorUltrasonico sensor2(TRIGGER_2, ECHO_2);
SensorUltrasonico sensor3(TRIGGER_3, ECHO_3);

SensorColor _______(_____,____);

void avanzarCentellita()
	{
		motorA.avanzar();
		motorB.avanzar();
	}
	
void frenarCentellita()
	{
		motorA.frenar();
		motorB.frenar();
	}
	
void retrocederCentellita()
	{
		motorA.retroceder();
		motorB.retroceder();
	}
	
void girarderechaCentellita()
	{
		motorA.avanzar();
		motorB.retroceder();
	}
	
void girarizquierdaCentellita()
	{
		motorA.retroceder();
		motorB.avanzar();
	}
	
void setup()

{
	Serial.begin (9600);
	
	motorA.inicializar();
	motorB.inicializar();
	
	sensor1.inicializar();
	sensor2.inicializar();
	sensor3.inicializar();
	
	
	//SENSOR DE COLOR
	
	serial.printl("Centellita listo :p");
}

void loop ()
{
	bool paredSensor1 = sensor1.paredCerca();
	bool paredSensor2 = sensor2.paredCerca();
	bool paredSensor3 = sensor3.paredCerca();
	
	//asumiendo que "Sensor 1" es el de ENFRENTE, el "Sensor 2" es IZQUIERDA y el "Sensor 3" es DERECHA.
	
	if (paredSensor1)
	{
		Serial.println("pared cerca, buscando camino");
		frenarCentellita();
		delay(200);
	
	
		if (!paredSensor2)
		{
			Serial.println("pared izquierda, girando a la derecha");
			girarIzquierdaCentellita();
			delay(400);
		}
	
		else if (!paredSensor3)
		{
			Serial.println("pared derecha, girando a la izquierda");
			girarDerechaCentellita();
			delay(400);
		}
	
		else 
		{
			Serial.println("pared en todos lados, retrocediendo");
			retrocederCentellita();
			delay(600);
		}
	}
	
	else
	{
		if (paredSensor2)
		{
			Serial.println("pared izquierda, girando a la derecha");
			girarDerechaCentellita();
			delay(100);
		}
		
		else if (paredSensor3)
		{
			Serial.println("pared derecha, girando a la izquierda");	
			girarIzquierdaCentellita();
			delay(100);
		}
		
		else
		{
			Serial.println("camino libre, avanzando recto");
			avanzarCentellita();
		}
	}
	
	delay (50);
}

/////////////////////

class SensorColor {
  private:

  public:
    int pinR = 9;
    int pinG = 10;
    int pinB = 11;
