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
DIO pump(5);
SimpleTimer timer;
/*
 * The setup function. We only start the sensors here
 */
float desiredTemperature=66;
float histerese = 1;
int count = 0;
int step = 9;
float stepTemperature = 1;
float initialTemperature = 66;
float currentTemperature ();
void printCurrentTemperature ();
void gradientTemperature();
void sirenOFF();
void fervuraOFF();
void brassagemOFF();
void mashOutOFF();
void preHeaterOFF();
bool heaterBeer (float temp);
void preHeater();
void brassagem();
void mashOut();
void fervura();
void alert ();

bool modePreHeater = false;
bool modeBrassagem = false;
bool modeFervura = false;
bool modeMashOut = false;

bool activeHisterese = false;
bool timerOn = false;

void setup(void)
{
  // start serial port
  Serial.begin(9600);
  timer.setInterval(5000, printCurrentTemperature);
  // Start up the library
  sensors.begin();
}

/*
 * Main function, get and show the temperature
 */
void loop(void)
{
  timer.run();
  if (modePreHeater) {
    preHeater();
  }

  if (modeBrassagem) {
    brassagem();
  }

  if (modeMashOut) {
    mashOut();
  }

  if (modeFervura) {
    fervura();
  }

}

void preHeater () {
  desiredTemperature = 71;
  if (heaterBeer(desiredTemperature)) {
    preHeaterOFF();
  }
}

void preHeaterOFF () {
  modePreHeater = false;
  siren.on();
  timer.setTimeout(2000, sirenOFF);
}

void brassagem () {
  desiredTemperature = 66;
  if (!timerOn) {
    timer.setTimeout(3600000, brassagemOFF);
    timerOn = true;
  }
  heaterBeer(desiredTemperature);
  pump.on();
}

void brassagemOFF () {
  modeBrassagem = false;
  timerOn = false;
  pump.off();
  siren.on();
  timer.setTimeout(2000, sirenOFF);
}


void mashOut () {
  if (!timerOn) {
    timer.setTimer(60000, gradientTemperature, 9);
    timerOn = true;
    count = 0;
    initialTemperature = 66;
    stepTemperature = 1;
    desiredTemperature = initialTemperature + (count * stepTemperature);
    count = 1;
  }
  heaterBeer(desiredTemperature);
}

void gradientTemperature ()
{
  count++;
  desiredTemperature = initialTemperature + (count * stepTemperature);
  if (count == 10) {
    count = 0;
    mashOutOFF();
  }
}

void mashOutOFF () {
  modeMashOut = false;
  timerOn = true;
  pump.off();
  siren.on();
  timer.setTimeout(2000, sirenOFF);
}

void fervura () {
  desiredTemperature = 100;
  heaterBeer(desiredTemperature);
  if (!timerOn) {
    timer.setTimeout(3300000, alert);
    timer.setTimeout(2400000, alert);
    timer.setTimeout(3600000, fervuraOFF);
    timerOn = true;
  }
  if (currentTemperature() < 70) {
    pump.on();
  } else {
    pump.off();
  }
}

void alert () {
  siren.on();
  timer.setTimeout(2000, sirenOFF);
}

void fervuraOFF () {
  modeFervura = false;
  timerOn = false;
  pump.off();
  siren.on();
  timer.setTimeout(2000, sirenOFF);
}

void sirenOFF ()
{
  siren.off();
}

bool heaterBeer (float temp) {
  if (currentTemperature() + histerese < temp) {
    heater.on();
    activeHisterese = false;
    return false;
  } else {
    heater.off();
    activeHisterese = true;
    return true;
  }
}

float currentTemperature ()
{
  sensors.requestTemperatures(); // Send the command to get temperatures
  //Serial.print("Temperature for the device 1 (index 0) is: ");
  //Serial.println(sensors.getTempCByIndex(0));
  return sensors.getTempCByIndex(0);
}

void printCurrentTemperature ()
{
  sensors.requestTemperatures(); // Send the command to get temperatures
  Serial.print("Temperature for the device 1 (index 0) is: ");
  Serial.println(sensors.getTempCByIndex(0));
}
