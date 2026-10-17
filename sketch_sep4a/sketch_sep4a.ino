#include <Arduino.h>

#include "ultrasonico.h"
#include "motor.h"
#include "sensorcolor.h"

// #1 PWMA 23, AIN2 22, AIN1 21; PWMB 19, BIN2 18, BIN1 5; STBY 12
// #2 PWMA 13, AIN2 15, AIN1 2; PWMB 17, BIN2 16, BIN1 4; STBY 12
//motor A
const int IN1 = 21;
const int IN2 = 22;
const int ENA = 23;

//motor B
const int IN3 = 5;
const int IN4 = 18;
const int ENB = 19;

//motor C
const int IN5 = 30;
const int IN6 = 31;
const int ENC = 32;

//motor D
const int IN7 = 33;
const int IN8 = 34;
const int END = 35;

const int PIN_STBY = 12;

//sensor ultrasonico 1: FRENTE
const int TRIGGER_1 = 36;
const int ECHO_1 = 37;

//sensor ultrasonico 2: IZQUIERDA (el loop lo usa como izquierda)
const int TRIGGER_2 = 38;
const int ECHO_2 = 39;

//sensor ultrasonico 3: DERECHA (el loop lo usa como derecha)
const int TRIGGER_3 = 40;
const int ECHO_3 = 41;

//sensor de color (LED RGB)
const int PINR = 42;
const int PING = 43;
const int PINB = 44;

//creamos aquí los objetos
Motor motorA(IN1, IN2, ENA); //Frontal Izquierdo
Motor motorB(IN3, IN4, ENB); //Trasero Izquierdo
Motor motorC(IN5, IN6, ENC); //Frontal Derecho
Motor motorD(IN7, IN8, END); //Trasero Derecho

SensorUltrasonico sensor1(TRIGGER_1, ECHO_1);
SensorUltrasonico sensor2(TRIGGER_2, ECHO_2);
SensorUltrasonico sensor3(TRIGGER_3, ECHO_3);

SensorColor sensorColor(PINR, PING, PINB);
bool sensorColorOK = false;

//Ajustes para calibrar en la pista
const int TIEMPO_GIRO_90  = 400;  // ms
const int TIEMPO_GIRO_180 = 800;  // ms
const int TIEMPO_PASO     = 400;  // ms de avance por cada paso registrado

enum EstadoRobot {
  Explorar,       //Paredes y ultrasonicos
  LlegadaMeta,    //Se detiene cuando detecta el rojo de la meta
  RegresarInicio, //Se regresa para el bonus
  Fin             //Se apaga
};

EstadoRobot estadoActual = Explorar;

enum Movimiento { AVANZAR, GiroIzq, GiroDer, RETROCEDER };
Movimiento historialPasos[150]; //Guarda hasta 150 pasos
int totalPasos = 0;
unsigned long ultimoAvance = 0;

//Para guardar el recorrido
void registrarPaso(Movimiento paso) {
  if (totalPasos < 150) {
    historialPasos[totalPasos] = paso;
    totalPasos++;
  }
}

void avanzarCentellita() {
  motorA.avanzar();
  motorB.avanzar();
  motorC.avanzar();
  motorD.avanzar();
}

void frenarCentellita() {
  motorA.frenar();
  motorB.frenar();
  motorC.frenar();
  motorD.frenar();
}

void retrocederCentellita() {
  motorA.retroceder();
  motorB.retroceder();
  motorC.retroceder();
  motorD.retroceder();
}

void girarDerechaCentellita() {
  motorA.avanzar();
  motorB.avanzar();
  motorC.retroceder();
  motorD.retroceder();
}

void girarIzquierdaCentellita() {
  motorA.retroceder();
  motorB.retroceder();
  motorC.avanzar();
  motorD.avanzar();
}

void setup() {
  Serial.begin(115200);

  pinMode(PIN_STBY, OUTPUT);
  digitalWrite(PIN_STBY, HIGH); //habilita motores

  motorA.inicializar();
  motorB.inicializar();
  motorC.inicializar();
  motorD.inicializar();

  sensor1.inicializar();
  sensor2.inicializar();
  sensor3.inicializar();

  sensorColorOK = sensorColor.iniciar();
  if (!sensorColorOK) {
    Serial.println("Sensor de color NO DETECTADO, sigo sin el");
  }

  Serial.println("Centellita listo :p");
  delay(2000);
}

