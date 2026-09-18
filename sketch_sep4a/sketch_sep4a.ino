int salida = 3; //activa el motor

// sensor ultrasonico frontal
int trigPin = 2; 
int echoPin = 4;

// sensor izq
int trigPinIzq = 7;
int trigPinIzq = 8;

// sensor der
int trigPinDer = 12;
int trigPinDer = 13;

void setup() {
  // sensor frontal
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // sensor izq
  pinMode(trigPinIzq, OUTPUT);
  pinMode(echoPinIzq, INPUT);

  // sensor der
  pinMode(trigPinDer, OUTPUT);
  pinMode(echoPinDer, INPUT);

}

void loop() {
  analogWrite(salida,127); //señal PWM a 50%
  delay(2000);
  analogWrite(salida,0); //señal PWM a 0%

}
