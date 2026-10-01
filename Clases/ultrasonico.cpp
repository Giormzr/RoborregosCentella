#include "ultrasonico.h"

//Guarda los pines
SensorUltrasonico::SensorUltrasonico(int _pT, int _pE) {
  pinTrigger = _pT;
  pinEcho = _pE;
}

//Configura las entradas y salidas
void SensorUltrasonico::inicializar() {
  pinMode(pinTrigger, OUTPUT);
  pinMode(pinEcho, INPUT);
}

//Lanza el pulso y mide la distancia en cm
float SensorUltrasonico::medirDistancia() {
  digitalWrite(pinTrigger, LOW); //Verificamos que la salida esté apagada
  delayMicroseconds(2);

  digitalWrite(pinTrigger, HIGH); //Encendemos la salida
  delayMicroseconds(10);
  digitalWrite(pinTrigger, LOW);  //Lanza la onda y apagamos la salida

  long tiempomedido = pulseIn(pinEcho, HIGH, 1000); //Evaluar timeout conforme a la velocidad de Centellita

  if (tiempomedido == 0) return -1.0;

  return (tiempomedido * 0.0343) / 2.0;
}

//Para que Centellita detecte la pared

bool SensorUltrasonico::paredcerca() {
  float distancia = medirDistancia();

  if (distancia == -1.0) return false; //No detecta pared (cuando la onda no regresa en el timeout dado)

  if (distancia <= 5.0) {
    return true; //Aplica si detecta la pared 5cm o menos
  }

  return false; //Aplica si la distancia medida es mayor a 5cm; se pone para que Centellita no se confunda si se da el caso
}

