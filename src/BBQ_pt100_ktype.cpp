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
#include <ArduinoJson.h>

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

//#define RELAY_PIN   17

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

// THERMOCOUPLE PINS
#define SCK_PIN 18
#define CS_PIN 5
#define SO_PIN 19
#define CS2_PIN 22
// #define SO_PIN 21
// #define CS2_PIN 26
// NOT FULLY TESTED AND OPERATIONAL FOR MULTIPLE SENSORS - FYI

const int RELAYPIN1 = 14;
const int RELAYPIN2 = 13;
const int RELAYPIN3 = 25; 
const int RELAYPIN4 = 33;
const int RELAYPIN5 = 2; // DISCONNECT PIN AT FLASH - CONNECT AFTER FLASHING
const int RELAYPIN6 = 15; //

#define OUTPUT_PIN1 4 // Fan1
const int freq = 10;
const int ledChannel1 = 0;
const int resolution = 8;

#define OUTPUT_PIN2 0 // fan2 not tested
//const int freq = 10;
const int ledChannel2 = 1;

Thermocouple* thermocouple[5];
//Thermocouple* thermocouple2;

#define updateTempAndRelays 1000 //1000
#define updateHumidity 2500
#define updateOledDisplay 5000

#define DHTPIN1 16 // 
#define DHTPIN2 17
#define DHTPIN3 12 // DISCONNECT PIN AT FLASH - CONNECT AFTER FLASHING/ RUNNING 

DHT dht[] = {
  {DHTPIN1, DHT22},
  {DHTPIN2, DHT22},
  {DHTPIN3, DHT22},
};

Adafruit_MAX31865 maxthermo[4] = {Adafruit_MAX31865(5), Adafruit_MAX31865(26), Adafruit_MAX31865(27), Adafruit_MAX31865(32)} ; // 5, 26, 27, 32, 12 // 35 probably won't work, but there are no pins left
//Adafruit_MAX31865 maxthermo[5] = {Adafruit_MAX31865(34), Adafruit_MAX31865(35), Adafruit_MAX31865(36), Adafruit_MAX31865(39), Adafruit_MAX31865(5)} ;
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
void timeControl();
void reInitializeTimeStrings();
void reInitializeTimeInts();
void writeStringToEEPROM(int addrOffset, const String &strToWrite);
int convertStringToInt(String str, int workflow);
double modifiedMap(double x, double in_min, double in_max, double out_min, double out_max);

void sendTempToClient();
void sendHumidityToClient();
void samplingTemp();
void samplingHumidity();
void updateGraph();

void displayOledScreen(float temp1, float temp2, float temp3, float temp4);

void fan1Control();
void light1Control(boolean manualRelay, const int RELAYPIN);
void heater1Control(boolean manualRelay, const int RELAYPIN);
void humidity1Control(boolean manualRelay, const int RELAYPIN);
void messageFanState1();

void executeTask(byte function, boolean manualRelay, const int RELAYPIN);

void fan2Control();
void light2Control(boolean manualRelay, const int RELAYPIN);
void heater2Control(boolean manualRelay, const int RELAYPIN);
void humidity2Control(boolean manualRelay, const int RELAYPIN);
void messageFanState2();

void light3Control(boolean manualRelay, const int RELAYPIN);
void heater3Control(boolean manualRelay, const int RELAYPIN);
void humidity3Control(boolean manualRelay, const int RELAYPIN);

void onRootRequest(AsyncWebServerRequest *request);
void initWebServer();
void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len);
void initWebSocket();
void handleWebSocketMessage(void *arg, uint8_t *data, size_t len);
void notifyClients(); 
void notifyClientsSingleObject(String object, boolean value);
void notifyClientsSingleString(String object, String &message);
void sendProgramInfo();

void setupESP32();
void setupOledScreen();
void setupSPIFFS();
void setupEEPROM();
void setupWIFI();
void setupTempSensors();
void setupDHTSensors();
void setupFans();
void setupRelays();

#include "globalVariables.h"
#include "readTemperature.h"
#include "setupFunctions.h"
#include "websocketMessages.h"
#include "oledDisplay.h"

//input/output variables passed by reference, so they are updated automatically
AutoPID myPID(&temperature, &setPoint, &outputVal1, OUTPUT_MIN1, OUTPUT_MAX1, KP, KI, KD);

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
setupFans();
setupRelays();

myPID.setBangBang(tempRange1);
myPID.setTimeStep(100);
}

