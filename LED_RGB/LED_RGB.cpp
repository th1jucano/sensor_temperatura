#include "LED_RGB.h"
#include <Arduino.h>


LED_RGB::LED_RGB(int pinR, int pinG, int pinB){

    this->pinoR = pinR;
    this->pinoG = pinG;
    this->pinoB = pinB;

    pinMode(pinoR, OUTPUT);
    pinMode(pinoG, OUTPUT);
    pinMode(pinoB, OUTPUT);

}

void LED_RGB::red(int intensidade){

    analogWrite(pinoR, intensidade);
    analogWrite(pinoG, 0);
    analogWrite(pinoB, 0);

}

void LED_RGB::green(int intensidade){

    analogWrite(pinoR, 0);
    analogWrite(pinoG, intensidade);
    analogWrite(pinoB, 0);

}

void LED_RGB::blue(int intensidade){

    analogWrite(pinoR, 0);
    analogWrite(pinoG, 0);
    analogWrite(pinoB, intensidade);

}

void LED_RGB::off(){

    analogWrite(pinoR, 0);
    analogWrite(pinoG, 0);
    analogWrite(pinoB, 0);

}
