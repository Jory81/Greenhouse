void setupESP32(){
  Serial.begin(115200);   
  pinMode(LED,OUTPUT);
  digitalWrite(LED, LOW);
}
void setupOledScreen(){
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    for(;;); // Don't proceed, loop forever
  }
  display.display();
  display.clearDisplay();
  display.setTextSize(1); 
  display.setTextColor(WHITE,BLACK);  
  display.setCursor (0,0);  
}
void setupSPIFFS(){
  if( !SPIFFS.begin()){
    Serial.println("Error mounting SPIFFS");
    while(1);
  }  
}
void setupEEPROM(){
  EEPROM.begin(255);
  
  #ifdef ESP8266
  int check;
  EEPROM.get(0, check);
  #else
  int check  = EEPROM.readInt(0);
  #endif
  
  if (check == 11234){
  display.print(F("code: ")); display.println(check);
  display.println(F("EEPROM SET"));
  display.display();
  initializeEEPROMvariables(); // functio in global_variables.h at the bottom.    
  delay(1000);
  }
  
  else if (check != 11234){
  display.println(F("EEPROM not initialized"));
  display.println(F("Write to EEPROM"));
  display.display();
  int eeAddress=0;
  EEPROM.put(eeAddress, customVar);
  EEPROM.commit();
  display.println(F("EEPROM is initialized"));
  display.display();
  delay(2000);
  ESP.restart();
  }
  display.clearDisplay();  
}

void setupWIFI(){
  int wifiCounter=0;
  WiFi.mode(WIFI_STA);
  const char *WIFI_SSID = &wifiID[0];
  const char *WIFI_PASS = &wifiPASS[0];
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.printf("Trying to connect [%s] ", WiFi.macAddress().c_str());
  while (WiFi.status() != WL_CONNECTED) {
      Serial.println(".");
      display.clearDisplay();
      display.setCursor(0,0);
      display.println(F("connecting to wifi..."));
      display.display();
      delay(500);
      wifiCounter++;
      wifiStationMode = true;
      if (wifiCounter > 20){
      wifiStationMode = false;  
      break;  
      }
  }
  if (wifiCounter >20){
  WiFi.mode(WIFI_OFF);
  delay(1000);
  WiFi.mode(WIFI_AP);
  WiFi.softAP(WIFI_SSID_AP, WIFI_PASS_AP);
  Serial.println(F("soft-AP IP:"));
  display.println(F("soft-AP IP:"));
  Serial.println(WiFi.softAPIP());
  display.println(WiFi.softAPIP());
  display.display();    
  }
  if (wifiStationMode){
  Serial.printf(" %s\n", WiFi.localIP().toString().c_str());
  }
  else{
  Serial.printf(" %s\n", WiFi.softAPIP().toString().c_str());
  }
  
  display.clearDisplay();
  display.setCursor(0,0);

  if (wifiStationMode){
  display.printf(" %s\n", WiFi.localIP().toString().c_str());
  }
  else {
  display.printf(" %s\n", WiFi.softAPIP().toString().c_str());
  }
    
  initWebSocket();
  initWebServer();
    
  display.println(F("setup complete"));
//    display.println(F("mDNS: esp32.local/"));
  display.display();  
  
  delay(1000);   
}

void setupTempSensors(){
  if (sensorType == 1){
    for (int sensor = 0; sensor <sensorAmount; sensor++){
    maxthermo[sensor].begin(MAX31865_2WIRE);  // set to 3WIRE or 4WIRE as necessary
    //maxthermo[sensor].begin(MAX31865_3WIRE);  // set to 2WIRE or 4WIRE as necessary
    //maxthermo[sensor].begin(MAX31865_4WIRE);  // set to 2WIRE or 3WIRE as necessary
    }
  }
  else {
    for (int sensor = 0; sensor <sensorAmount; sensor++){    
    thermocouple[0] = new MAX6675_Thermocouple(SCK_PIN, CS_PIN, SO_PIN); // sensor 1
    thermocouple[1] = new MAX6675_Thermocouple(SCK_PIN, CS2_PIN, SO_PIN); // sensor 2
    // requiers more inputs
  }
}
}

