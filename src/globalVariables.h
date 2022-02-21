//TEMPERATURE SETTINGS
float targetSoilTemp1 = 25.0f; //! float targetTemperature1=25.0f;
float tempRange1 = 1.5f; //! float offsetTemperatureMin = 1.5f;
float alarmRange1 = 10.0f; //! float offsetTemperatureMax=10.0f;
boolean soilAlarm1 = false;
float targetAirTemp1 = 25.0f; //!
boolean airAlarm1 = false;

float targetSoilTemp2 = 25.0f;
float tempRange2 = 1.5f;
float alarmRange2 = 10.0f;
boolean soilAlarm2 = false;
float targetAirTemp2 = 25.0f;
boolean airAlarm2 = false;

float temp[5] = {0,0,0,0,0}; 
float oldtemp[5] = {0,0,0,0,0}; 

float preHumidity[3] = {0,0,0};
float humidity[3] = {0,0,0};

float predhtTemp[3] = {0,0,0};
float dhtTemp[3] = {0,0,0};

//FAN SETTINGS
boolean manualMosfet1 = false; //boolean fanManual = false;
uint8_t manualFanspeed1 = 127; //byte fanManualAmount = 127;
boolean manualMosfet2 = false;
uint8_t manualFanspeed2 = 127;
boolean fan1Connected = false;
boolean fan2Connected = false;

double OUTPUT_MIN1 = 0;
double OUTPUT_MAX1 = 255;
double OUTPUT_MIN2 = 0;
double OUTPUT_MAX2 = 255;

//RELAY SETTINGS
boolean manualRelay1 = false; //controls heater1
boolean manualRelay2 = false; //controls heater2
boolean manualRelay3 = false; // controls lights1
boolean manualRelay4 = false; // controls lights2
boolean manualRelay5 = false; // optional/ humidity1
boolean manualRelay6 = false; // optional/ humidity2

boolean relay1Connected = false; //controls heater1
boolean relay2Connected = false; //controls heater2
boolean relay3Connected = false; // controls lights1
boolean relay4Connected = false; // controls lights2
boolean relay5Connected = false; // optional/ humidity1
boolean relay6Connected = false; // optional/ humidity2

//LIGHT SETTINGS
char lights1on[6];
char lights1off[6];
char lights2on[6];
char lights2off[6];

//HUMIDITY SETTINGS
uint8_t humidMin1 = 30;
uint8_t humidMax1 = 70;
uint8_t humidMin2 = 30;
uint8_t humidMax2 = 70;

//CALIBRATION SETTINGS
float calibrationValue[11] = {-0.0f, -0.0f, -0.0f, -0.0f, -0.0f, -0.0f, -0.0f, -0.0f, -0.0f, -0.0f, -0.0f};
//float calValue1 = -0.0; // temperature 1 climate 1 soil 
//float calValue2 = -0.0; // temperature 2 climate 1 temp 2
//float calValue3 = -0.0; // temperature 3 climate 2 soil
//float calValue4 = -0.0; // temperature 4 climate 2 temp 2
//float calValue5 = -0.0; // temperature 5
//float calValue6 = -0.0; // DHT temperature 6 climate 1 air
//float calValue7 = -0.0; // DHT temperature 7 climate 2 air
//float calValue8 = -0.0; // DHT temperature 8
//float calValue9 = -0.0; // humidity % climate 1 humidity 
//float calValue10 = -0.0; // humidity % climat 2 humidity
//float calValue11 = -0.0; // humidity %
double KP = 5;
double KI = 3;
double KD = 1;
double temperature;
double setPoint;
double outputVal1;
double outputVal2;

//SYSTEM SETTINGS
int check;
boolean initializeTimeOnOpen = true;
uint8_t probeTypeT = 1; //uint8_t sensorType = 1;
uint8_t probeCountT = 1; // uint8_t sensorAmount = 1;
uint8_t probeTypeH = 1; 
uint8_t probeCountH = 0; // uint8_t humiditySensorAmount = 0;
uint32_t graphUpdate = 5000; //uint32_t updateTimeGraph = 5000;
boolean PIDcontrol = false;
char SSID[32]; // or char[100]; //!
char PASS[32]; // or char[100]; //!
//uint8_t relayCount = 6;
uint8_t fanCount = 0;
boolean Reboot = false;
boolean saveInEEPROM = false;

const char *WIFI_SSID_AP = "ESP32-AP";
const char *WIFI_PASS_AP =  "";
String wifiID;
String wifiPASS;
char ssidStorage[32];
char passStorage[32];
boolean wifiStationMode=false;

