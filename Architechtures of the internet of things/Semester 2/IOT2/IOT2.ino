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

}