void loop() {
  switch (estadoActual) {

    case Explorar: {
      //Color (solo si el sensor responde)
      if (sensorColorOK) {
        sensorColor.procesarColor();

        //Para el rojo de la meta (prioridad)
        if (sensorColor.Rojo()) {
          frenarCentellita();
          estadoActual = LlegadaMeta;
          break;
        }
        else if (sensorColor.Turquesa()) Serial.println("cuadro TURQUESA detectado");
        else if (sensorColor.Amarillo()) Serial.println("cuadro AMARILLO detectado");
        else if (sensorColor.Naranja())  Serial.println("cuadro NARANJA detectado");
        else if (sensorColor.Rosa())     Serial.println("cuadro ROSA detectado");
      }

      bool paredFrente    = sensor1.paredCerca();
      bool paredIzquierda = sensor2.paredCerca();
      bool paredDerecha   = sensor3.paredCerca();

      if (paredFrente) {
        Serial.println("pared enfrente, buscando camino...");
        frenarCentellita();
        delay(200);

        if (!paredIzquierda) {
          Serial.println("Izquierda libre, girando a la izquierda");
          girarIzquierdaCentellita();
          delay(TIEMPO_GIRO_90);
          registrarPaso(GiroIzq);
        }
        else if (!paredDerecha) {
          Serial.println("Derecha libre, girando a la derecha");
          girarDerechaCentellita();
          delay(TIEMPO_GIRO_90);
          registrarPaso(GiroDer);
        }
        else {
          Serial.println("camino sin salida, retrocediendo...");
          retrocederCentellita();
          delay(600);
          registrarPaso(RETROCEDER);
        }
      }
      else {
        if (paredIzquierda && !paredDerecha) {
          Serial.println("pared izquierda, girando a la derecha");
          girarDerechaCentellita();
          delay(100);
        }
        else if (paredDerecha && !paredIzquierda) {
          Serial.println("pared derecha, girando a la izquierda");
          girarIzquierdaCentellita();
          delay(100);
        }
        else {
          Serial.println("camino libre, avanzando recto");
          avanzarCentellita();
          //Un paso registrado por cada TIEMPO_PASO ms de avance
          if (millis() - ultimoAvance >= TIEMPO_PASO) {
            registrarPaso(AVANZAR);
            ultimoAvance = millis();
          }
        }
      }
      break;
    }

    case LlegadaMeta: {
      Serial.println("Meta Alcanzada! Preparando Bonus");
      frenarCentellita();
      delay(2000);
      estadoActual = RegresarInicio;
      break;
    }

    case RegresarInicio: {
      Serial.println("Iniciando recorrido de regreso");

      //Media vuelta para mirar hacia el inicio
      girarDerechaCentellita();
      delay(TIEMPO_GIRO_180);
      frenarCentellita();
      delay(200);

      //Bucle invertido para leer las decisiones tomadas
      for (int i = totalPasos - 1; i >= 0; i--) {
        switch (historialPasos[i]) {
          case GiroIzq:
            //Si giró a la izquierda de ida, gira a la derecha de regreso
            girarDerechaCentellita();
            delay(TIEMPO_GIRO_90);
            break;

          case GiroDer:
            //Si giró a la derecha de ida, gira a la izquierda de regreso
            girarIzquierdaCentellita();
            delay(TIEMPO_GIRO_90);
            break;

          case AVANZAR:
          case RETROCEDER:
            //El avance se queda igual; el retroceso de la ida se convierte en avance
            avanzarCentellita();
            delay(TIEMPO_PASO);
            break;
        }
      }

      frenarCentellita();
      delay(200);
      estadoActual = Fin;
      break;
    }

    case Fin: {
      frenarCentellita();
      Serial.println("Se ha completado el bonus");
      while (true) {
        delay(1000);
      }
      break;
    }
  }
}
