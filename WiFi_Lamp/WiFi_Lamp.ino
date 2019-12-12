#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <WiFiUdp.h> 
#include "config.h"

#define RGBLED_R  D0      //RGB LED pin assignment.
#define RGBLED_G  D1
#define RGBLED_B  D2

void callback(char* , byte* , unsigned int);
const char* mqtt_server = "broker.hivemq.com";    //mqtt broker address
WiFiClient espClient;
PubSubClient MQTT_client(mqtt_server, 1883, callback, espClient);

byte ch_buffer[8];  //used by function getCharactor()
char str[] = "NCTU ECE!";
byte mac_byte[6];
String mac = "";

int glo_R=0, glo_G = 0, glo_B=0;
int glo_H=0, glo_S=255, glo_V=255;
uint8_t breath_period = 0;
uint8_t rainbow_period = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(RGBLED_R, OUTPUT);
  pinMode(RGBLED_G, OUTPUT);
  pinMode(RGBLED_B, OUTPUT);
  //Initialize the serial port
  Serial.begin(115200);

  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  WiFi.macAddress(mac_byte);
  mac = String(mac_byte[0], HEX) + ":" + String(mac_byte[1], HEX) + ":" + String(mac_byte[2], HEX) + ":" + String(mac_byte[3], HEX) + ":" + String(mac_byte[4], HEX) + ":" + String(mac_byte[5], HEX);
  Serial.print("MAC: ");
  Serial.println(mac);
}

void loop() {
  if (!MQTT_client.connected()) {
    MQTT_reconnect();
  }
  MQTT_client.loop();
  if(breath_period != 0){
    static bool rising;
    if(rising && glo_V<255)
      glo_V++;
    else if(!rising && glo_V>0)
      glo_V--;
    else
      rising = !rising;
    setHSV(glo_H, glo_S, glo_V);
    delay(breath_period);
  }else if(rainbow_period != 0){
    glo_H++;
    glo_H = glo_H%360;
    setHSV(glo_H, glo_S, glo_V);
    delay(rainbow_period);
  }
  
  analogWrite(RGBLED_R, glo_R);
  analogWrite(RGBLED_G, glo_G);
  analogWrite(RGBLED_B, glo_B);
}
void callback(char* topic, byte* payload, unsigned int _length) {   //called when a subscribed topic is received.
  String s = String(topic), p;
  for (int i = 0; i < _length; i++)
    p += (char)payload[i];
  p += '\0';

  Serial.print("Topic: ");
  Serial.println(topic);
  Serial.print("Payload: ");
  Serial.println(p);

  if (s.equals((String)randomID + (String)channel[RGBLED])) {
    int R, G, B;
    if (p.length() < 7) {
      for (int i = 0; i < 7 - p.length(); i++)
        p += "0";
    }
    if (p.startsWith("#")) {
      R = hexString2int(p.substring(1, 3)) * 4;
      G = hexString2int(p.substring(3, 5)) * 4;
      B = hexString2int(p.substring(5, 7)) * 4;
    }
    setRGBLED(R, G, B);
    glo_H = getH(R, G, B);
  }
  else if (s.equals((String)randomID + (String)channel[BREATH_PERIOD])) {
    breath_period = p.toInt();
  }
  else if (s.equals((String)randomID + (String)channel[RAINBOW_PERIOD])) {
    rainbow_period = p.toInt();
  }
//  else if (s.equals((String)randomID + (String)channel[_BEGIN_LIGHT_TIME])) {
//    int hours, mins;
//    if (p.length() > 5){
//      hours = p.substring(0, 2).toInt();
//      mins = p.substring(3, 5).toInt();
//      hours = (hours > 23) ? (begin_light_hour) : (hours);
//      hours = (hours < 0) ? (begin_light_hour) : (hours);
//      mins = (mins > 59) ? (begin_light_min) : (mins);
//      mins = (mins < 0) ? (begin_light_min) : (mins);
//      begin_light_hour = hours;
//      begin_light_min = mins;
//      Serial.print(begin_light_hour);
//      Serial.print(" : ");
//      Serial.println(begin_light_min);
//    }
//  }
//  else if (s.equals((String)randomID + (String)channel[_END_LIGHT_TIME])) {
//    int hours, mins;
//    if (p.length() > 5){
//      hours = p.substring(0, 2).toInt();
//      mins = p.substring(3, 5).toInt();
//      hours = (hours > 23) ? (end_light_hour) : (hours);
//      hours = (hours < 0) ? (end_light_hour) : (hours);
//      mins = (mins > 59) ? (end_light_min) : (mins);
//      mins = (mins < 0) ? (end_light_min) : (mins);
//      end_light_hour = hours;
//      end_light_min = mins;
//      Serial.print(end_light_hour);
//      Serial.print(" : ");
//      Serial.println(end_light_min);
//    }
//  }
}




