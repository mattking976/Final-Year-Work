//setting up the required variables
int tempVal = 0;
const int tempPin = A3;
const int ledPin = 13;
long previousMillis = 0;
long previousMillis2 = 0;
unsigned long currentMillis = millis();
unsigned long currentMillis2 = millis();

void setup() {
  //setting up serial for output
  //setting the led pin to ouput for blinking
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  //reading the analog pin
  //sending data
  tempVal = analogRead(tempPin);
  sendValue(tempVal);
  ShowLife();
}

//custom function for sending the raw data to the RPI
void sendValue(int Value)
{
  //mapping the data to a siutable range
  //Convert to deg c within the ds18b20's range
  //printing the value to the serial connection
  //waiting for the adc to settle for next reading
  long SendInterval = 2;
  if (currentMillis2 - previousMillis2 > SendInterval)
  {
    Value = map(Value, 0, 1023, -55, 125);
    Serial.println(Value);
    previousMillis2 = currentMillis2;
  }
}

void ShowLife()
{
  long blinkInterval = 1000;
  int ledState = LOW;
  if (currentMillis - previousMillis > blinkInterval)
  {
    if(ledState == LOW)
    {
      ledState = HIGH;
    }
    else
    {
      ledState = LOW;
    }
  }
  digitalWrite(ledPin, ledState);
  previousMillis = currentMillis;
}