void loop(){
    //ws.cleanupClients();

    if (millis() - previousMillis1 >= updateTempAndRelays){
      timeControl();
      samplingTemp();        
      sendTempToClient();
      if (relay1Connected){executeTask(funcRelay1, manualRelay1, RELAYPIN1);};
      if (relay2Connected){executeTask(funcRelay2, manualRelay2, RELAYPIN2);}; 
      if (relay3Connected){executeTask(funcRelay3, manualRelay3, RELAYPIN3);};
      if (relay4Connected){executeTask(funcRelay4, manualRelay4, RELAYPIN4);};
      if (relay5Connected){executeTask(funcRelay5, manualRelay5, RELAYPIN5);};
      if (relay6Connected){executeTask(funcRelay6, manualRelay6, RELAYPIN6);};
    previousMillis1 = millis();     
    }

    if (millis() - previousMillis2 >= updateHumidity){
      samplingHumidity();
      sendHumidityToClient();
    previousMillis2 = millis();
    }

    if (millis() - previousMillis3 >= graphUpdate){
      updateGraph();
    previousMillis3 = millis();
    }

    if (millis() - previousMillis4 >= updateOledDisplay){
      displayOledScreen(temp[0], temp[1], temp[2], temp[3]);
    previousMillis4 = millis();
    }
}

void timeControl(){
  currentMillis = millis();
  seconds = currentMillis / 1000;
  minutes = (seconds / 60);
  totalMinutes = minutes + minutesStart;
  hours = (minutes / 60);
  totalHours = hours + hourStart;
  days = hours / 24;
  
  currentMillis %= 1000;
  seconds %= 60;
  minutes %= 60;
  totalMinutes %= 60;
  hours %= 24;
  boolean mismatch;
  if (minutes + minutesStart > 59){
      mismatch = 1;
  }
  else {
      mismatch = 0;
  }
  totalHours = totalHours + mismatch;
  totalHours %= 24;

  // char buffer[40];
  // sprintf(buffer, "%02d:%02d", hours, minutes);

  currentMinutes = ((totalHours*60)+totalMinutes);
  
  //Serial.println(buffer);
}

void executeTask(byte function, boolean manualRelay, const int relayPin){
  switch (function){
    case 0: break; // do nothing - no function attached
    case 1: heater1Control(manualRelay, relayPin); break;
    case 2: heater2Control(manualRelay, relayPin); break;
    case 3: heater3Control(manualRelay, relayPin); break;
    case 4: light1Control(manualRelay, relayPin); break;
    case 5: light2Control(manualRelay, relayPin); break;
    case 6: light3Control(manualRelay, relayPin); break;
    case 7: humidity1Control(manualRelay, relayPin); break;
    case 8: humidity2Control(manualRelay, relayPin); break;
    case 9: humidity3Control(manualRelay, relayPin); break;
    default: break;
    return;
}
}

void light1Control(boolean manualRelay, const int relayPin){
  if (!manualRelay){
    if (currentMinutes > minutesLights1On && currentMinutes < minutesLights1Off){
      lights1=true;
      targetSoilTemp1=daySoilTemp1;
    }
    else if (currentMinutes < minutesLights1On || currentMinutes > minutesLights1On){
      lights1=false;
      targetSoilTemp1=nightSoilTemp1;
    }
  }
  bool relayReg1 = !(*portOutputRegister( digitalPinToPort(relayPin) ) & digitalPinToBitMask(relayPin));
      if (lights1 != relayReg1){
        digitalWrite(relayPin, !lights1);
        //Serial.println("send lights message to client");
        notifyClientsSingleObject("lights1", lights1);
      }
  return;
}

void light2Control(boolean manualRelay, const int relayPin){
 if (!manualRelay){
    if (currentMinutes > minutesLights2On && currentMinutes < minutesLights2Off){
      lights2=true;
      targetSoilTemp2=daySoilTemp2;
      //Serial.println("LIGHTS2_ON");
    }
    else if (currentMinutes < minutesLights2On || currentMinutes > minutesLights2On){
      lights2=false;
      targetSoilTemp2=nightSoilTemp2;
      //Serial.println("LIGHTS2_OFF");
    }
  } 
  bool relayReg2 = !(*portOutputRegister( digitalPinToPort(relayPin) ) & digitalPinToBitMask(relayPin));

  if (lights2 != relayReg2){
    //lightState2 = lights2;
    digitalWrite(relayPin, !lights2);
    //Serial.println("send lights message to client");
    notifyClientsSingleObject("lights2", lights2);
  }
  return;
  }

  void light3Control(boolean manualRelay, const int relayPin){
 if (!manualRelay){
    if (currentMinutes > minutesLights3On && currentMinutes < minutesLights3Off){
      lights3=true;
      targetSoilTemp3=daySoilTemp3;
      //Serial.println("LIGHTS2_ON");
    }
    else if (currentMinutes < minutesLights3On || currentMinutes > minutesLights3On){
      lights3=false;
      targetSoilTemp3=nightSoilTemp3;
      //Serial.println("LIGHTS2_OFF");
    }
  } 
  bool relayReg3 = !(*portOutputRegister( digitalPinToPort(relayPin) ) & digitalPinToBitMask(relayPin));

  if (lights3 != relayReg3){
    //lightState2 = lights2;
    digitalWrite(relayPin, !lights3);
    //Serial.println("send lights message to client");
    notifyClientsSingleObject("lights3", lights3);
  }
  return;
  }

  void heater1Control(boolean manualRelay, const int relayPin){
  if (!manualRelay){
    if (temp[0] < targetSoilTemp1 - tempRange1){
      heater1 = true;
    }
    else if (temp[0] > targetSoilTemp1 + tempRange1){
      heater1 = false;
    }
  }
  bool relayReg4 = !(*portOutputRegister( digitalPinToPort(relayPin) ) & digitalPinToBitMask(relayPin));
  if (heater1 != relayReg4){
    //heaterState1 = heater1;
    digitalWrite(relayPin, !heater1);
    notifyClientsSingleObject("heater1", heater1);
  }
  return;
}

