CPP

#incluide "ultrasonico.h"
#incluide "motor.h"
#incluide "sensorcolor.h"

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

Motor motorA(IN1, IN2, INA);
Motor motorB(IN3, IN4, INB);

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
		motorB.retroceder()
	}
	
void girarizquierdaCentellita()
	{
		motorA.retroceder();
		motorB.avanzar();
	}
	
void setup()

{
	serial.begin (9600);
	
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
	bool
}

/////////////////////

class SensorColor {
  private:

  public:
    int pinR = 9;
    int pinG = 10;
    int pinB = 11;
