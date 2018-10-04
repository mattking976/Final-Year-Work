//included libraries
//OneWire.h and DallasTemperature.h for getting the data
//from the ds18b20 temperature sensor
#include <OneWire.h>
#include <DallasTemperature.h>

//defining the bus that the components will be connected to
#define ONE_WIRE_BUS A3

//initialising the OneWire and Dallas Temperature classes
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

//setting up constant variables
const int ledPin = 13;
const int blinkInterval = 500;
const int readingInterval = 1000;

//setting up variables
int ledState = HIGH;
 float Celcius=0;
 float Fahrenheit=0;
unsigned long currentMillis = 0;
unsigned long previousMillisLED = 0;
unsigned long previousMillisTemp = 0;

void setup() {
  // put your setup code here, to run once:
  //setting up the serial interface along with the sensors
  //and the needed pin for the onboard LED
  Serial.begin(9600);
  sensors.begin();
  pinMode(ledPin, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  currentMillis = millis();
  LifeLED();
  SendData();
}

void SendData()
{
  if ((unsigned long) currentMillis - previousMillisTemp >= readingInterval)
  {
    sensors.requestTemperatures();
    Celcius = sensors.getTempCByIndex(0);
    Fahrenheit = sensors.toFahrenheit(Celcius);
    Serial.print(Celcius);
    Serial.print(",");
    Serial.println(Fahrenheit);
    previousMillisTemp += readingInterval;
  }
}

void LifeLED()
{
  //checking to see if the led state is low
  //if true we check to see if the blinkInterval has passed
  //if true we set the led state to high and write this change to the pin
  //we then update the previousMillisLED varaible by our blinkInterval
  //this is so that the intervals are equal 
  //as the line could also be previousMillisLed = currentMills
  //however this can be less accurate
  if (ledState == LOW)
  {
    if((unsigned long) currentMillis - previousMillisLED >= blinkInterval)
    {
      ledState = HIGH;
      digitalWrite(ledPin, ledState);
      previousMillisLED += blinkInterval;
    }
  }
  //if the led is already high we check to see that the blinkInterval has passed
  //if true we set the ledState to low and write this change to the pin
  //and update the previousMillisLED by the blinkInterval
  //this is done the same way as above for the same reason
  else
  {
    if((unsigned long) currentMillis - previousMillisLED >= blinkInterval)
    {
      ledState = LOW;
      digitalWrite(ledPin,ledState);
      previousMillisLED += blinkInterval;
    }
  }
}
