CPP

#include "ultrasonico.h"
#include "motor.h"
#include "sensorcolor.h"


// #1 PWMA 23, AIN2 22, AIN1 21; PWMB 19, BIN2 18, BIN1 5; STBY 12
// #2 PWMA 13, AIN2 15, AIN1 2; PWMB 17, BIN2 16, BIN1 4; STBY 12
//motor A
const int IN1 = ;
const int IN2 = ;
const int ENA = ;

//motor B
const int IN3 = ;
const int IN4 = ;
const int ENB = ;

//motor C
const int IN5 = ;
const int IN6 = ;
const int ENC = ;

//motor D
const int IN7 = ;
const int IN8 = ;
const int END = ;

const int PIN_STBY = 12;

//sensor ultrasonico 1: FRENTE
const int TRIGGER_1 = ;
const int ECHO_1 = ;

//sensor ultrasonico 2: DERECHA
const int TRIGGER_2 = ;
const int ECHO_2 = ;

//sensor ultrasonico 3: IZQUIERDA
const int TRIGGER_3 = ;
const int ECHO_3 = ;

//sensor de color
const int PINR = ;
const int PING = ;
const int PINB = ;

//creamos aquí los objetos

Motor motorA(IN1, IN2, ENA); //Frontal Izquierdo
Motor motorB(IN3, IN4, ENB); //Trasero Izquierdo
Motor motorC(IN5, IN6, ENC); //Frontal Derecho
Motor motorD(IN7, IN8. END); //Trasero Derecho

SensorUltrasonico sensor1(TRIGGER_1, ECHO_1);
SensorUltrasonico sensor2(TRIGGER_2, ECHO_2);
SensorUltrasonico sensor3(TRIGGER_3, ECHO_3);

SensorColor sensorColor(PINR, PING, PINB);


//Estructura para el historial de pasos (bonus point 1)

enum EstadoRobot { 
	Explorar, //Paredes y ultrasonicos
	LlegadaMeta, //Se detiene cuando detecta el rojo de la meta
	RegresarInicio, //Se regresa para el bonus 
	Fin //Se apaga 
};

EstadoRobot estadoActual = Explorar;

enum Movimiento {AVANZAR, GiroIzq, GiroDer, RETROCEDER}; 
Movimiento historialPasos[150]; //Guarda hasta 150 pasos
int totalPasos = 0; //Inicia en 0

//Para guardar el recorrido
void registrarPaso(Movimiento paso){
	if (totalPasos < 150){
		historialPasos[totalPasos] = paso;
		totalPasos++;
	}
}

void avanzarCentellita()
	{
		motorA.avanzar();
		motorB.avanzar();
		motorC.avanzar();
		motorD.avanzar();
	}
	
void frenarCentellita()
	{
		motorA.frenar();
		motorB.frenar();
		motorC.frenar();
		motorD.frenar();
	}
	
void retrocederCentellita()
	{
		motorA.retroceder();
		motorB.retroceder();
		motorC.retroceder();
		motorD.retroceder();
	}
	
void girarDerechaCentellita()
	{
		motorA.avanzar();
		motorB.avanzar();
		motorC.retroceder();
		motorD.retroceder();
	}
	
void girarIzquierdaCentellita()
	{
		motorA.retroceder();
		motorB.retroceder();
		motorC.avanzar();
		motorD.avanzar();
	}
	
void setup()
{
	Serial.begin (115200);
	
	pinMode(PIN_STBY, OUTPUT);
	digitalWrite(PIN_STBY, HIGH);//habilita motores
	
	motorA.inicializar();
	motorB.inicializar();
	motorC.inicializar();
	motorD.inicializar();
	
	sensor1.inicializar();
	sensor2.inicializar();
	sensor3.inicializar();
	
	if (!sensorColor.iniciar())
	{
		Serial.println("Sensor de color NO DETECTADO correctamente")	
	}
	
	Serial.println("Centellita listo :p");
	delay(2000);
}

void loop ()
{
	switch (estadoActual){
		case Explorar: {
			sensorColor.procesarColor();
			
			if(sensorColor.Turquesa()){
				Serial.println("cuadro TURQUESA detectado");
			}
			else if (sensorColor.Amarillo()){
				Serial.println("cuadro AMARILLO detectado")
			}
			else if (sensorColor.Naranja()){
				Serial.printl("cuadro NARANJA detectado")
			}
			else if (sensorColor.Rosa()){
				Serial.printl("cuadro ROSA detectado")
			}

			//Para el rojo de la meta
			if (sensorColor.Rojo()){
				frenarCentellita();
				estadoActual = LlegadaMeta;
				break;
			}
	
		bool paredFrente = sensor1.paredCerca();
		bool paredIzquierda = sensor2.paredCerca();
		bool paredDerecha = sensor3.paredCerca();
	
		if (paredFrente)
		{
			Serial.println("pared enfrente, buscando camino...");
			frenarCentellita();
			delay(200);
	
			if (!paredIzquierda)
			{
			Serial.println("Izquierda libre, girando a la izquierda");
			girarIzquierdaCentellita();
			delay(400);
			registrarPaso(GiroIzq);
			}
		
			else if (!paredDerecha)
			{
			Serial.println("Derecha libre, girando a la derecha");
			girarDerechaCentellita();
			delay(400);
			registrarPaso(GiroDer);
			}
	
			else 
			{
			Serial.println("camino sin salida, retrocediendo...");
			retrocederCentellita();
			delay(600);
			registrarPaso(RETROCEDER);
			}
	}
	
	else
	{
		if (paredIzquierda && !paredDercha)
		{
			Serial.println("pared izquierda, girando a la derecha");
			girarDerechaCentellita();
			delay(100);
		}
		
		else if (paredDerecha && !paredIzquierda)
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
	{
		Serial.println("Meta Alcanzada! Preparando Bonus");
		frenarCentellita();
		delay(2000);
		estadoActual = RegresarInicio;
		break;
	}

	case RegresarInicio:
	{		
		Serial.println("Iniciando recorrido de regreso");

		//Bucle invertido para leer las decisiones tomadas
		for (int i = totalPasos - 1; i>=0; i--){
			switch (historialPasos[i])
			{
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
		}
	
	frenarCentellita();
	delay(200);
	estadoActual = Fin;
	break;
}

	case Fin:
	{
		frenarCentellita();
		Serial.println("Se ha completado el bonus");
		while (true)
		{
		//bucle infinito para terminarlo
			delay(1000);
		}
		break;
	}
}
}
