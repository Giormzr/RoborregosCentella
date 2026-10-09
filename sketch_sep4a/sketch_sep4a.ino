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
const int PINR = 20;
const int PING = 21;
const int PINB = 22;

//creamos aquí los objetos

Motor motorA(IN1, IN2, ENA);
Motor motorB(IN3, IN4, ENB);

SensorUltrasonico sensor1(TRIGGER_1, ECHO_1);
SensorUltrasonico sensor2(TRIGGER_2, ECHO_2);
SensorUltrasonico sensor3(TRIGGER_3, ECHO_3);

SensorColor sensorColor(PINR, PING, PINB);


//Estructura para el historial de pasos (bonus point 1)

enum estadoRobot { 
	Explorar, //Paredes y ultrasonicos
	LlegadaMeta, //Se detiene cuando detecta el rojo de la meta
	RegresarInicio, //Se regresa para el bonus 
	Fin //Se apaga 
};

estadoRobot estadoActual = Explorar;

enum Movimiento {AVANZAR, GiroIzq, GiroDer, RETROCEDER}; 
Movimiento historialPasos[50]; //Guarda hasta 50 pasos
int totalPasos = 0; //Inicia en 0

//Para guardar el recorrido
void registrarPaso(Movimiento paso){
	if (totalPasos < 50){
		historialPasos[totalPasos] = paso;
		totalPasos++;
	}
}

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
	
void girarDerechaCentellita()
	{
		motorA.avanzar();
		motorB.retroceder();
	}
	
void girarIzquierdaCentellita()
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
	sensorColor.iniciar();
	
	Serial.println("Centellita listo :p");
}

void loop ()
{
	switch (estadoActual){
		case Explorar: {
			sensorColor.procesarColor();

			//Para el rojo de la meta
			if (sensorColor.Rojo()){
				frenarCentellita();
				estadoActual = LlegadaMeta;
				break;
			}
	
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
			registrarPaso(GiroIzq);
		}
	
		else if (!paredSensor3)
		{
			Serial.println("pared derecha, girando a la izquierda");
			girarDerechaCentellita();
			delay(400);
			registrarPaso(GiroDer);
		}
	
		else 
		{
			Serial.println("pared en todos lados, retrocediendo");
			retrocederCentellita();
			delay(600);
			registrarPaso(RETROCEDER);
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
			registrarPaso(AVANZAR);
		}
	}
	break;
}

case LlegadaMeta:
	Serial.println("Meta Alcanzada! Preparando Bonus");
	frenarCentellita();
	delay(2000);
	estadoActual = RegresarInicio;
	break;

case RegresarInicio:
	Serial.println("Iniciando recorrido de regreso");

	//Bucle invertido para leer las decisiones tomadas
	for (int i = totalPasos - 1; i>=0; i--){
		switch (historialPasos[i]){

			case GiroIzq:
				//Si giró a la izquierda de ida, gira a la derecha de regreso
				girarDerechaCentellita();
				delay(400);
				break;

			case GiroDer:
				//Si giró a la derecha de ida, gira a la izquierda de regreso
				girarIzquierdaCentellita();
				delay(400);
				break;

			case AVANZAR:
				//El avance se queda igual
				avanzarCentellita();
				delay(400);
				break;

			case RETROCEDER:
				//El retroceso de la ida se convierte en avance
				avanzarCentellita();
				delay(400);
				break;
		}
	frenarCentellita();
	delay(200);
}

estadoActual = Fin;
break;

case Fin:
	frenarCentellita();
	Serial.println("Se ha completado el bonus");
	break;
}