//CLIMATE 1 PARAMETERS
float temperature1;
boolean heater1; //boolean heaterON = false; // heater1
float temperature6;
uint8_t fanspeed1 = 127; // fanspeed
boolean fan1; //boolean fanON = false;
float humidity1;
boolean humidifier1; //boolean humidifierON = false;
String lights1ON; //!
String lights1OFF; //!
boolean lights1; //boolean lightsON = true;
float temperature2;

boolean heaterState1 = !heater1;
boolean lightState1 = !lights1;
boolean humidifierState1 = !humidifier1;
boolean fanState1 = !fan1;

//CLIMATE 2 PARAMETERS
float temperature3;
boolean heater2;
float temperature7;
uint8_t fanspeed2 = 127;
boolean fan2;
float humidity2;
boolean humidifier2;
String lights2ON; //!
String lights2OFF; //!
boolean lights2;
float temperature4;

boolean heaterState2 = !heater2;
boolean lightState2 = !lights2;
boolean humidifierState2 = !humidifier2;
boolean fanState2 = !fan2;

//TIME RELATED PARAMETERS
unsigned long previousMillis=0;
unsigned long previousMillis1=0;
unsigned long previousMillis2=0;
unsigned long previousMillis3=0;
unsigned long previousMillis4=0;

unsigned long currentMillis = 0;
unsigned long seconds = 0;
int minutes = 0;
int hours = 0;

int minutesStart = 0;
int hourStart = 0;

uint16_t EEPROMposition = 0;
uint16_t stringLength;
boolean msgFanState1= true;
boolean msgFanState2= true;
boolean displayTime;

struct storeInEEPROM {
int check;
uint8_t probeTypeT;
uint8_t probeCountT;
uint8_t probeTypeH;
uint8_t probeCountH;
//uint8_t relayCount;
boolean relay1Connected;
boolean relay2Connected;
boolean relay3Connected;
boolean relay4Connected;
boolean relay5Connected;
boolean relay6Connected;
boolean fan1Connected;
boolean fan2Connected;
uint32_t graphUpdate;
uint8_t humidMin1;
uint8_t humidMax1;
uint8_t humidMin2;
uint8_t humidMax2;
byte manualFanspeed1;
byte manualFanspeed2;
char ssidStorage[32];
char passStorage[32];
char lights1on[6];
char lights1off[6];
char lights2on[6];
char lights2off[6];
float calibrationValue[11];
boolean PIDcontrol;
double KP;
double KI;
double KD;
boolean manualMosfet1;
boolean manualMosfet2;
double OUTPUT_MIN1;
double OUTPUT_MAX1;
double OUTPUT_MIN2;
double OUTPUT_MAX2;

float targetSoilTemp1;
float targetAirTemp1;
float alarmRange1;
float tempRange1;

float targetSoilTemp2;
float targetAirTemp2;
float alarmRange2;
float tempRange2;
};

storeInEEPROM customVar = {
      11331, // code to check
      1, // sensorTypeT ; 1 is PT100 2 is thermokoppel
      1, // sensorAmountT 
      1, // sensorTypeH
      0, // humidsensoramount
      0, // relay1Connected
      0, // relay2Connected
      0, // relay3Connected
      0, // relay4Connected
      0, // relay5Connected
      0, // relay6Connected
      0, // fan1Connected
      0, // fan2Connected
      5000, // graphupdate
      30, // humidmin1
      80, // humidmax1
      30, // humidmin2
      80, // humidmax2
      50, // manualFanSpeed
      50, // manualFanSpeed2
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // ssidstorage
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // passstorage
      0, 0, 0, 0, 0, 0, // lights1on
      0, 0, 0, 0, 0, 0, // lights1off
      0, 0, 0, 0, 0, 0, // lights2on
      0, 0, 0, 0, 0, 0, // lights2off
      0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, //11 calibration values
      0, // boolean tempControlPID;
      5, 3, 1, // PID values
      0, 0, // manualMosfet1 manualMosfet2
      0, 255, // OUTPUT_MIN1 OUTPUT_MAX1
      0, 255, // OUTPUT_MIN2 OUTPUT_MAX2
      25.0f, // targetSoilTemp1
      25.0f, // targetAirTemp1
      10.0f, // alarmTemp1
      1.5f, // temprange1
      25.0f, // targetSoilTemp2
      25.0f, // targetAirTemp2
      10.0f, // alarmTemp2
      1.5f // temprange2
    };

char stringStorage[32];
