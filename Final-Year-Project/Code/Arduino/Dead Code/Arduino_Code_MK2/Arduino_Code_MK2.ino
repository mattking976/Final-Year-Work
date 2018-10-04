#include<OneWire.h>
#include<DallasTemperature.h>

//setting up constant variables
const int ledPin = 13;
const int tempPin = A3;
const int blinkInterval = 500;
const int readingInterval = 5;

//setting up variables
int ledState = HIGH;
int tempValRaw = 0;
unsigned long currentMillis = 0;
unsigned long previousMillisLED = 0;
unsigned long previousMillisTemp = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  //get the latest value of millis
  //update the led based on the currentMillis and ledState variables
  currentMillis = millis();
  tempValRaw = analogRead(tempPin);
  LifeLED();
  SendData(tempValRaw);
}

void SendData(int tempValRaw)
{
  int tempValMapped = tempValRaw;

  if ((unsigned long) currentMillis - previousMillisTemp >= readingInterval)
  {
    //tempValMapped= map(tempValRaw, 0, 1023, -55, 125);
    Serial.println(tempValMapped);
    previousMillisTemp = readingInterval;
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

