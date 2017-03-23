// Include the libraries we need
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Atuadores.h>
#include <SimpleTimer.h>

// Data wire is plugged into port 2 on the Arduino
#define ONE_WIRE_BUS 14

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);

DIO heater(12);
DIO siren(6);
SimpleTimer timer;
/*
 * The setup function. We only start the sensors here
 */
float desiredTemperature=0;
float histerese = 1;
int count = 0;
int step = 0;
float _stepTemperature = 0;
float initialTemperature = 0;
float currentTemperature ();
void brassagem();
void heaterTemperature(float desiredTemperature);
float valueHisterese();
void stepTemperature();
void sireneOFF();

bool modeBrassagem = false;
bool modeFervura = false;
bool activeHisterese = false;

void setup(void)
{
  // start serial port
  Serial.begin(9600);

  // Start up the library
  sensors.begin();
}

/*
 * Main function, get and show the temperature
 */
void loop(void)
{
  //
}

void brassagem()
{
  timer.setTimer(60000, stepTemperature, step);
  heaterTemperature(desiredTemperature);
}

void iniciarStep () {
  timer.setTimer(60000, stepTemperature, step);
  count = 1;
  _stepTemperature = (desiredTemperature - initialTemperature) / step;
  desiredTemperature = desiredTemperature + (count * _stepTemperature);
  heaterTemperature(desiredTemperature);

}


void stepTemperature ()
{
  count++;
  desiredTemperature = desiredTemperature + (count * _stepTemperature);
  if (step == count - 1) {
    timer.setTimeout(3000, sireneOFF);
    siren.on();
  }
}

void sireneOFF () {
  siren.off();
}
void heaterTemperature(float _desiredTemperature)
{
  if (currentTemperature() + valueHisterese() < _desiredTemperature) { //adicionar histerese
    heater.on();
    activeHisterese = false;
  } else {
    heater.off();
    activeHisterese = true;
  }
}

float valueHisterese ()
{
  if (activeHisterese) {
    return histerese;
  } else {
    return 0;
  }
}

float currentTemperature ()
{
  sensors.requestTemperatures(); // Send the command to get temperatures
  Serial.print("Temperature for the device 1 (index 0) is: ");
  Serial.println(sensors.getTempCByIndex(0));
  return sensors.getTempCByIndex(0);
}
