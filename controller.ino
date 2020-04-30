/*
 *  This sketch sends a message to a TCP server
 *
 */

#include <WiFi.h>
#include <WiFiMulti.h>
#include <Wire.h>
#include "Sparkfun_APDS9301_Library.h"

APDS9301 apds;
#define INT_PIN 2
#define LED_PIN 15
#define LED_CHANNEL 0
WiFiMulti WiFiMulti;
WiFiClient client;

bool lightIntHappened = false; 
const uint16_t port = 5001;
const char * host = "192.168.0.92"; // ip or dns
int maxloops = 0, brightness=0;
bool change_brightness=true;
float lux_lvl, lux_lvl2=-2; 
void setup()
{
    Serial.begin(115200);
    delay(10);
    Wire.begin(21, 22);
    APDS_setup();
    pinMode(INT_PIN, INPUT_PULLUP);
    ledcSetup(LED_CHANNEL, 5000, 8);
    ledcAttachPin(LED_PIN, LED_CHANNEL);
    ledcWrite(LED_CHANNEL, 0);
    attachInterrupt(digitalPinToInterrupt(INT_PIN), lightInt, FALLING);
    Serial.println(apds.getLowThreshold());
    Serial.println(apds.getHighThreshold());
    // We start by connecting to a WiFi network
    WiFiMulti.addAP("www.greenlan.pl-prv-A52C", "kamilus1");
    
    Serial.println();
    Serial.println();
    Serial.print("Waiting for WiFi... ");

    while(WiFiMulti.run() != WL_CONNECTED) {
        Serial.print(".");
        delay(500);
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    delay(500);


    Serial.print("Connecting to ");
    Serial.println(host);
    
    
    while(!client.connect(host, port)) {
        Serial.println("Connection failed.");
        Serial.println("Waiting 5 seconds before retrying...");
        delay(5000);
        
    }
    apds.clearIntFlag();
    lux_lvl = apds.readLuxLevel();
    lux_lvl2 = lux_lvl;
}


void loop()
{
  
  
  String message;
  if(!change_brightness){
    apds.clearIntFlag();
    lux_lvl = apds.readLuxLevel();
    if(abs(lux_lvl-lux_lvl2)>=3){
      change_brightness = true;
      Serial.println(lux_lvl2);
      Serial.println(lux_lvl);
      lux_lvl2 = lux_lvl;
    }
  }
  if(change_brightness){
    ledcWrite(LED_CHANNEL, 0);
    delay(500);
    float ll;
    apds.clearIntFlag();
    ll = apds.readLuxLevel();
    message = String(ll);
    client.print(message);
  }

  

  //wait for the server's reply to become available
  if (client.available() > 0)
  {
    //read back one line from the server
    String line;
    line = client.readStringUntil('\r');
    Serial.println(line);
    brightness = line.toInt();
    ledcWrite(LED_CHANNEL, brightness);
    change_brightness = false;
    delay(500);
    apds.clearIntFlag();
    lux_lvl = apds.readLuxLevel();
    lux_lvl2 = lux_lvl;
  }
  /*
  else
  {
    Serial.println("client.available() timed out ");
    client.stop();
    while(!client.connect(host, port)) {
        Serial.println("Connection failed.");
        Serial.println("Waiting 5 seconds before retrying...");
        delay(5000);
        
    }
    
  }*/
}

void lightInt()
{
  lightIntHappened = true;
}
void APDS_setup(){
    apds.begin(0x39); 
    apds.setGain(APDS9301::HIGH_GAIN);
    apds.setIntegrationTime(APDS9301::INT_TIME_402_MS);
    apds.setLowThreshold(0);
    apds.setHighThreshold(50);
    apds.setCyclesForInterrupt(1); 
    apds.enableInterrupt(APDS9301::INT_ON); // Enable the interrupt.
  apds.clearIntFlag();
  }
