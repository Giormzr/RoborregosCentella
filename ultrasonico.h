#ifndef ULTRASONICO_H
#define ULTRASONICO_H

class SensorUltrasonico 
{
private:
	int pinTrigger; //envía la señal; salida
	int pinEcho; //recibe la señal; cuando la recibe indica la distancia (tiempo que tarda en regresar la onda); entrada
	
public:
	SensorUltrasonico(int _pT, int _pE);

	void inicializar();
    float medirDistancia();

  //aplicado en Centellita para que detecte la pared:
  bool paredCerca();
};

#endif
