#include "Atuadores.h"

DIO::DIO(int pin) {
  this->_pin = pin;
  pinMode(this->_pin, OUTPUT);
}

void DIO::on() {
  digitalWrite(this->_pin, ON);
}

void DIO::off() {
  digitalWrite(this->_pin, OFF);
}

void DIO::toggle() {
  digitalWrite(this->_pin, !digitalRead(this->_pin));
}

void DIO::print() {
  String state = digitalRead(this->_pin) == ON ? "ON." : "OFF.";
  Serial.println(this->_host + " está " + state);
}

bool DIO::state() {
  if (digitalRead(this->_pin) == ON) {
    return true;
  }
  if (digitalRead(this->_pin) == OFF) {
    return false;
  }
}
 void DIO::setHost(String host) {
   this->_host = host;
 }
