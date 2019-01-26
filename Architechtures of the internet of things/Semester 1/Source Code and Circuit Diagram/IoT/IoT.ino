//include libraries
#include <LiquidCrystal.h>
#include <SPI.h>
#include <Ethernet2.h>
#include <PubSubClient.h>

LiquidCrystal lcd(12,11,5,4,3,2);

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

long previous_time = 0;
long connection_interval = 10000;

int lights_pin = 7;
int heat_pin = 6;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("starting mqtt client on arduino...");

  lcd.begin(16,2);
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

void send_message()
{
  char time_str[40];
  sprintf(time_str, "up for %lu s", (millis() / 1000));
  client.publish("matthew_d_king/f/status-messages", time_str);
}

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

void send_temp()
{
  if(client.connected())
  {
    int temp = analogRead(A0);
    temp = (5*temp*100)/1024;
    char temp_level[5];
    sprintf(temp_level, "%i", temp);
    client.publish("matthew_d_king/f/temp-sense", temp_level);
  }
}

void auto_heating()
{
  float current_heat_level = analogRead(A1);
  current_heat_level = (3.3*current_heat_level*100)/1024;
  if(current_heat_level < 18)
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

