#include "motor.h"

Motor::Motor(int _in1, int _in2, int _offset, bool _invertido) {
    pinIN1 = _in1;
    pinIN2 = _in2;
    offsetVelocidad = _offset;
    invertido = _invertido;
}

void Motor::inicializar() {
    pinMode(pinIN1, OUTPUT);
    pinMode(pinIN2, OUTPUT);
    frenar();
}

void aplicarPulsos(int pin1, int pin2) {
    digitalWrite(pin1, HIGH);
    digitalWrite(pin2, LOW);
    delay(8);  // Tiempo de impulso (auméntalo si quieres más velocidad)
    
    digitalWrite(pin1, LOW);
    digitalWrite(pin2, LOW);
    delay(2);  // Tiempo de pausa de control
}

void Motor::avanzar() {
    if (!invertido) {
        aplicarPulsos(pinIN2, pinIN1);
    } else {
        aplicarPulsos(pinIN1, pinIN2);
    }
}

void Motor::retroceder() {
    if (!invertido) {
        aplicarPulsos(pinIN1, pinIN2);
    } else {
        aplicarPulsos(pinIN2, pinIN1);
    }
}

void Motor::frenar() {
    digitalWrite(pinIN1, LOW);
    digitalWrite(pinIN2, LOW);
}