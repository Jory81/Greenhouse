void setupESP32(){
  Serial.begin(115200); 
  //pinMode(onboard_led.pin, OUTPUT);
  //pinMode(button.pin,      INPUT);
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
  EEPROM.begin(750);
  
  #ifdef ESP8266
  int check;
  EEPROM.get(0, check);
  #else
  int check  = EEPROM.readInt(0);
  Serial.print("check is: "); Serial.println(check);
  #endif
  
  if (check == 11221){
  display.print(F("code: ")); display.println(check);
  display.println(F("EEPROM SET"));
  display.display();
  initializeEEPROMvariables(); // functio in global_variables.h at the bottom.    
  delay(1000);
  }
  
  else if (check != 11221){
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

void setupRTC(){

   if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }
//rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
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
  if (probeTypeT == 1){
      for (int sensor = 0; sensor < probeCountT; sensor++){
      maxthermo[sensor].begin(MAX31865_2WIRE);  // set to 3WIRE or 4WIRE as necessary
      //maxthermo[sensor].begin(MAX31865_3WIRE);  // set to 2WIRE or 4WIRE as necessary
      //maxthermo[sensor].begin(MAX31865_4WIRE);  // set to 2WIRE or 3WIRE as necessary
      }
  }
  else if (probeTypeT == 2) {
    for (int sensor = 0; sensor < probeCountT; sensor++){    
    thermocouple[0] = new MAX6675_Thermocouple(SCK_PIN, CS_PIN, SO_PIN); // sensor 1
    thermocouple[1] = new MAX6675_Thermocouple(SCK_PIN, CS2_PIN, SO_PIN); // sensor 2
    // requiers more inputs
  }
  }
}

void setupDHTSensors(){
  if (probeCountH > 0){
    for (int sensor = 0; sensor < probeCountH; sensor++){
      dht[sensor].begin();
    }
  }
  else {
  return;
  }
}

void setupFans(){
  ledcSetup(ledChannel1, freq, resolution);
  ledcSetup(ledChannel2, freq, resolution);
  ledcAttachPin(OUTPUT_PIN1, ledChannel1);
  ledcAttachPin(OUTPUT_PIN2, ledChannel2);
  if (!fan1Connected){
      ledcWrite(ledChannel1, 0);
  }
  if (!fan2Connected){
        ledcWrite(ledChannel2, 0);
  }
}

void setupRelays(){
  pinMode(RELAYPIN1,   OUTPUT);
  digitalWrite(RELAYPIN1, true);
  
  pinMode(RELAYPIN2,   OUTPUT);
  digitalWrite(RELAYPIN2, true);
  
  pinMode(RELAYPIN3,   OUTPUT);
  digitalWrite(RELAYPIN3, true);
  
  pinMode(RELAYPIN4,   OUTPUT);
  digitalWrite(RELAYPIN4, true);
  
  pinMode(RELAYPIN5,   OUTPUT);
  digitalWrite(RELAYPIN5, true);
  
  pinMode(RELAYPIN6 ,   OUTPUT);
 digitalWrite(RELAYPIN6, true);
}

void initializeEEPROMvariables(){
probeTypeT  = EEPROM.read(offsetof(storeInEEPROM, probeTypeT));
probeCountT = EEPROM.read(offsetof(storeInEEPROM, probeCountT));
probeTypeH  = EEPROM.read(offsetof(storeInEEPROM, probeTypeH));
probeCountH = EEPROM.read(offsetof(storeInEEPROM, probeCountH));
relay1Connected = EEPROM.read(offsetof(storeInEEPROM, relay1Connected));
relay2Connected = EEPROM.read(offsetof(storeInEEPROM, relay2Connected));
relay3Connected = EEPROM.read(offsetof(storeInEEPROM, relay3Connected));
relay4Connected = EEPROM.read(offsetof(storeInEEPROM, relay4Connected));
relay5Connected = EEPROM.read(offsetof(storeInEEPROM, relay5Connected));
relay6Connected = EEPROM.read(offsetof(storeInEEPROM, relay6Connected));
funcRelay1 = EEPROM.read(offsetof(storeInEEPROM, funcRelay1));
funcRelay2 = EEPROM.read(offsetof(storeInEEPROM, funcRelay2));
funcRelay3 = EEPROM.read(offsetof(storeInEEPROM, funcRelay3));
funcRelay4 = EEPROM.read(offsetof(storeInEEPROM, funcRelay4));
funcRelay5 = EEPROM.read(offsetof(storeInEEPROM, funcRelay5));
funcRelay6 = EEPROM.read(offsetof(storeInEEPROM, funcRelay6));
fan1Connected = EEPROM.read(offsetof(storeInEEPROM, fan1Connected));
fan2Connected = EEPROM.read(offsetof(storeInEEPROM, fan2Connected));

PIDcontrol = EEPROM.read(offsetof(storeInEEPROM, PIDcontrol));

measurements = EEPROM.read(offsetof(storeInEEPROM, measurements));

humidMin1 = EEPROM.read(offsetof(storeInEEPROM, humidMin1));
humidMax1 =  EEPROM.read(offsetof(storeInEEPROM, humidMax1));
humidMin2 = EEPROM.read(offsetof(storeInEEPROM, humidMin2));
humidMax2 =  EEPROM.read(offsetof(storeInEEPROM, humidMax2));
// humidMin3 = EEPROM.read(offsetof(storeInEEPROM, humidMin3));
// humidMax3 =  EEPROM.read(offsetof(storeInEEPROM, humidMax3));

manualFanspeed1 = EEPROM.read(offsetof(storeInEEPROM, manualFanspeed1));
manualFanspeed2 = EEPROM.read(offsetof(storeInEEPROM, manualFanspeed2));

manualMosfet1 = EEPROM.read(offsetof(storeInEEPROM, manualMosfet1));
manualMosfet2 = EEPROM.read(offsetof(storeInEEPROM, manualMosfet2));

fan1NightSwitch = EEPROM.read(offsetof(storeInEEPROM, fan1NightSwitch));
fan2NightSwitch = EEPROM.read(offsetof(storeInEEPROM, fan2NightSwitch));

#ifdef ESP8266  

EEPROM.get(offsetof(storeInEEPROM, graphUpdate), graphUpdate;
EEPROM.get(offsetof(storeInEEPROM, tempUodate), tempUpdate;

EEPROM.get(offsetof(storeInEEPROM, KP, KP);
EEPROM.get(offsetof(storeInEEPROM, KI), KI);
EEPROM.get(offsetof(storeInEEPROM, KD), KD);

EEPROM.get(offsetof(storeInEEPROM, OUTPUT_MIN1), OUTPUT_MIN1);
EEPROM.get(offsetof(storeInEEPROM, OUTPUT_MAX1), OUTPUT_MAX1);
EEPROM.get(offsetof(storeInEEPROM, OUTPUT_MIN2), OUTPUT_MIN2);
EEPROM.get(offsetof(storeInEEPROM, OUTPUT_MAX2), OUTPUT_MAX2);

EEPROM.get(offsetof(storeInEEPROM, daySoilTemp1), daySoilTemp1;
EEPROM.get(offsetof(storeInEEPROM, nightSoilTemp1), nightSoilTemp1;
EEPROM.get(offsetof(storeInEEPROM, targetAirTemp1), targetAirTemp1;
EEPROM.get(offsetof(storeInEEPROM, tempRange1), tempRange1;
EEPROM.get(offsetof(storeInEEPROM, alarmRange1), alarmRange1;

EEPROM.get(offsetof(storeInEEPROM, daySoilTemp2), daySoilTemp2;
EEPROM.get(offsetof(storeInEEPROM, nightSoilTemp2), nightSoilTemp2;
EEPROM.get(offsetof(storeInEEPROM, targetAirTemp2, targetAirTemp2;
EEPROM.get(offsetof(storeInEEPROM, tempRange2), tempRange2;
EEPROM.get(offsetof(storeInEEPROM, alarmRange2), alarmRange2;

EEPROM.get(offsetof(storeInEEPROM, daySoilTemp3), daySoilTemp3;
EEPROM.get(offsetof(storeInEEPROM, nightSoilTemp3), nightSoilTemp3;
EEPROM.get(offsetof(storeInEEPROM, tempRange3), tempRange3;
EEPROM.get(offsetof(storeInEEPROM, alarmRange3), alarmRange3;

EEPROM.get(offsetof(storeInEEPROM, daySoilTemp4), daySoilTemp4;
EEPROM.get(offsetof(storeInEEPROM, nightSoilTemp4), nightSoilTemp4;
EEPROM.get(offsetof(storeInEEPROM, tempRange4), tempRange4;
EEPROM.get(offsetof(storeInEEPROM, alarmRange4), alarmRange4;

for (int m = 0; m < 8; m++){
int offsetPosition = offsetof(storeInEEPROM, calibrationValue[0]);
EEPROM.get(offsetPosition+(4*m), calibrationValue[m]);
}

#else
graphUpdate = EEPROM.readInt(offsetof(storeInEEPROM, graphUpdate));
tempUpdate = EEPROM.readInt(offsetof(storeInEEPROM, tempUpdate));

daySoilTemp1 = EEPROM.readFloat(offsetof(storeInEEPROM, daySoilTemp1));
nightSoilTemp1 = EEPROM.readFloat(offsetof(storeInEEPROM, nightSoilTemp1));
targetAirTemp1 = EEPROM.readFloat(offsetof(storeInEEPROM, targetAirTemp1));
tempRange1 = EEPROM.readFloat(offsetof(storeInEEPROM, tempRange1));
alarmRange1 = EEPROM.readFloat(offsetof(storeInEEPROM, alarmRange1));

daySoilTemp2 = EEPROM.readFloat(offsetof(storeInEEPROM, daySoilTemp2));
nightSoilTemp2 = EEPROM.readFloat(offsetof(storeInEEPROM, nightSoilTemp2));
targetAirTemp2 = EEPROM.readFloat(offsetof(storeInEEPROM, targetAirTemp2));
tempRange2 = EEPROM.readFloat(offsetof(storeInEEPROM, tempRange2));
alarmRange2 = EEPROM.readFloat(offsetof(storeInEEPROM, alarmRange2));

daySoilTemp3 = EEPROM.readFloat(offsetof(storeInEEPROM, daySoilTemp3));
nightSoilTemp3 = EEPROM.readFloat(offsetof(storeInEEPROM, nightSoilTemp3));
tempRange3 = EEPROM.readFloat(offsetof(storeInEEPROM, tempRange3));
alarmRange3 = EEPROM.readFloat(offsetof(storeInEEPROM, alarmRange3));

daySoilTemp4 = EEPROM.readFloat(offsetof(storeInEEPROM, daySoilTemp4));
nightSoilTemp4 = EEPROM.readFloat(offsetof(storeInEEPROM, nightSoilTemp4));
tempRange4 = EEPROM.readFloat(offsetof(storeInEEPROM, tempRange4));
alarmRange4 = EEPROM.readFloat(offsetof(storeInEEPROM, alarmRange4));

KP = EEPROM.readDouble(offsetof(storeInEEPROM, KP));
KI = EEPROM.readDouble(offsetof(storeInEEPROM, KI));
KD = EEPROM.readDouble(offsetof(storeInEEPROM, KD));

OUTPUT_MIN1 = EEPROM.readDouble(offsetof(storeInEEPROM, OUTPUT_MIN1));
OUTPUT_MAX1 = EEPROM.readDouble(offsetof(storeInEEPROM, OUTPUT_MAX1));
OUTPUT_MIN2 = EEPROM.readDouble(offsetof(storeInEEPROM, OUTPUT_MIN2));
OUTPUT_MAX2 = EEPROM.readDouble(offsetof(storeInEEPROM, OUTPUT_MAX2));

for (int m = 0; m < 8; m++){
int offsetPosition = offsetof(storeInEEPROM, calibrationValue[0]);
calibrationValue[m]  = EEPROM.readFloat(offsetPosition+4*m);
}
#endif

targetSoilTemp1 = daySoilTemp1;
targetSoilTemp2 = daySoilTemp2;
targetSoilTemp3 = daySoilTemp3;
targetSoilTemp4 = daySoilTemp4;
// for (int m = 0; m < 8; m++){
// int offsetPosition = offsetof(storeInEEPROM, lightSchedule[0]);
// lightSchedule[m]  = EEPROM.readChar(offsetPosition+m);
// // Serial.println(lightSchedule[m]);
// }

//Serial.print("position: "); Serial.print(offsetPosition+4*m); Serial.print(" cal. value: "); Serial.println(calibrationValue[m]);

for (int m = 0; m < 32; m++){
int offsetPosition = offsetof(storeInEEPROM, ssidStorage[0]);
ssidStorage[m]  = EEPROM.read(offsetPosition+m);
}
wifiID = String(ssidStorage);
Serial.print("wifiID "); Serial.println(wifiID);

for (int m = 0; m < 32; m++){  
int offsetPosition = offsetof(storeInEEPROM, passStorage[0]);
passStorage[m]  = EEPROM.read(offsetPosition+m);
}
wifiPASS = String(passStorage);
Serial.print("wifiPASS "); Serial.println(wifiPASS);

for (int m = 0; m < 6; m++){
int offsetPosition = offsetof(storeInEEPROM, lights1on[0]);
lights1on[m]  = EEPROM.read(offsetPosition+m);
}
lights1ON = String(lights1on);
//Serial.print("lights1on "); Serial.println(lights1ON);

for (int m = 0; m < 6; m++){
int offsetPosition = offsetof(storeInEEPROM, lights1off[0]);
lights1off[m]  = EEPROM.read(offsetPosition+m);
}
lights1OFF = String(lights1off);
//Serial.print("lights1off "); Serial.println(lights1OFF);

for (int m = 0; m < 6; m++){
int offsetPosition = offsetof(storeInEEPROM, lights2on[0]);
lights2on[m]  = EEPROM.read(offsetPosition+m);
}
lights2ON = String(lights2on);
//Serial.print("lights2on "); Serial.println(lights2ON);

for (int m = 0; m < 6; m++){
int offsetPosition = offsetof(storeInEEPROM, lights2off[0]);
lights2off[m]  = EEPROM.read(offsetPosition+m);
}
lights2OFF = String(lights2off);
//Serial.print("lights2off "); Serial.println(lights2OFF);

for (int m = 0; m < 6; m++){
int offsetPosition = offsetof(storeInEEPROM, lights3on[0]);
lights3on[m]  = EEPROM.read(offsetPosition+m);
}
lights3ON = String(lights3on);
//Serial.print("lights2on "); Serial.println(lights2ON);

for (int m = 0; m < 6; m++){
int offsetPosition = offsetof(storeInEEPROM, lights3off[0]);
lights3off[m]  = EEPROM.read(offsetPosition+m);
}
lights3OFF = String(lights3off);

reInitializeTimeInts();

}


int convertStringToInt(String str, int workflow){
  int str_len = str.length() + 1;
  char char_array[str_len];
  str.toCharArray(char_array, str_len);
  
  char buffer1[3];

  switch (workflow){
    case 0:
      buffer1[0]=char_array[0];
      buffer1[1]=char_array[1];
      buffer1[2]='\0';
    break;
    case 1:
      buffer1[0]=char_array[3];
      buffer1[1]=char_array[4];
      buffer1[2]='\0';
    break;
  }
  return atoi(buffer1);
}


void reInitializeTimeStrings(){
    for (int m = 0; m < 6; m++){
    int offsetPosition = offsetof(storeInEEPROM, lights1on[0]);
    lights1on[m]  = EEPROM.read(offsetPosition+m);
    }
    lights1ON = String(lights1on);

    for (int m = 0; m < 6; m++){
    int offsetPosition = offsetof(storeInEEPROM, lights1off[0]);
    lights1off[m]  = EEPROM.read(offsetPosition+m);
    }
    lights1OFF = String(lights1off);

    for (int m = 0; m < 6; m++){
    int offsetPosition = offsetof(storeInEEPROM, lights2on[0]);
    lights2on[m]  = EEPROM.read(offsetPosition+m);
    }
    lights2ON = String(lights2on);

    for (int m = 0; m < 6; m++){
    int offsetPosition = offsetof(storeInEEPROM, lights2off[0]);
    lights2off[m]  = EEPROM.read(offsetPosition+m);
    }
    lights2OFF = String(lights2off);

    for (int m = 0; m < 6; m++){
    int offsetPosition = offsetof(storeInEEPROM, lights3on[0]);
    lights3on[m]  = EEPROM.read(offsetPosition+m);
    }
    lights3ON = String(lights3on);
    //Serial.print("lights2on "); Serial.println(lights2ON);

    for (int m = 0; m < 6; m++){
    int offsetPosition = offsetof(storeInEEPROM, lights3off[0]);
    lights3off[m]  = EEPROM.read(offsetPosition+m);
    }
    lights3OFF = String(lights3off);    

    reInitializeTimeInts();

    return;
}

void reInitializeTimeInts(){
    hoursOn1 = convertStringToInt(lights1ON, 0);
    minutesOn1 = convertStringToInt(lights1ON, 1);
    hoursOff1 = convertStringToInt(lights1OFF, 0);
    minutesOff1 = convertStringToInt(lights1OFF, 1);
    currentMinutes = ((hours*60)+minutes);

    minutesLights1On = ((hoursOn1*60)+minutesOn1);
    minutesLights1Off = ((hoursOff1*60)+minutesOff1);

    hoursOn2 = convertStringToInt(lights2ON, 0);
    minutesOn2 = convertStringToInt(lights2ON, 1);
    hoursOff2 = convertStringToInt(lights2OFF, 0);
    minutesOff2 = convertStringToInt(lights2OFF, 1);
 
    minutesLights2On = ((hoursOn2*60)+minutesOn2);
    minutesLights2Off = ((hoursOff2*60)+minutesOff2);

    hoursOn3 = convertStringToInt(lights3ON, 0);
    minutesOn3 = convertStringToInt(lights3ON, 1);
    hoursOff3 = convertStringToInt(lights3OFF, 0);
    minutesOff3 = convertStringToInt(lights3OFF, 1);
 
    minutesLights3On = ((hoursOn2*60)+minutesOn3);
    minutesLights3Off = ((hoursOff2*60)+minutesOff3);    
}

void syncTimeRTC(){ // This function syncs RTC timer upon connection to wifi in case lostPower is true.

      if (rtc.lostPower()) {
        Serial.println("RTC lost power, lets set the time!");

        configTime(3600, 3600, ntpServer);
        // following line sets the RTC to the date &amp; time this sketch was compiled
        //rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

        struct tm time;
  
        if(!getLocalTime(&time)){
          Serial.println("Could not obtain time info");
          return;
        }
      
        Serial.println("\n---------TIME----------");
        
        Serial.println(asctime(&time));

        rtc.adjust(DateTime(time.tm_year, time.tm_mon, time.tm_mday, time.tm_hour, time.tm_min, time.tm_sec));
      }

}