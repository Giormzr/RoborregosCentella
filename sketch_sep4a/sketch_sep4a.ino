CPP

#include <Arduino.h>

class SensorUltrasonico 
{
  private:
    int pinTrigger;
    int pinEcho;

  public:
    SensorUltrasonico(int _pT, int _pE)
    {
      pinTrigger = _pT;
      pinEcho = _pE;

      pinMode(_pT, OUTPUT);
      pinMode(_pE, INPUT);
    }

    float medirDistancia()
    {
      digitalWrite(_pT, LOW);
      delayMicroseconds (2);

      digitalWrite(_pT, HIGH);
      delayMicroseconds (10);
      digitalWrite(_pT, LOW)

      long tiempomedido = pulseIn(_pE, HIGH, 30000) //30000 es el timeout

      if (tiempomedido == 0) return -1,0;

      return (tiempomedido * 0.0343)/2.0;
    }

    //este se usa para detectar obstaculos, en el caso de Centellita, será para las paredes
    bool obstaculo()
    {
      float distancia = medirDistancia();

      if (distancia == -1,0) return false; //indica que no regresó la onda en el tiempo dado, por ende, no detecta pared a la distancia dada por el timeout

      if (distancia <=5.0) //5.0 depende de la distancia a la que se busca detectar el obstáculo, en el caso de Centellita es de 5cm
        {
          return true;
        }

      return false; //si la distancia medida es mayor a 5, está libre para ir a esta zona
    }
};

SensorUltrasonico sensorIzquierda(7,8);
SensorUltrasonico sensorDerecha(12,13);
SensorUltrasonico sensorFrontal(2,4);

                                                 int salida = 3; //activa el motor

void setup() 
{
  Serial.begin(115200);
  Serial.println("sensores listos");
}

void loop() 
{
  bool paredIzquierda = sensorIzquierda.obstaculo();
  bool paredDerecha = sensorDerecha.obstaculo();
  bool paredFrontal = sensorFrontal.obstaculo();

  if (paredIzquierda)
  {
    Serial.println("[Izquierda] Pared cerca")
  }

  if (paredDerecha)
  {
    Serial.println("[Derecha] Pared cerca")
  }

  if (paredFrontal)
  {
    Setial.println("[Frontal] Pared cerca")
  }

  delay (100);
}
