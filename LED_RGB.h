#ifndef LED_RGB__H_INCLUDED
#define LED_RGB__H_INCLUDED

#include <Arduino.h>

class LED_RGB{
public:

    LED_RGB(int pinR, int pinG, int pinB);

    void red(int intensidade);
    void green(int intensidade);
    void blue(int intensidade);
    void off();

private:
    int pinoR;
    int pinoG;
    int pinoB;


};


#endif // LED_RGB__H_INCLUDED
