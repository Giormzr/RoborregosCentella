#include "sensorcolor.h"

//Inicializa la librería de Adafruit y guarda los pines
SensorColor::SensorColor(int pinRojo, int pinVerde, int pinAzul)
  : tcs(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_1X) {
  pinR = pinRojo;
  pinG = pinVerde;
  pinB = pinAzul;
}

// Inicializar el sensor
bool SensorColor::iniciar() {
  pinMode(pinR, OUTPUT);
  pinMode(pinG, OUTPUT);
  pinMode(pinB, OUTPUT);

  if (tcs.begin()) {
    Serial.println("Inicializando");
    delay(2000);
    return true;
  } else {
    Serial.println("Error");
    Serial.println("Debe de revisar las conexiones e iniciar nuevamente");
    return false;
  }
}

//Leer y procesar color
void SensorColor::procesarColor() {
  uint16_t R, G, B, C;
  tcs.getRawData(&R, &G, &B, &C);

  if (R > G && R > B) {
    R += 30;
    if (G >= 20) G -= 20; else G = 0;
  }

  if (G > R && G > B) {
    if (R >= 20) R -= 20; else R = 0;
    if (G >= 20) G -= 20; else G = 0;
    B += 30;
  }

  //Límites
  if (R > 255) { R = 255; }
  if (G > 255) { G = 255; }
  if (B > 255) { B = 255; }

  //Salida analógica a los pines
  analogWrite(pinR, R);
  analogWrite(pinG, G);
  analogWrite(pinB, B);

  //Se guardan las lecturas para la meta
  redVal = R; 
  greenVal = G; 
  blueVal = B;

  Serial.println(" R = " + String(R) + " G = " + String(G) + " B = " + String(B));
}

//Para evaluar la meta 
bool SensorColor::Rojo(){
    return(redVal > 130 && greenVal < 80 && blueVal < 80);
}

bool SensorColor::Turquesa(){
	return(greenVal > 100 && blueVal > 100 && redVal < 100 && abs((int)greenVal - (int)blueVal) < 45);
}

bool SensorColor::Amarillo(){
	return(redVal > 120 && greenVal > 120 && blueVal < 100 && redVal > blueVal);
}

bool SensorColor::Naranja(){
	return(redVal > 120 && greenVal > 60 && greenVal < 160 && blueVal < 80 && redVal > greenVal);
}

bool SensorColor::Rosa(){
	return(redVal > 100 && blueVal > 100 && greenVal < 90);
}
