// Include the libraries we need
#include <OneWire.h>
#include <DallasTemperature.h>
#include <SimpleTimer.h>
#include <Atuadores.h>

// Data wire is plugged into port 2 on the Arduino
#define ONE_WIRE_BUS 14

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);

DIO luz(12);
DIO heater(6);
DIO sirene(5);
SimpleTimer timer;

float _gradient;
unsigned int _stepTime;
float _currentTemperature;
float _desiredTemperature;
float _initialTemperature;
float _endTemperature;
int _count;
bool activeSirene = false;

void iniciarBrassagem(unsigned int stepTime, float temperature);
void callback_temperature();
void updateDesiredTemperature ();
void callback_sirene();
/*
 * The setup function. We only start the sensors here
 */
void setup()
{
  // start serial port
  Serial.begin(9600);
  Serial.println("Dallas Temperature IC Control Library Demo");

  // Start up the library
  sensors.begin();
  timer.setInterval(5000, callback_temperature);

  luz.on();
  luz.print();
  delay(3000);
  luz.off();
  luz.print();
  delay(3000);
  if (!luz.state()) {
    luz.toggle();
    delay(300);
    luz.toggle();
    delay(300);
    luz.toggle();
    delay(300);
    luz.toggle();
    delay(300);
    luz.toggle();
    delay(300);
    luz.toggle();
    delay(3000);
  }
  luz.toggle();
  luz.print();
  delay(3000);
  luz.toggle();
  luz.print();
  delay(3000);
  if (luz.state()) {
    luz.toggle();
    delay(300);
    luz.toggle();
    delay(300);
    luz.toggle();
    delay(300);
  }
}

/*
 * Main function, get and show the temperature
 */
void loop()
{
  timer.run();

  if (_currentTemperature < _desiredTemperature) {
    heater.on();
  } else {
    heater.off();
  }
  if (activeSirene == true) {
    timer.setTimer(2000, callback_sirene, 2);
    activeSirene = false;
  }
}

void callback_sirene () {
  sirene.toggle();
}

void iniciarBrassagem(unsigned int stepTime, float temperature) {
  _stepTime = stepTime;
  _initialTemperature = sensors.getTempCByIndex(0);
  _endTemperature = temperature;
  _gradient = (_endTemperature - _initialTemperature) / _stepTime;

  _count = 0;
  timer.setTimer(60000,updateDesiredTemperature,_stepTime);


}

void updateDesiredTemperature () {
  if (_count < _stepTime) {
  _count++;
  _desiredTemperature = _desiredTemperature + _count * _gradient;
  if (_count == _stepTime) {
    activeSirene = true;
  }
  }
}

void reset() {
  _gradient = 0;
  _stepTime = 0;
  _initialTemperature = 0;
  _endTemperature = 0;
  _currentTemperature = 0;
  _desiredTemperature = 0;
  _count = 0;
}

void callback_temperature() {
  // call sensors.requestTemperatures() to issue a global temperature
  // request to all devices on the bus
  Serial.print("Requesting temperatures...");
  sensors.requestTemperatures(); // Send the command to get temperatures
  Serial.println("DONE");
  // After we got the temperatures, we can print them here.
  // We use the function ByIndex, and as an example get the temperature from the first sensor only.
  Serial.print("Temperature for the device 1 (index 0) is: ");
  Serial.println(sensors.getTempCByIndex(0));
}
