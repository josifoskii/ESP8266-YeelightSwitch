#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>         //https://github.com/tzapu/WiFiManager
#define button 12 // Pin for the button
#define led 14 //Pin for the notificationLed
WiFiManager wifiManager;
String text="{\"id\":1,\"method\":\"toggle\",\"params\":[]}\r\n"; // Function for toggle the light 
const char* H0 = "192.168.1.106"; // Ip from first light
const char* H1 = "192.168.1.107"; //Ip from second light if you are using one light comment this line and lines 47,48,49
int port =55443; // Lights port 
void setup() {
      pinMode(led, OUTPUT);
      digitalWrite(led, LOW);
      pinMode(button, INPUT);
      Serial.begin(115200);
      wifiManager.setConfigPortalTimeout(180); 
      wifiManager.autoConnect("LightSwitch");  
      Serial.println("Started!");
      digitalWrite(led, HIGH);
}
void loop() {
int tt = 0;
while (digitalRead(button) == LOW && tt < 40) //This is function with option to change the WiFi network if you press the button for ~8 sec you will enter into congif mode.
{                                             //If you press the button for less of 8 sec you will send toggle to led light.
    Serial.println(WiFi.status());
    Serial.println("Reading Seconds!");
    delay(100);
    tt++;
}
if (tt < 40){
Serial.println("ShortTime");
SendInfo();
}
else{
Serial.println("LongTime");
OnDemandConfig();
} 
}
void SendInfo(){ 
  // Use WiFiClient class to create TCP connections
  digitalWrite(led, LOW);
  WiFiClient client;
  client.connect(H0,port);
  client.print(text);
  client.stop();
  client.connect(H1,port);
  client.print(text);
  client.stop();
  digitalWrite(led, HIGH);
}
void OnDemandConfig(){
      wifiManager.setConfigPortalTimeout(180); //Startuva WiFi vo AP i moze da se prekonfigurira WIFI-ot i novite informacii ke se zacuvaat vo EEPROM.
      wifiManager.startConfigPortal("LightSwitch"); // Se pushta so ime SmartPost 
}

