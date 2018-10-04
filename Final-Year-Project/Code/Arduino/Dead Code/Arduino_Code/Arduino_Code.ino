//setting up the required variables
int tempVal = 0;
const int tempPin = A0;

void setup() {
  //setting up serial for output
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  //reading the analog pin
  //sending data
  tempVal = analogRead(tempPin);
  sendValue();
}

//custom function for sending the raw data to the RPI
void sendValue(int Value)
{
  //mapping the data to a siutable range
  //Convert to deg c within the ds18b20's range
  //printing the value to the serial connection
  //waiting for the adc to settle for next reading
  
  Value = map(Value, 0, 1023, -55, 125);
  Serial.println(Value);
  delay(2);
}