void heater2Control(boolean manualRelay, const int relayPin){
  if (!manualRelay){
    if (temp[1] < targetSoilTemp2 - tempRange2){
      heater2 = true;
    }
    else if (temp[1] > targetSoilTemp2 + tempRange2){
      heater2 = false;
    }
  }
  bool relayReg5 = !(*portOutputRegister( digitalPinToPort(relayPin) ) & digitalPinToBitMask(relayPin));
  if (heater2 != relayReg5){
    //heaterState2 = heater2;
    digitalWrite(relayPin, !heater2);
    notifyClientsSingleObject("heater2", heater2);
  }
  return;
}

void heater3Control(boolean manualRelay, const int relayPin){
  if (!manualRelay){
    if (temp[2] < targetSoilTemp3 - tempRange3){
      heater3 = true;
    }
    else if (temp[1] > targetSoilTemp3 + tempRange3){
      heater3 = false;
    }
  }
  bool relayReg6 = !(*portOutputRegister( digitalPinToPort(relayPin) ) & digitalPinToBitMask(relayPin));
  if (heater3 != relayReg6){
    //heaterState2 = heater2;
    digitalWrite(relayPin, !heater3);
    notifyClientsSingleObject("heater3", heater3);
  }
  return;
}

void humidity1Control(boolean manualRelay, const int relayPin){
  if (!manualRelay){
    if (humidity[0] < humidMin1){
      humidifier1 = true;
    }
    else if (humidity[0] > humidMax1){
      humidifier1 = false;
    }
  }
  bool relayReg7 = !(*portOutputRegister( digitalPinToPort(relayPin) ) & digitalPinToBitMask(relayPin));
    //if (humidifierState1 != humidifier1){
    if (humidifier1 != relayReg7){
      //humidifierState1 = humidifier1;
      digitalWrite(relayPin, humidifier1);
      notifyClientsSingleObject("humidifier1", humidifier1);
    }
    return;
}

void humidity2Control(boolean manualRelay, const int relayPin){
if (!manualRelay){
  if (humidity[1] < humidMin2){
    humidifier2 = true;
  }
  else if (humidity[1] > humidMax2){
    humidifier2 = false;
  }
}
bool relayReg8 = !(*portOutputRegister( digitalPinToPort(relayPin) ) & digitalPinToBitMask(relayPin));
  // if (humidifierState2 != humidifier2){
  //   humidifierState2 = humidifier2;
  if (humidifier2 != relayReg8){
    digitalWrite(relayPin, humidifier2);
    notifyClientsSingleObject("humidifier2", humidifier2);
  }
  return;
}

void humidity3Control(boolean manualRelay, const int relayPin){
if (!manualRelay){
  if (humidity[2] < humidMin3){
    humidifier3 = true;
  }
  else if (humidity[2] > humidMax3){
    humidifier3 = false;
  }
}
bool relayReg9 = !(*portOutputRegister( digitalPinToPort(relayPin) ) & digitalPinToBitMask(relayPin));
  // if (humidifierState2 != humidifier2){
  //   humidifierState2 = humidifier2;
  if (humidifier3 != relayReg9){
    digitalWrite(relayPin, humidifier3);
    notifyClientsSingleObject("humidifier3", humidifier3);
  }
  return;
}

void samplingTemp(){
  switch (probeTypeT){
          case 1: {
            for (int sensor = 0; sensor <probeCountT; sensor++){
              uint16_t rtd = maxthermo[sensor].readRTD();
              temp[sensor] = processRTD(rtd) - calibrationValue[sensor];
              oldtemp[sensor] = temp[sensor];
            }
          }
          break;
          case 2: {
            for (int sensor = 0; sensor <probeCountT; sensor++){
              temp[sensor] = thermocouple[sensor]->readCelsius();
                 if ((temp[sensor] < 300 && temp[sensor] > oldtemp[sensor]-50 && temp[sensor] < oldtemp[sensor]+50 ) || oldtemp[sensor] == 0){
                  oldtemp[sensor] = temp[sensor] - calibrationValue[sensor];;             
                  }
            }
          }
          break;
        }    
}