void setupDHTSensors(){
  if (humiditySensorAmount > 0){
    for (int sensor = 0; sensor < humiditySensorAmount; sensor++){
      dht[sensor].begin();
    }
  }
  else {
  return;
  }
}

void initializeEEPROMvariables(){
sensorType  = EEPROM.read(offsetof(storeInEEPROM, sensorType));
sensorAmount = EEPROM.read(offsetof(storeInEEPROM, sensorAmount));
humiditySensorAmount = EEPROM.read(offsetof(storeInEEPROM, humiditySensorAmount));
tempControlPID = EEPROM.read(offsetof(storeInEEPROM, tempControlPID));
humidmin = EEPROM.read(offsetof(storeInEEPROM, humidmin));
humidmax =  EEPROM.read(offsetof(storeInEEPROM, humidmax));
fanManualAmount = EEPROM.read(offsetof(storeInEEPROM, fanManualAmount));


#ifdef ESP8266  
EEPROM.get(offsetof(storeInEEPROM, KP, KP);
EEPROM.get(offsetof(storeInEEPROM, KI), KI);
EEPROM.get(offsetof(storeInEEPROM, KD), KD);
EEPROM.get(offsetof(storeInEEPROM, OUTPUT_MIN), OUTPUT_MIN);
EEPROM.get(offsetof(storeInEEPROM, OUTPUT_MAX), OUTPUT_MAX);
EEPROM.get(offsetof(storeInEEPROM, targetTemperature1), targetTemperature1;
EEPROM.get(offsetof(storeInEEPROM, offsetTemperatureMax), offsetTemperatureMax;
EEPROM.get(offsetof(storeInEEPROM, offsetTemperatureMin), offsetTemperatureMin;

for (int m = 0; m < 5; m++){
int offsetPosition = offsetof(storeInEEPROM, calibrationValue[0]);
EEPROM.get(offsetPosition+(4*m), calibrationValue[m]);
}

for (int m = 0; m < 8; m++){
int offsetPosition = offsetof(storeInEEPROM, lightSchedule[0]);
EEPROM.get(offsetPosition+m, lightSchedule[m]);
}

#else
targetTemperature1 = EEPROM.readFloat(offsetof(storeInEEPROM, targetTemperature1));
offsetTemperatureMax = EEPROM.readFloat(offsetof(storeInEEPROM, offsetTemperatureMax));
offsetTemperatureMin = EEPROM.readFloat(offsetof(storeInEEPROM, offsetTemperatureMin));
KP = EEPROM.readDouble(offsetof(storeInEEPROM, KP));
KI = EEPROM.readDouble(offsetof(storeInEEPROM, KI));
KD = EEPROM.readDouble(offsetof(storeInEEPROM, KD));
OUTPUT_MIN = EEPROM.readDouble(offsetof(storeInEEPROM, OUTPUT_MIN));
OUTPUT_MAX = EEPROM.readDouble(offsetof(storeInEEPROM, OUTPUT_MAX));

for (int m = 0; m < 5; m++){
int offsetPosition = offsetof(storeInEEPROM, calibrationValue[0]);
calibrationValue[m]  = EEPROM.readFloat(offsetPosition+4*m);
}

for (int m = 0; m < 8; m++){
int offsetPosition = offsetof(storeInEEPROM, lightSchedule[0]);
lightSchedule[m]  = EEPROM.readChar(offsetPosition+m);
// Serial.println(lightSchedule[m]);
}

//Serial.print("position: "); Serial.print(offsetPosition+4*m); Serial.print(" cal. value: "); Serial.println(calibrationValue[m]);
#endif

 
for (int m = 0; m < 32; m++){
int offsetPosition = offsetof(storeInEEPROM, ssidStorage[0]);
ssidStorage[m]  = EEPROM.read(offsetPosition+m);
}
wifiID = String(ssidStorage);
//Serial.print("wifiID "); Serial.println(wifiID);

for (int m = 0; m < 32; m++){
//Serial.print(m); Serial.print("m ");   
int offsetPosition = offsetof(storeInEEPROM, passStorage[0]);
passStorage[m]  = EEPROM.read(offsetPosition+m);
}
wifiPASS = String(passStorage);
//Serial.print("wifiPASS "); Serial.println(wifiPASS);

}