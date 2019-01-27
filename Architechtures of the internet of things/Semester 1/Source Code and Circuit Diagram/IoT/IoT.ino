//include libraries
#include <LiquidCrystal.h>
#include <SPI.h>
#include <Ethernet2.h>
#include <PubSubClient.h>

//mac address
byte mac[] = {0x90, 0xA2, 0xDA, 0x11, 0x3B, 0xAD};

//ethernet details
IPAddress ip(192,168,0,11);
IPAddress gateway(10,108,192,1);
IPAddress subnet(255, 255, 255, 0);

//mqtt declarations
const char userID[] = "matthew_d_king";
const char apikey[] = "fb2cd923a9464c90b6b7647fa04fabf1";
const char server[] = "io.adafruit.com";
const long port = 1883;

EthernetClient ethernet_client;
PubSubClient  client(ethernet_client);

LiquidCrystal lcd(12,11,5,4,3,2);

long previous_time = 0;
long connection_interval = 10000;

int lights_pin = 7;
int heat_pin = 6;

const int lcd_timer = 3000;
int lcd_previous_time = 0;
int lcd_current_time = millis();

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("starting mqtt client on arduino...");

  lcd.begin(16,2);
  lcd.clear();
  lcd.print("Starting System");
  
  //pin setup  
  pinMode(A0, INPUT);
  pinMode(lights_pin, OUTPUT);

  digitalWrite(lights_pin, LOW);
  digitalWrite(heat_pin, LOW);
  
  client.setServer(server, port);
  client.setCallback(callback);

  if (Ethernet.begin(mac) == 0)
  {
    Serial.println("failed to configure ethernet using DHCP");
    Ethernet.begin(mac, ip);
  }

  delay(1500);
  lcd.clear();
  Serial.print("mqtt client is at: ");
  Serial.println(Ethernet.localIP());
}

void loop() {
  // put your main code here, to run repeatedly:
  if(!client.connected())
  {
    reconnect();
  }
  else
  {
    client.loop();

    unsigned long current_time = millis();
    if (current_time - previous_time > connection_interval)
    {
      previous_time = current_time;
      send_message();
      send_temp();
      send_light();
    }
  }
  auto_lights();
  auto_heating();
  if((lcd_current_time - lcd_previous_time) > lcd_timer)
  {
    int current_light = analogRead(A0);
    int current_temp_sensor = analogRead(A1);
    float current_temp = ((current_temp_sensor*5)/1023.0)*100.0;
    lcd.setCursor(0,0);
    lcd.print("Light: ");
    lcd.print(current_light);
    lcd.setCursor(0,1);
    lcd.print("Temp: ");
    lcd.print(current_temp);
    lcd_previous_time = lcd_current_time;
  }
}

void callback(char* topic, byte* payload, unsigned int length)
{
  Serial.print("message arrived [");
  Serial.print(topic);
  Serial.print("] ");

  for (int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void reconnect()
{
  while (!client.connected())
  {
    Serial.println("attempting mqtt connection...");

    if (client.connect("my_arduino", userID, apikey))
    {
      Serial.println("...connected");

      client.publish("matthew_d_king/f/status­messages","we are alive!");
      client.subscribe("matthew_d_king/f/#");
    }
    else
    {
      Serial.print("connection failed, rc = ");
      Serial.print(client.state());
      Serial.println();
      Serial.println("we will try again in 5 seconds");

      delay(5000);
    }
  }
}

//custom header function that sends the current uptime of the device
void send_message()
{
  char time_str[40];
  sprintf(time_str, "up for %lu s", (millis() / 1000));
  client.publish("matthew_d_king/f/status-messages", time_str);
}

//custom function that sends the light value to the
//adafruit.io dashboard with the light guage feed on it
void send_light()
{
  if(client.connected())
  {
    int light = analogRead(A0);
    char light_level[5];
    sprintf(light_level, "%i", light);
    client.publish("matthew_d_king/f/ldr-guage", light_level);
  }
}

//custom function that sends the temperature value to the
//adafruit.io dashboard with the temperature graph feed on it
void send_temp()
{
  if(client.connected())
  {
    int temp = analogRead(A1);
    float tempCel = temp*5;
    tempCel = tempCel / 1023;
    tempCel = tempCel * 100;
    Serial.println(tempCel);
    char temp_level[10];
    dtostrf(tempCel, 3, 2, temp_level);
    Serial.println(temp_level);
    client.publish("matthew_d_king/f/temp-sense", temp_level);
  }
}

//custom fuction for turning on the heating
//if the temperature level is below a certain value the heating comes on
//else the heating remains off
void auto_heating()
{
  int current_temp_sensor = analogRead(A1);
  float current_temp = ((current_temp_sensor*5)/1023.0)*100.0;
  if(current_temp < 18)
  {
    //turn on the heating on
    digitalWrite(heat_pin, HIGH);
  }
  else
  {
    //turn the lights heating off
    digitalWrite(heat_pin, LOW);
  }
}

//custom fuction for turning on the lights
//if the light level is below a certain value the lights come on
//else the lights remain off
void auto_lights()
{
  int current_light_level = analogRead(A0);
  if(current_light_level < 100)
  {
    //turn on the lights
    digitalWrite(lights_pin, HIGH);
  }
  else
  {
    //turn the lights off
    digitalWrite(lights_pin, LOW);
  }
}

//custom function for writing information to the lcd
void LCD_print_int(int cursor_Pos, int line_Pos, float text_Output)
{
  //screen clearing
  lcd.setCursor(cursor_Pos, line_Pos);
  lcd.print("                ");
  
  //set new content
  lcd.setCursor(cursor_Pos, line_Pos);
  lcd.print(text_Output);
}
//custom function for writing information to the lcd
void LCD_print_String(int cursor_Pos, int line_Pos, String text_Output)
{
  //screen clearing
  lcd.setCursor(cursor_Pos, line_Pos);
  lcd.print("                ");
  
  //set new content
  lcd.setCursor(cursor_Pos, line_Pos);
  lcd.print(text_Output);
}

