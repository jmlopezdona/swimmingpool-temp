#include <ESP8266WiFi.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "ESP8266HTTPClient.h"
#include "ESP8266httpUpdate.h"

#include "config.h"

const int FW_VERSION = 10;

const char* FW_HOST = _FW_HOST;
const int FW_PORT = 80;
const char* FW_URL_VERSION = FW_URL_VERSION;
const char* FW_URL_IMAGE = _FW_URL_IMAGE;

#define ONE_WIRE_BUS 2  // DS18B20 on arduino pin2 corresponds to D4 on physical board
#define ONE_WIRE_BUS_2 0  // DS18B20 on arduino pin0 corresponds to D3 on physical board


OneWire oneWire(ONE_WIRE_BUS);
OneWire oneWire2(ONE_WIRE_BUS_2);
DallasTemperature DS18B20(&oneWire);
DallasTemperature DS18B20_2(&oneWire2);

const char* THINGSPEAK_SERVER = "api.thingspeak.com";
const String THINGSPEAK_APIKEY = _THINGSPEAK_APIKEY;

const char* MY_SSID = _MY_SSID; 
const char* MY_PWD = _MY_PWD;

void wifi_status_led_uninstall();

void setup() {
  Serial.begin(115200);
  connectWifi();
  updateLastVersion();  
}

void loop() {
  float temp;
  DS18B20.requestTemperatures(); 
  temp = DS18B20.getTempCByIndex(0);
  Serial.print("Temperature swimming pool: ");
  Serial.println(temp);

  float enviromentTemp;
  DS18B20_2.requestTemperatures(); 
  enviromentTemp = DS18B20_2.getTempCByIndex(0);
  Serial.print("Temperature enviroment: ");
  Serial.println(enviromentTemp);
  
  if (temp<=60 & temp>=-10) {
    sendTeperatureTS(temp, enviromentTemp);
  }

  ESP.deepSleep(600e6);
  //delay(30000);
}

void connectWifi() {
  Serial.print("Connecting to "+*MY_SSID);
  WiFi.begin(MY_SSID, MY_PWD);
  while (WiFi.status() != WL_CONNECTED) {
  delay(1000);
  Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("Connected");
  Serial.println("");  
}

void sendTeperatureTS(float swimmingPoolTemp, float enviromentTemp) {  
   WiFiClient client;
  
   if (client.connect(THINGSPEAK_SERVER, 80)) {
     Serial.println("Connected to thingspeak server");
     
     String postStr = THINGSPEAK_APIKEY;
     postStr += "&field1=";
     postStr += String(swimmingPoolTemp);
     postStr += "&field2=";
     postStr += String(enviromentTemp);
     postStr += "\r\n\r\n";
     
     client.print("POST /update HTTP/1.1\n");
     client.print("Host: api.thingspeak.com\n");
     client.print("Connection: close\n");
     client.print("X-THINGSPEAKAPIKEY: " + THINGSPEAK_APIKEY + "\n");
     client.print("Content-Type: application/x-www-form-urlencoded\n");
     client.print("Content-Length: ");
     client.print(postStr.length());
     client.print("\n\n");
     client.print(postStr);
     delay(1000);
   }
  client.stop();
}

void updateLastVersion() {
  int version = FW_VERSION;
  
  // Connect HOST
  WiFiClient client;
  client.setTimeout(1000);
  Serial.print("connecting to ");
  Serial.println(FW_HOST);
  if (!client.connect(FW_HOST, FW_PORT)) {
    Serial.println("connection failed");
    return;
  }

  // Call host to recovery last version
  client.print(String("GET ") + FW_URL_VERSION +" HTTP/1.1\r\n" +
             "Host: " + FW_HOST + "\r\n" +
             "accept: text/html\r\n" +
             "cache-control: max-age=0\r\n" +
             "Connection: close\r\n\r\n");

  // If call to connection is more than 5 seconds then cancel
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }

  // Read all the lines of the reply from server
  while (client.available()) {
    String line = client.readStringUntil('\n');
    Serial.println(line);
    if (line == "\r") {
      Serial.println("headers received");
      version = client.readStringUntil('\n').toInt();
      Serial.print("Latest version is: ");
      Serial.println(version);
    }
  }

  // Check if current version is below last version
  if (version > FW_VERSION)  {
    updateFirmware();
  }
  else {
    Serial.println( "Already on latest version" );
  }

  Serial.println();
  Serial.println("closing connection");
  client.flush();
  client.stop();
}

void updateFirmware() {
  Serial.print("Downloading FW image from ");
  Serial.println(FW_URL_IMAGE);
  
  auto ret = ESPhttpUpdate.update(FW_URL_IMAGE);
  switch(ret) {
    case HTTP_UPDATE_FAILED:
      Serial.printf("HTTP_UPDATE_FAILD Error (%d): %s", ESPhttpUpdate.getLastError(), ESPhttpUpdate.getLastErrorString().c_str());
      break;

    case HTTP_UPDATE_NO_UPDATES:
      Serial.println("HTTP_UPDATE_NO_UPDATES");
      break;
  }
}

