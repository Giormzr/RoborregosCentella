#ifndef MOTOR_H
#define MOTOR_H

#include <Arduino.h>

class Motor {
private:
    int pinIN1;
    int pinIN2;
    int offsetVelocidad;
    bool invertido;

public:
    Motor(int _in1, int _in2, int _offset = 0, bool _invertido = false);

    void inicializar();
    void avanzar();
    void retroceder();
    void frenar();
};

#endif