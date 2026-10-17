#include "ultrasonico.h"
#include "motor.h"
#include "sensorcolor.h"

//motores A DERECHO
const int IN1 = 16;
const int IN2 = 17;
const int ENA = 22;

//motores B IZQUIERDO
const int IN3 = 18;
const int IN4 = 19;
const int ENB = 23;

// --- PINES DE SENSORES ULTRASONICOS ---
const int TRIGGER_1 = 32; // Frente
const int ECHO_1 = 33;
const int TRIGGER_2 = 25; // Derecha
const int ECHO_2 = 26;
const int TRIGGER_3 = 27; // Izquierda
const int ECHO_3 = 14;

// --- PINES DE SENSOR DE COLOR ---
const int PINR = 13;
const int PING = 15;
const int PINB = 2;

// --- CREACIÓN DE OBJETOS ---
Motor motorA(IN1, IN2, 0, false); // Derecho
Motor motorB(IN3, IN4, 0, false); // Izquierdo

SensorUltrasonico sensorFrente(TRIGGER_1, ECHO_1);
SensorUltrasonico sensorDer(TRIGGER_2, ECHO_2);
SensorUltrasonico sensorIzq(TRIGGER_3, ECHO_3);
SensorColor sensorColor(PINR, PING, PINB);

// --- ESTADOS Y HISTORIAL DEL ROBOT ---
enum EstadoRobot { 
    Explorar, 
    LlegadaMeta, 
    RegresarInicio, 
    Fin 
};

EstadoRobot estadoActual = Explorar;

enum Movimiento { AVANZAR, GiroIzq, GiroDer, RETROCEDER }; 
Movimiento historialPasos[150]; 
int totalPasos = 0; 

void registrarPaso(Movimiento paso){
    if (totalPasos < 150){
        historialPasos[totalPasos] = paso;
        totalPasos++;
    }
}

// --- MOVIMIENTOS ---
void avanzarCentellita() {
    motorA.avanzar();
    motorB.avanzar();
}
  
void frenarCentellita() {
    motorA.frenar();
    motorB.frenar();
}
  
void retrocederCentellita() {
    motorA.retroceder();
    motorB.retroceder();
}
  
void girarDerechaCentellita() {
    motorA.retroceder();
    motorB.avanzar();
}
  
void girarIzquierdaCentellita() {
    motorA.avanzar();
    motorB.retroceder();
}

// --- SETUP ---
void setup() {
    Serial.begin(115200);
    delay(1000);
    
	pinMode(ENA, OUTPUT);
    digitalWrite(ENA, HIGH); // Habilita lado derecho (Pin 22)

    pinMode(ENB, OUTPUT);
    digitalWrite(ENB, HIGH); // Habilita lado izquierdo (Pin 23)

    motorA.inicializar();
    motorB.inicializar();
    
    sensorFrente.inicializar();
    sensorIzq.inicializar();
    sensorDer.inicializar();
    sensorColor.iniciar();
    
    Serial.println("Centellita listo y definitivo :p");
    delay(2000);
}

// --- LOOP ---
void loop() {
    switch (estadoActual) {
        case Explorar: {
            sensorColor.procesarColor();

            if (sensorColor.Rojo()) {
                frenarCentellita();
                estadoActual = LlegadaMeta;
                break;
            }
        
            bool paredFrente = sensorFrente.paredCerca();
            bool paredIzquierda = sensorIzq.paredCerca();
            bool paredDerecha = sensorDer.paredCerca();
        
            if (paredFrente) {
                frenarCentellita();
                delay(200);
            
                if (!paredIzquierda) {
                    girarIzquierdaCentellita();
                    delay(400);
                    registrarPaso(GiroIzq);
                } else if (!paredDerecha) {
                    girarDerechaCentellita();
                    delay(400);
                    registrarPaso(GiroDer);
                } else {
                    retrocederCentellita();
                    delay(600);
                    registrarPaso(RETROCEDER);
                }
            } else {
                if (paredIzquierda && !paredDerecha) {
                    girarDerechaCentellita();
                    delay(100);
                } else if (paredDerecha && !paredIzquierda) {
                    girarIzquierdaCentellita();  
                    delay(100);
                } else {
                    avanzarCentellita();
                    registrarPaso(AVANZAR);
                }
            }
            break;
        }

        case LlegadaMeta: {
            frenarCentellita();
            delay(2000);
            estadoActual = RegresarInicio;
            break;
        }

        case RegresarInicio: { 
            for (int i = totalPasos - 1; i >= 0; i--) {
                switch (historialPasos[i]) {
                    case GiroIzq:
                        girarDerechaCentellita();
                        delay(400);
                        break;
                    case GiroDer:
                        girarIzquierdaCentellita();
                        delay(400);
                        break;
                    case AVANZAR:
                    case RETROCEDER:
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

        case Fin: {
            frenarCentellita();
            while (true) {
                delay(1000);
            }
            break;
        }
    }
}