void samplingHumidity(){
    if (probeCountH > 0){
        for (int sensor = 0; sensor < probeCountH; sensor++){
        preHumidity[sensor] = dht[sensor].readHumidity();
        predhtTemp[sensor] = dht[sensor].readTemperature();
          if(!isnan(predhtTemp[sensor]) && !isnan(preHumidity[sensor])){
            dhtTemp[sensor] = predhtTemp[sensor] - calibrationValue[sensor+5];;
            humidity[sensor] = preHumidity[sensor] - calibrationValue[sensor+8];;
            // Serial.printf("sensor %d humidity ", sensor); Serial.print(humidity[sensor]);
            // Serial.printf(" temp "); Serial.println(dhtTemp[sensor]);
          }
        }
    }
}

void fan1Control(){ // CLIMATE 1 based on TargetAirTemp1 and value from DHT22 sensor.
if (manualMosfet1){
  if (fan1){ outputVal1=manualFanspeed1; }
  else { outputVal1=0; }
  if (msgFanState1){
    messageFanState1();
  }
}
else if (!manualMosfet1){
  if (!PIDcontrol){
    fan1 = true;
    if (humidifier1){
      outputVal1 = OUTPUT_MIN1;
    }
    else {
      if (dhtTemp[0] > targetAirTemp1 + alarmRange1){
        outputVal1 = OUTPUT_MAX1;
      }
      else if (dhtTemp[0] < targetAirTemp1 - tempRange1){
        outputVal1 = OUTPUT_MIN1;
      }
      else if (dhtTemp[0] > targetAirTemp1 - tempRange1 && dhtTemp[0] < targetAirTemp1 + tempRange1){
        outputVal1 = manualFanspeed1;
      }
      else if (dhtTemp[0] > targetAirTemp1 + tempRange1 && dhtTemp[0] < targetAirTemp1 + alarmRange1){
        outputVal1 = modifiedMap((targetAirTemp1-dhtTemp[0]), 0, tempRange1, OUTPUT_MIN1, OUTPUT_MAX1);
      }
      }
  }
  else {
      temperature=dhtTemp[0];
      myPID.run(); //call every loop, updates automatically at certain time interval
      if (outputVal1 < 5){
        fan1 = false;
      }
      else{
        fan1 = true;
      }
  }
  if (fanState1 != fan1 || msgFanState1 == true){
  fanState1 = fan1;
  messageFanState1();
  }   
}
//Serial.println(outputVal);
ledcWrite(ledChannel1, outputVal1);
fanspeed1 = map(outputVal1, 0, 255, 0, 100);
notifyClientsSingleObject("fanspeed1", fanspeed1);   
}

void messageFanState1(){
//digitalWrite(ONBOARD_LED, fan1);
msgFanState1 = false;
notifyClientsSingleObject("fan1", fan1);
return;
}

void fan2Control(){ // CLIMATE 1 based on TargetAirTemp1 and value from DHT22 sensor.
if (manualMosfet2){
  if (fan2){ outputVal2=manualFanspeed2; }
  else { outputVal2=0; }
  if (msgFanState2){
    messageFanState2();
  }
}
else if (!manualMosfet2){
    fan2 = true;
    if (humidifier2){
      outputVal2 = OUTPUT_MIN2;
    }
    else {
      if (dhtTemp[1] > targetAirTemp2 + alarmRange2){
        outputVal2 = OUTPUT_MAX2;
      }
      else if (dhtTemp[1] < targetAirTemp2 - tempRange2){
        outputVal2 = OUTPUT_MIN2;
      }
      else if (dhtTemp[1] > targetAirTemp2 - tempRange2 && dhtTemp[1] < targetAirTemp2 + tempRange2){
        outputVal2 = manualFanspeed2;
      }
      else if (dhtTemp[1] > targetAirTemp2 + tempRange2 && dhtTemp[1] < targetAirTemp2 + alarmRange2){
        outputVal2 = modifiedMap((targetAirTemp2-dhtTemp[1]), 0, tempRange2, OUTPUT_MIN2, OUTPUT_MAX2);
      }
      }
  if (fanState2 != fan2 || msgFanState2 == true){
  fanState2 = fan2;
  messageFanState2();
  }   
}
//Serial.println(outputVal);
ledcWrite(ledChannel2, outputVal2);
fanspeed2 = map(outputVal2, 0, 255, 0, 100);
notifyClientsSingleObject("fanspeed2", fanspeed2);   
}

void messageFanState2(){
msgFanState2 = false;
notifyClientsSingleObject("fan2", fan2);
return;
}