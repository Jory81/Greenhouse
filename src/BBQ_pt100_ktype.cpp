/*************************************************** 
  This is a library for the Adafruit PT100/P1000 RTD Sensor w/MAX31865

  Designed specifically to work with the Adafruit RTD Sensor
  ----> https://www.adafruit.com/products/3328

  This sensor uses SPI to communicate, 4 pins are required to  
  interface
  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution


HALLO LEON

 ****************************************************/

#include <Arduino.h>

#ifdef ESP8266
 #include <ESP8266WiFi.h>
 #include "FS.h" 
#else
 #include <WiFi.h>
 #include <SPIFFS.h>
#endif

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include <SPI.h>
#include <EEPROM.h>

#include <ESPAsyncWebServer.h>

#include <Thermocouple.h>
#include <MAX6675_Thermocouple.h>
#include <Adafruit_MAX31865.h>

#if (defined(__AVR__))
#include <avr\pgmspace.h>
#else
#include <pgmspace.h>
#endif

#include <pt100rtd.h>
#include <AutoPID.h>

#include <DHT.h>


// OLED SD1306 properties
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

// THERMOCOUPLE PINS
#define SCK_PIN 18
#define CS_PIN 5
//#define SO_PIN 19
//#define CS2_PIN 22
#define SO_PIN 21
#define CS2_PIN 26
// NOT FULLY TESTED AND OPERATIONAL FOR MULTIPLE SENSORS - FYI


const int relayLight1 = 16;
const int relayLight2 = 16;
const int relayHeat1 = 16;
const int relayHeat2 = 16;
const int relayHumidifier = 16;
const int relay = 16;

// const int relayLight1 = 16;
// const int relayLight2 = 17;
// const int relayHeat1 = 12;
// const int relayHeat2 = 13;
// const int relayHumidifier = 14;
// const int relay = 15;


Thermocouple* thermocouple[5];
//Thermocouple* thermocouple2;

#define LED 2
#define updateTimeTemp 1000
#define updateTimeHumidity 2500

// #define DHTPIN1 34
// #define DHTPIN2 35
// #define DHTPIN3 36

#define DHTPIN1 17
#define DHTPIN2 17
#define DHTPIN3 17

DHT dht[] = {
  {DHTPIN1, DHT22},
  {DHTPIN2, DHT22},
  {DHTPIN3, DHT22},
};


Adafruit_MAX31865 maxthermo[5] = {Adafruit_MAX31865(5), Adafruit_MAX31865(26), Adafruit_MAX31865(27), Adafruit_MAX31865(32), Adafruit_MAX31865(33)} ; // 5, 26, 27, 32, 12

// The value of the Rref resistor. Use 430.0!
#define RREF 430.0

// Like, duh.
#define C2F(c) ((9 * c / 5) + 32)

pt100rtd PT100 = pt100rtd();

/*
 * functions used in this script
 */
float processRTD(uint16_t rtd);
void initializeEEPROMvariables();
void sendAllTempToClient();
void sendAllHumidityToClient();
void sendAllCalibrationValues();
void sendAllPIDValues();
void samplingTemp();
void samplingHumidity();
void updateTimeAndGraph();
void sendTimeToClient1 (uint16_t Time);
void sendCounterToClient1 (uint16_t Time);
void sendCounterToClient2 (uint16_t Time);
void updateGraph (float temp);
void updateGraph2 (float temp);
void displayOledScreen(float temp1, float temp2, float temp3, float temp4);
void fanControl();
void lightControl();
void heaterControl();
void humidityControl();
void messageFanState();
void updateFanSpeed(byte fanSpeed);

void onRootRequest(AsyncWebServerRequest *request);
void initWebServer();
void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len);
void initWebSocket();
void notifyClients(); 
void handleWebSocketMessage(void *arg, uint8_t *data, size_t len);
void processWebSocketMessageS(String str, int stringLength, String dataString);
void processWebSocketMessage(String str, int dataVar);
void processWebSocketMessageFloat(String str, float dataVar);
void processWebSocketMessageFloat2(String str, float dataVar);
void processWebSocketMessageDouble(String str, double dataVar);
double modifiedMap(double x, double in_min, double in_max, double out_min, double out_max);

void setupESP32();
void setupOledScreen();
void setupSPIFFS();
void setupEEPROM();
void setupWIFI();
void setupTempSensors();
void setupDHTSensors();

//pid settings and gains
double OUTPUT_MIN = 0;
double OUTPUT_MAX = 255;
// #define KP .12
// #define KI .0003
// #define KD 0

double KP = 5;
double KI = 3;
double KD = 1;

double temperature, setPoint, outputVal;

//input/output variables passed by reference, so they are updated automatically
AutoPID myPID(&temperature, &setPoint, &outputVal, OUTPUT_MIN, OUTPUT_MAX, KP, KI, KD);

#define OUTPUT_PIN 4
const int freq = 10;
const int ledChannel = 0;
const int resolution = 8;

#include "globalVariables.h"
#include "readTemperature.h"
#include "setupFunctions.h"
#include "websocketMessages.h"
#include "oledDisplay.h"

//#include <OneWire.h>

