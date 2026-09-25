#include <Arduino.h>

#ifndef ULTRASONICO_H
#define ULTRASONICO_H

class SensorUltrasonico 
{
private:
	int pinTrigger; //envia la senal; salida
	int pinEcho; //recibe la senal; cuando la recibe indica la distancia (tiempo que tarda en regresar la onda); entrada
	
public:
	SensorUltrasonico(int _pT, int _pE)
  {
    pinTrigger = _pT;
    pinEcho = _pE;

    pinMode(_pT, OUTPUT); //salida
    pinMode(_pE, INPUT); //entrada	
  }

	void inicializar ()
	{
		pinMode(pinTrigger, OUTPUT);
		pinMode(pinEcho, INPUT);	
	}

  float medirDistancia() 
  {
    digitalWrite(pinTrigger, LOW); //nos aseguramos de que la salida esta apagada
    delayMicroseconds (2);

    digitalWrite(pinTrigger, HIGH); //encendemos la salida
    delayMicroseconds(10);  
    digitalWrite(pinTrigger, LOW); //lanza la onda y apagamos la salida

    long tiempomedido = pulseIn(pinEcho, HIGH, 1000); //evalaur el timeout conforme a la velocidad de Centellita

    if (tiempomedido == 0) return -1.0;

    return (tiempomedido * 0.0343)/2.0;
  }

  //aplicado en Centellita para que detecte la pared:
  bool paredcerca()
  {
    float distancia = medirDistancia();

    if (distancia == -1.0) return false; //no detecta la pared (cuando la onda no regresa en el timeout dado)

    if (distancia <=5.0)
      {
      return true; //aplica si detecta la pared a 5 o menos cm
      }

    return false; //aplica si la distancia medida en mayor a 5cm; se pone para que Centellita no se confunda si se da el caso
  }
};

#endif


