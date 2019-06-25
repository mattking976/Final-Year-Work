//tempCel = ((temp*5) / 1023.0) * 100.0; formula for lm35 to degC
//variable declarations
//output pinouts
int LED1 = 2;
int LED2 = 3;

//input pinouts
int LDR1 = A0;
int TMP1 = A1;
int TMP2 = A2;

//input volatile data storage variables
float LDR1_val = 0;
float TMP1_val = 0;
float TMP2_val = 0;

//timer variables
int timerInt1 = 1000;
int timerInt5 = 5000;
int timerInt10 = 10000;
int current_time = 0;
int previous_time = 0;

//led current states
int led1nowState = 0;
int led2nowState = 0;

void setup() {
  // put your setup code here, to run once:
  //setting up the serial communications for node red
  Serial.begin(57600);

  //setting up the pins for the IO
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LDR1, INPUT);
  pinMode(TMP1, INPUT);
  pinMode(TMP2, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  //heartbeatLED();
  TMP1_val = analogRead(TMP1);
  TMP2_val = analogRead(TMP2);
  LDR1_val = analogRead(LDR1);

  TMP1_val = ((TMP1_val*5) / 1023.0) * 100.0;
  TMP1_val = (TMP1_val - 32) / 1.8;
  TMP2_val = ((TMP2_val*5) / 1023.0) * 100.0;
  TMP2_val = (TMP2_val - 32) / 1.8;
  Serial.println(TMP1_val);
  Serial.println(TMP2_val);
  Serial.println(LDR1_val);
  delay(3000);
}

void heartbeatLED()
{
  unsigned long current_time = millis();
  if((current_time - previous_time) > timerInt1)
  {
    if(led1nowState == 1)
    {
      digitalWrite(LED1, 0);
      led1nowState = 0;
      previous_time = current_time;
    }
    if (led1nowState == 0)
    {
      digitalWrite(LED1, 1);
      led1nowState = 1;
      previous_time = current_time;
    }
  }
}
