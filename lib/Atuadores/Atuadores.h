#pragma once

#include "Arduino.h"

#define ON 1
#define OFF 0

class DIO {
  private:
    int _pin;
    String _host = "Atuador";
  public:

    DIO(int pin);
    void setHost(String host);
    void on();
    void off();
    void toggle();
    void print();
    bool state();

};