void setup()
{
setupESP32();
setupOledScreen();
setupSPIFFS();
setupEEPROM();
setupWIFI();
setupTempSensors();
setupDHTSensors();

ledcSetup(ledChannel, freq, resolution);
ledcAttachPin(OUTPUT_PIN, ledChannel);

myPID.setBangBang(offsetTemperatureMin);
myPID.setTimeStep(100);
}

void loop()
{
  if (millis() - previousMillis1 >= updateTimeTemp){
    samplingTemp(); 
    samplingHumidity();   
    displayOledScreen(temp[0], temp[1], temp[2], temp[3]);
    lightControl();
    heaterControl();
    humidityControl();
    fanControl();
    updateTimeAndGraph();
    previousMillis1 = millis();
    }
}

void samplingTemp(){
  switch (sensorType){
          case 1: {
            for (int sensor = 0; sensor <sensorAmount; sensor++){
              uint16_t rtd = maxthermo[sensor].readRTD();
              temp[sensor] = processRTD(rtd) - calibrationValue[sensor];
              oldtemp[sensor] = temp[sensor];
            }
            sendAllTempToClient();
          }
          break;
          case 2: {
            for (int sensor = 0; sensor <sensorAmount; sensor++){
              temp[sensor] = thermocouple[sensor]->readCelsius();
                 if ((temp[sensor] < 300 && temp[sensor] > oldtemp[sensor]-50 && temp[sensor] < oldtemp[sensor]+50 ) || oldtemp[sensor] == 0){
                  oldtemp[sensor] = temp[sensor];             
                  }
            }
            sendAllTempToClient();
          }
          break;
        }    
}

void samplingHumidity(){
    if (humiditySensorAmount > 0){
      if (millis() - previousMillis2 >= updateTimeHumidity){
      for (int sensor = 0; sensor <humiditySensorAmount; sensor++){
        preHumidity[sensor] = dht[sensor].readHumidity();
        predhtTemp[sensor] = dht[sensor].readTemperature();

        if(!isnan(predhtTemp[sensor]) && !isnan(preHumidity[sensor]))        {
          humidity[sensor] = preHumidity[sensor];
          dhtTemp[sensor] = predhtTemp[sensor];
        }

      }
      sendAllHumidityToClient();
      previousMillis2 = millis();
      }
    }
}

void fanControl(){
if (fanManual){
  if (fanON){ outputVal=fanManualAmount; }
  else { outputVal=0; }
  if (msgFanState){
    messageFanState();
  }
}
else if (!fanManual){
  if (!tempControlPID){
    fanON = true;
    if (humidifierON){
      outputVal = OUTPUT_MIN;
    }
    else {
      if (temp[0] > targetTemperature1 + offsetTemperatureMax){
        outputVal = OUTPUT_MAX;
      }
      else if (temp[0] < targetTemperature1 - offsetTemperatureMin){
        outputVal = OUTPUT_MIN;
      }
      else if (temp[0] > targetTemperature1 - offsetTemperatureMin && temp[0] < targetTemperature1 + offsetTemperatureMin){
        outputVal = fanManualAmount;
      }
      else if (temp[0] > targetTemperature1 + offsetTemperatureMin && temp[0] < targetTemperature1 + offsetTemperatureMax){
        outputVal = modifiedMap((targetTemperature1-temp[0]), 0, offsetTemperatureMin, OUTPUT_MIN, OUTPUT_MAX);
      }
      }
  }
  else {
      temperature=temp[0];
      myPID.run(); //call every loop, updates automatically at certain time interval
      if (outputVal < 5){
        fanON = false;
      }
      else{
        fanON = true;
      }
  }
  if (lastfanONState != fanON || msgFanState == true){
  lastfanONState = fanON;
  messageFanState();
  }   
}
//Serial.println(outputVal);
ledcWrite(ledChannel, outputVal);
fanSpeed = map(outputVal, 0, 255, 0, 100);
updateFanSpeed(fanSpeed);
}

void messageFanState(){
digitalWrite(LED, fanON);
String mergedString = "GQ"+String(fanON); ws.textAll(mergedString);
msgFanState = false;
return;
}

void lightControl(){
  if (lightState != lightsON){
    lightState = lightsON;
    digitalWrite(relayLight1, lightsON);
    digitalWrite(relayLight2, lightsON);
    String mergedString = "Gq"+String(lightsON); ws.textAll(mergedString);
  }
}

void heaterControl(){
  if (temp[0] < targetTemperature1 - offsetTemperatureMin){
    heaterON = true;
  }
  else if (temp[0] > targetTemperature1 + offsetTemperatureMin){
    heaterON = false;
  }

  if (heaterState != heaterON){
    heaterState = heaterON;
    digitalWrite(relayHeat1, heaterON);
    digitalWrite(relayHeat2, heaterON);
    String mergedString = "Gs"+String(heaterON); ws.textAll(mergedString);
  }
}

void humidityControl(){
  if (humidity[0] < humidmin){
    humidifierON = true;
  }
  else if (humidity[0] > humidmax){
    humidifierON = false;
  }

  if (humidifierState != humidifierON){
    humidifierState = humidifierON;
    digitalWrite(relayHumidifier, humidifierON);
    String mergedString = "Gg"+String(humidifierON); ws.textAll(mergedString);
  }
}
