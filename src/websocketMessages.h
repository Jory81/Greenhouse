// String processor(const String &var) {
//     return String(var == "STATE" && relay1.on ? "on" : "off");
// }

void onRootRequest(AsyncWebServerRequest *request) {
  request->send(SPIFFS, "/index.html", "text/html");    // , false, processor
}

void initWebServer() {
    server.on("/", onRootRequest);
    server.serveStatic("/", SPIFFS, "/");
    server.begin();
}
             
void onEvent(AsyncWebSocket       *server,
             AsyncWebSocketClient *client,
             AwsEventType          type,
             void                 *arg,
             uint8_t              *data,
             size_t                len) {

    switch (type) {
        case WS_EVT_CONNECT:
            Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
            break;
        case WS_EVT_DISCONNECT:
            Serial.printf("WebSocket client #%u disconnected\n", client->id());
            break;
        case WS_EVT_DATA:
            handleWebSocketMessage(arg, data, len);
        case WS_EVT_PONG:
        case WS_EVT_ERROR:
            break;
    }
}

void initWebSocket() {
    ws.onEvent(onEvent);
    server.addHandler(&ws);
}

void notifyClientsSingleString(String object, String &message) {
        // String objectString = object;
    //const uint8_t size = JSON_OBJECT_SIZE(1);
    StaticJsonDocument<50> doc;
    String key = object;
    String Value = message;
    doc[key] = Value; 

    //serializeJson(doc, Serial);

    char data[50];
    size_t len = serializeJson(doc, data);
        for (int i = 0; i < len;  i++){
      //Serial.print(data[i]);
    }
    //Serial.println(" ");
    ws.textAll(data, len); 

}

void notifyClientsSingleObject(String object, boolean value) {
    // String objectString = object;
    //const uint8_t size = JSON_OBJECT_SIZE(1);
    StaticJsonDocument<50> doc;
    String key = object;
    boolean Value = value;
    doc[key] = Value; 

    //serializeJson(doc, Serial);

    char data[50];
    size_t len = serializeJson(doc, data);
        for (int i = 0; i < len;  i++){
      //Serial.print(data[i]);
    }
    //Serial.println(" ");
    ws.textAll(data, len); 
}

void notifyClients() {
    // const uint8_t size = JSON_OBJECT_SIZE(1);
    // StaticJsonDocument<size> json;
    // json["relay1"] = relay_temp1.on ? "on" : "off";

    // char buffer[17];
    // size_t len = serializeJson(json, buffer);
    // ws.textAll(buffer, len);
}

void handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
    AwsFrameInfo *info = (AwsFrameInfo*)arg;
    if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {

        // Serial.println("received text ");
        // for (int i = 0; i < len; i++){
        // strval[i]=(char)data[i];    
        // Serial.print((char)data[i]);
        // }
        // Serial.println(" ");

        //const uint8_t size = JSON_OBJECT_SIZE(1);
        StaticJsonDocument<200> json;
        DeserializationError err = deserializeJson(json, data);
        if (err) {
            Serial.print(F("deserializeJson() failed with code "));
            Serial.println(err.c_str());
            return;
        }
        //serializeJson(json, Serial);
        //uint8_t numberOfArguments = json.size();
        //Serial.println(numberOfArguments);
        //for (int i = 0; i < numberOfArguments; i++){
        if (json.containsKey("requestInfo")){sendProgramInfo();}
        else if (json.containsKey("hours")){if (initializeTimeOnOpen){hourStart = json["hours"]; minutesStart = json["minutes"]; initializeTimeOnOpen = false;};}
        else if (json.containsKey("saveInEEPROM")){saveInEEPROM = json["saveInEEPROM"];}   
        else if (json.containsKey("ssid")){String wifiID = json["ssid"]; Serial.println(wifiID); EEPROMposition = offsetof(storeInEEPROM, ssidStorage[0]); writeStringToEEPROM(EEPROMposition, wifiID); notifyClientsSingleString("wifiID", wifiID);}
        else if (json.containsKey("pass")){String wifiPASS = json["pass"]; Serial.println(wifiPASS); EEPROMposition = offsetof(storeInEEPROM, passStorage[0]); writeStringToEEPROM(EEPROMposition, wifiPASS); notifyClientsSingleString("wifiPASS", wifiPASS);}
        // THESE SYSTEM VALUES ARE ALWAYS SAVED TO EEPROM
        else if (json.containsKey("probeTypeT")){probeTypeT = json["probeTypeT"]; EEPROM.put(offsetof(storeInEEPROM, probeTypeT), probeTypeT);  EEPROM.commit(); }
        else if (json.containsKey("probeCountT")){probeCountT = json["probeCountT"]; EEPROM.put(offsetof(storeInEEPROM, probeCountT), probeCountT);  EEPROM.commit();}
        else if (json.containsKey("probeTypeH")){probeTypeH = json["probeTypeH"]; EEPROM.put(offsetof(storeInEEPROM, probeTypeH), probeTypeH);  EEPROM.commit();}
        else if (json.containsKey("probeCountH")){probeCountH = json["probeCountH"]; EEPROM.put(offsetof(storeInEEPROM, probeCountH), probeCountH);  EEPROM.commit();}
        else if (json.containsKey("relay1Connected")){relay1Connected = json["relay1Connected"]; EEPROM.put(offsetof(storeInEEPROM, relay1Connected), relay1Connected);  EEPROM.commit();}
        else if (json.containsKey("relay2Connected")){relay2Connected = json["relay2Connected"]; EEPROM.put(offsetof(storeInEEPROM, relay2Connected), relay2Connected);  EEPROM.commit();}
        else if (json.containsKey("relay3Connected")){relay3Connected = json["relay3Connected"]; EEPROM.put(offsetof(storeInEEPROM, relay3Connected), relay3Connected);  EEPROM.commit();}
        else if (json.containsKey("relay4Connected")){relay4Connected = json["relay4Connected"]; EEPROM.put(offsetof(storeInEEPROM, relay4Connected), relay4Connected);  EEPROM.commit();}
        else if (json.containsKey("relay5Connected")){relay5Connected = json["relay5Connected"]; EEPROM.put(offsetof(storeInEEPROM, relay5Connected), relay5Connected);  EEPROM.commit();}
        else if (json.containsKey("relay6Connected")){relay6Connected = json["relay6Connected"]; EEPROM.put(offsetof(storeInEEPROM, relay6Connected), relay6Connected);  EEPROM.commit();}
        else if (json.containsKey("fan1Connected")){fan1Connected = json["fan1Connected"]; EEPROM.put(offsetof(storeInEEPROM, fan1Connected), fan1Connected);  EEPROM.commit();}
        else if (json.containsKey("fan2Connected")){fan2Connected = json["fan2Connected"]; EEPROM.put(offsetof(storeInEEPROM, fan2Connected), fan2Connected);  EEPROM.commit();}
        else if (json.containsKey("graphUpdate")){graphUpdate = json["graphUpdate"]; graphUpdate = graphUpdate*1000; EEPROM.put(offsetof(storeInEEPROM, graphUpdate), (graphUpdate/1000));  EEPROM.commit();}
        else if (json.containsKey("PIDcontrol")){PIDcontrol = json["PIDcontrol"]; EEPROM.put(offsetof(storeInEEPROM, PIDcontrol), PIDcontrol);  EEPROM.commit();}
        
        else if (json.containsKey("lights1ON")){String StoreLights1ON = json["lights1ON"]; String StoreLights1OFF = json["lights1OFF"]; EEPROMposition = offsetof(storeInEEPROM, lights1on[0]); writeStringToEEPROM(EEPROMposition, StoreLights1ON); EEPROMposition = offsetof(storeInEEPROM, lights1off[0]); writeStringToEEPROM(EEPROMposition, StoreLights1OFF); reInitializeTimeStrings();}
        else if (json.containsKey("lights2ON")){String StoreLights2ON = json["lights2ON"]; String StoreLights2OFF = json["lights2OFF"]; EEPROMposition = offsetof(storeInEEPROM, lights2on[0]); writeStringToEEPROM(EEPROMposition, StoreLights2ON); EEPROMposition = offsetof(storeInEEPROM, lights2off[0]); writeStringToEEPROM(EEPROMposition, StoreLights2OFF); reInitializeTimeStrings();}
        // THESE VALUES ARE ONLY PERMANENTLT SWITCHED WHEN EEPROM BUTTON IS CHECKED
        else if (json.containsKey("calValue1")){calibrationValue[0] = json["calValue1"]; if (saveInEEPROM){EEPROM.put(offsetof(storeInEEPROM, calibrationValue[0]), calibrationValue[0]);  EEPROM.commit(); };}
        else if (json.containsKey("calValue2")){calibrationValue[1] = json["calValue2"]; if (saveInEEPROM){EEPROM.put(offsetof(storeInEEPROM, calibrationValue[1]), calibrationValue[1]);  EEPROM.commit(); };}
        else if (json.containsKey("calValue3")){calibrationValue[2] = json["calValue3"]; if (saveInEEPROM){EEPROM.put(offsetof(storeInEEPROM, calibrationValue[2]), calibrationValue[2]);  EEPROM.commit(); };}
        else if (json.containsKey("calValue4")){calibrationValue[3] = json["calValue4"]; if (saveInEEPROM){EEPROM.put(offsetof(storeInEEPROM, calibrationValue[3]), calibrationValue[3]);  EEPROM.commit(); };}
        else if (json.containsKey("calValue5")){calibrationValue[4] = json["calValue5"]; if (saveInEEPROM){EEPROM.put(offsetof(storeInEEPROM, calibrationValue[4]), calibrationValue[4]);  EEPROM.commit(); };}
        else if (json.containsKey("calValue6")){calibrationValue[5] = json["calValue6"]; if (saveInEEPROM){EEPROM.put(offsetof(storeInEEPROM, calibrationValue[5]), calibrationValue[5]);  EEPROM.commit(); };}
        else if (json.containsKey("calValue7")){calibrationValue[6] = json["calValue7"]; if (saveInEEPROM){EEPROM.put(offsetof(storeInEEPROM, calibrationValue[6]), calibrationValue[6]);  EEPROM.commit(); };}
        else if (json.containsKey("calValue8")){calibrationValue[7] = json["calValue8"]; if (saveInEEPROM){EEPROM.put(offsetof(storeInEEPROM, calibrationValue[7]), calibrationValue[7]);  EEPROM.commit(); };}
        else if (json.containsKey("calValue9")){calibrationValue[8] = json["calValue9"]; if (saveInEEPROM){EEPROM.put(offsetof(storeInEEPROM, calibrationValue[8]), calibrationValue[8]);  EEPROM.commit(); };}
        else if (json.containsKey("calValue10")){calibrationValue[9] = json["calValue10"]; if (saveInEEPROM){EEPROM.put(offsetof(storeInEEPROM, calibrationValue[9]), calibrationValue[9]);  EEPROM.commit(); };}
        else if (json.containsKey("calValue11")){calibrationValue[10] = json["calValue11"]; if (saveInEEPROM){EEPROM.put(offsetof(storeInEEPROM, calibrationValue[10]), calibrationValue[10]);  EEPROM.commit(); };}

        else if (json.containsKey("manualRelay1")){manualRelay1 = json["manualRelay1"];}
        else if (json.containsKey("manualRelay2")){manualRelay2 = json["manualRelay2"];}
        else if (json.containsKey("manualRelay3")){manualRelay3 = json["manualRelay3"];}
        else if (json.containsKey("manualRelay4")){manualRelay4 = json["manualRelay4"];}
        else if (json.containsKey("manualRelay5")){manualRelay5 = json["manualRelay5"];}
        else if (json.containsKey("manualRelay6")){manualRelay6 = json["manualRelay6"];}

        else if (json.containsKey("modeRelay1")){modeRelay1 = json["modeRelay1"]; EEPROM.put(offsetof(storeInEEPROM, modeRelay1), modeRelay1);  EEPROM.commit();}
        else if (json.containsKey("modeRelay2")){modeRelay2 = json["modeRelay2"]; EEPROM.put(offsetof(storeInEEPROM, modeRelay2), modeRelay2);  EEPROM.commit();}
        else if (json.containsKey("modeRelay3")){modeRelay3 = json["modeRelay3"]; EEPROM.put(offsetof(storeInEEPROM, modeRelay3), modeRelay3);  EEPROM.commit();}
        else if (json.containsKey("modeRelay4")){modeRelay4 = json["modeRelay4"]; EEPROM.put(offsetof(storeInEEPROM, modeRelay4), modeRelay4);  EEPROM.commit();}
        else if (json.containsKey("modeRelay5")){modeRelay5 = json["modeRelay5"]; EEPROM.put(offsetof(storeInEEPROM, modeRelay5), modeRelay5);  EEPROM.commit();}
        else if (json.containsKey("modeRelay6")){modeRelay6 = json["modeRelay6"]; EEPROM.put(offsetof(storeInEEPROM, modeRelay6), modeRelay6);  EEPROM.commit();}

        else if (json.containsKey("lights1")){lights1 = json["lights1"];} //lightState1 = !lights1;
        else if (json.containsKey("heater1")){heater1 = json["heater1"]; }// heaterState1 = !heater1;//  Serial.print("heaterState is now "); Serial.print(heaterState1); Serial.print(" And heater is: "); Serial.println(heater1);
        else if (json.containsKey("fan1")){fan1 = json["fan1"]; fanState1 = !fan1;}
        else if (json.containsKey("humidifier1")){humidifier1 = json["humidifier1"]; } // humidifierState1 = !humidifier1;

        else if (json.containsKey("lights2")){lights2 = json["lights2"]; } // lightState2 = !lights2;}
        else if (json.containsKey("heater2")){heater2 = json["heater2"]; }// heaterState2 = !heater2;// Serial.print("heaterState is now "); Serial.print(heaterState1); Serial.print(" And heater is: "); Serial.println(heater1);
        else if (json.containsKey("fan2")){fan2 = json["fan2"]; fanState2 = !fan2;}
        else if (json.containsKey("humidifier2")){humidifier2 = json["humidifier2"]; } // humidifierState2 = !humidifier2;

        else if (json.containsKey("lights3")){lights3 = json["lights3"]; } // lightState2 = !lights2;}
        else if (json.containsKey("heater3")){heater3 = json["heater3"]; }// heaterState2 = !heater2;// Serial.print("heaterState is now "); Serial.print(heaterState1); Serial.print(" And heater is: "); Serial.println(heater1);
        //else if (json.containsKey("fan2")){fan2 = json["fan2"]; fanState2 = !fan2;}
        else if (json.containsKey("humidifier3")){humidifier3 = json["humidifier3"]; } // humidifierState2 = !humidifier2;

        else if (json.containsKey("manualMosfet1")){manualMosfet1 = json["manualMosfet1"]; if (saveInEEPROM){EEPROM.put(offsetof(storeInEEPROM, manualMosfet1), manualMosfet1);  EEPROM.commit(); };}
        else if (json.containsKey("manualFanspeed1")){manualFanspeed1 = json["manualFanspeed1"]; if (saveInEEPROM){EEPROM.put(offsetof(storeInEEPROM, manualFanspeed1), manualFanspeed1);  EEPROM.commit(); };}
        else if (json.containsKey("manualMosfet2")){manualMosfet2 = json["manualMosfet2"]; if (saveInEEPROM){EEPROM.put(offsetof(storeInEEPROM, manualMosfet2), manualMosfet2);  EEPROM.commit(); };}
        else if (json.containsKey("manualFanspeed2")){manualFanspeed2 = json["manualFanspeed2"]; if (saveInEEPROM){EEPROM.put(offsetof(storeInEEPROM, manualFanspeed2), manualFanspeed2);  EEPROM.commit(); };}

        else if (json.containsKey("OUTPUT_MIN1")){OUTPUT_MIN1 = json["OUTPUT_MIN1"]; if (saveInEEPROM){EEPROM.put(offsetof(storeInEEPROM, OUTPUT_MIN1), OUTPUT_MIN1);  EEPROM.commit(); };}
        else if (json.containsKey("OUTPUT_MAX1")){OUTPUT_MAX1 = json["OUTPUT_MAX1"]; if (saveInEEPROM){EEPROM.put(offsetof(storeInEEPROM, OUTPUT_MAX1), OUTPUT_MAX1);  EEPROM.commit(); };}
        else if (json.containsKey("OUTPUT_MIN2")){OUTPUT_MIN2 = json["OUTPUT_MIN2"]; if (saveInEEPROM){EEPROM.put(offsetof(storeInEEPROM, OUTPUT_MIN2), OUTPUT_MIN2);  EEPROM.commit(); };}
        else if (json.containsKey("OUTPUT_MAX2")){OUTPUT_MAX2 = json["OUTPUT_MAX2"]; if (saveInEEPROM){EEPROM.put(offsetof(storeInEEPROM, OUTPUT_MAX2), OUTPUT_MAX2);  EEPROM.commit(); };}

        else if (json.containsKey("KP")){KP = json["KP"]; if (saveInEEPROM){EEPROM.put(offsetof(storeInEEPROM, KP), KP);  EEPROM.commit(); };}
        else if (json.containsKey("KI")){KI = json["KI"]; if (saveInEEPROM){EEPROM.put(offsetof(storeInEEPROM, KI), KI);  EEPROM.commit(); };}
        else if (json.containsKey("KD")){KD = json["KD"]; if (saveInEEPROM){EEPROM.put(offsetof(storeInEEPROM, KD), KD);  EEPROM.commit(); };}

        else if (json.containsKey("humidMin1")){humidMin1 = json["humidMin1"]; if (saveInEEPROM){EEPROM.put(offsetof(storeInEEPROM, humidMin1), humidMin1);  EEPROM.commit(); };}
        else if (json.containsKey("humidMax1")){humidMax1 = json["humidMax1"]; if (saveInEEPROM){EEPROM.put(offsetof(storeInEEPROM, humidMax1), humidMax1);  EEPROM.commit(); };}
        else if (json.containsKey("humidMin2")){humidMin2 = json["humidMin2"]; if (saveInEEPROM){EEPROM.put(offsetof(storeInEEPROM, humidMin2), humidMin2);  EEPROM.commit(); };}
        else if (json.containsKey("humidMax2")){humidMax2 = json["humidMax2"]; if (saveInEEPROM){EEPROM.put(offsetof(storeInEEPROM, humidMax2), humidMax2);  EEPROM.commit(); };}
        else if (json.containsKey("humidMin3")){humidMin3 = json["humidMin3"]; if (saveInEEPROM){EEPROM.put(offsetof(storeInEEPROM, humidMin3), humidMin3);  EEPROM.commit(); };}
        else if (json.containsKey("humidMax3")){humidMax3 = json["humidMax3"]; if (saveInEEPROM){EEPROM.put(offsetof(storeInEEPROM, humidMax3), humidMax3);  EEPROM.commit(); };}        

        else if (json.containsKey("daySoilTemp1")){daySoilTemp1 = json["daySoilTemp1"]; if (saveInEEPROM){EEPROM.put(offsetof(storeInEEPROM, daySoilTemp1), daySoilTemp1);  EEPROM.commit(); };}
        else if (json.containsKey("nightSoilTemp1")){nightSoilTemp1 = json["nightSoilTemp1"]; if (saveInEEPROM){EEPROM.put(offsetof(storeInEEPROM, nightSoilTemp1), nightSoilTemp1);  EEPROM.commit(); };}
        else if (json.containsKey("tempRange1")){tempRange1 = json["tempRange1"]; if (saveInEEPROM){EEPROM.put(offsetof(storeInEEPROM, tempRange1), tempRange1);  EEPROM.commit(); };}
        else if (json.containsKey("alarmRange1")){alarmRange1 = json["alarmRange1"]; if (saveInEEPROM){EEPROM.put(offsetof(storeInEEPROM, alarmRange1), alarmRange1);  EEPROM.commit(); };}
        else if (json.containsKey("soilAlarm1")){soilAlarm1 = json["soilAlarm1"];}
        else if (json.containsKey("targetAirTemp1")){targetAirTemp1 = json["targetAirTemp1"]; if (saveInEEPROM){EEPROM.put(offsetof(storeInEEPROM, targetAirTemp1), targetAirTemp1);  EEPROM.commit(); };}
        else if (json.containsKey("airAlarm1")){airAlarm1 = json["airAlarm1"];}

        else if (json.containsKey("daySoilTemp2")){daySoilTemp2 = json["daySoilTemp2"]; if (saveInEEPROM){EEPROM.put(offsetof(storeInEEPROM, daySoilTemp2), daySoilTemp2);  EEPROM.commit(); };}
        else if (json.containsKey("nightSoilTemp2")){nightSoilTemp2 = json["nightSoilTemp2"]; if (saveInEEPROM){EEPROM.put(offsetof(storeInEEPROM, nightSoilTemp2), nightSoilTemp2);  EEPROM.commit(); };}        
        else if (json.containsKey("tempRange2")){tempRange2 = json["tempRange2"]; if (saveInEEPROM){EEPROM.put(offsetof(storeInEEPROM, tempRange2), tempRange2);  EEPROM.commit(); };}
        else if (json.containsKey("alarmRange2")){alarmRange2 = json["alarmRange2"]; if (saveInEEPROM){EEPROM.put(offsetof(storeInEEPROM, alarmRange2), alarmRange2);  EEPROM.commit(); };}
        else if (json.containsKey("soilAlarm2")){soilAlarm2 = json["soilAlarm2"];}
        else if (json.containsKey("targetAirTemp2")){targetAirTemp2 = json["targetAirTemp2"]; if (saveInEEPROM){EEPROM.put(offsetof(storeInEEPROM, targetAirTemp2), targetAirTemp2);  EEPROM.commit(); };}
        else if (json.containsKey("airAlarm2")){airAlarm2 = json["airAlarm2"];}

        else if (json.containsKey("daySoilTemp3")){daySoilTemp3 = json["daySoilTemp3"]; if (saveInEEPROM){EEPROM.put(offsetof(storeInEEPROM, daySoilTemp3), daySoilTemp3);  EEPROM.commit(); };}
        else if (json.containsKey("nightSoilTemp3")){nightSoilTemp3 = json["nightSoilTemp3"]; if (saveInEEPROM){EEPROM.put(offsetof(storeInEEPROM, nightSoilTemp3), nightSoilTemp3);  EEPROM.commit(); };}        
        else if (json.containsKey("tempRange3")){tempRange3 = json["tempRange3"]; if (saveInEEPROM){EEPROM.put(offsetof(storeInEEPROM, tempRange3), tempRange3);  EEPROM.commit(); };}
        else if (json.containsKey("alarmRange3")){alarmRange3 = json["alarmRange3"]; if (saveInEEPROM){EEPROM.put(offsetof(storeInEEPROM, alarmRange3), alarmRange3);  EEPROM.commit(); };}
        else if (json.containsKey("soilAlarm3")){soilAlarm3 = json["soilAlarm3"];}
        else if (json.containsKey("targetAirTemp3")){targetAirTemp3 = json["targetAirTemp3"]; if (saveInEEPROM){EEPROM.put(offsetof(storeInEEPROM, targetAirTemp3), targetAirTemp3);  EEPROM.commit(); };}
        else if (json.containsKey("airAlarm3")){airAlarm3 = json["airAlarm3"];}        
        else if (json.containsKey("Reboot")){ESP.restart();}
        notifyClientsSingleObject("recMsg", true);
    }
}  

void sendProgramInfo(){
StaticJsonDocument<2500> doc;
doc["probeTypeT"] = probeTypeT;
doc["probeCountT"]   = probeCountT;
doc["probeTypeH"]   = probeTypeH;
doc["probeCountH"]   = probeCountH;
doc["relay1Connected"]   = relay1Connected;
doc["relay2Connected"]   = relay2Connected;
doc["relay3Connected"]   = relay3Connected;
doc["relay4Connected"]   = relay4Connected;
doc["relay5Connected"]   = relay5Connected;
doc["relay6Connected"]   = relay6Connected;
doc["fan1Connected"]   =     fan1Connected;
doc["fan2Connected"]   =     fan2Connected;
doc["graphUpdate"]   = graphUpdate/1000;
doc["PIDcontrol"]   = PIDcontrol;
doc["calValue1"]   = String(calibrationValue[0],2);
doc["calValue2"]   = String(calibrationValue[1],2);
doc["calValue3"]   = String(calibrationValue[2],2);
doc["calValue4"]   = String(calibrationValue[3],2);
doc["calValue5"]   = String(calibrationValue[4],2);
doc["calValue6"]   = String(calibrationValue[5],2);
doc["calValue7"]   = String(calibrationValue[6],2);
doc["calValue8"]   = String(calibrationValue[7],2);
doc["calValue9"]   = String(calibrationValue[8],2);
doc["calValue10"]   = String(calibrationValue[9],2);
doc["calValue11"]   = String(calibrationValue[10],2);
doc["manualRelay1"] = manualRelay1;
doc["manualRelay2"] = manualRelay2;
doc["manualRelay3"] = manualRelay3;
doc["manualRelay4"] = manualRelay4;
doc["manualRelay5"] = manualRelay5;
doc["manualRelay6"] = manualRelay6;
doc["modeRelay1"] = modeRelay1;
doc["modeRelay2"] = modeRelay2;
doc["modeRelay3"] = modeRelay3;
doc["modeRelay4"] = modeRelay4;
doc["modeRelay5"] = modeRelay5;
doc["modeRelay6"] = modeRelay6;
doc["OUTPUT_MIN1"]   = String(OUTPUT_MIN1,2);
doc["OUTPUT_MAX1"]   = String(OUTPUT_MAX1,2);
doc["OUTPUT_MIN2"]   = String(OUTPUT_MIN2,2);
doc["OUTPUT_MAX2"]   = String(OUTPUT_MAX2,2);
doc["KP"]   = String(KP,2); // may require more decimals
doc["KI"]   = String(KI,2); // may require more decimals
doc["KD"]   = String(KD,2); // may require more decimals
doc["lights1ON"] = lights1ON;
doc["lights1OFF"] = lights1OFF;
doc["lights2ON"] = lights2ON;
doc["lights2OFF"] = lights2OFF;
doc["lights3ON"] = lights3ON;
doc["lights3OFF"] = lights3OFF;
doc["humidMin1"] = humidMin1;
doc["humidMax1"] = humidMax1;
doc["humidMin2"] = humidMin2;
doc["humidMax2"] = humidMax2;
doc["humidMin3"] = humidMin3;
doc["humidMax3"] = humidMax3;
doc["daySoilTemp1"] = String(daySoilTemp1,2);
doc["nightSoilTemp1"] = String(nightSoilTemp1,2);
doc["tempRange1"] = String(tempRange1,2);
doc["alarmRange1"] = String(alarmRange1,2);
doc["soilAlarm1"] = soilAlarm1;
doc["targetAirTemp1"] = String(targetAirTemp1,2);
doc["airAlarm1"] = airAlarm1;
doc["daySoilTemp2"] = String(daySoilTemp2,2);
doc["nightSoilTemp2"] = String(nightSoilTemp2,2);
doc["tempRange2"] = String(tempRange2,2);
doc["alarmRange2"] = String(alarmRange2,2);
doc["soilAlarm2"] = soilAlarm2;
doc["targetAirTemp2"] = String(targetAirTemp2,2);
doc["airAlarm2"] = airAlarm2;
doc["daySoilTemp3"] = String(daySoilTemp3,2);
doc["nightSoilTemp3"] = String(nightSoilTemp3,2);
doc["tempRange3"] = String(tempRange3,2);
doc["alarmRange3"] = String(alarmRange3,2);
doc["soilAlarm3"] = soilAlarm3;
doc["targetAirTemp3"] = String(targetAirTemp3,2);
doc["airAlarm3"] = airAlarm2;
doc["manualMosfet1"] = manualMosfet1;
doc["manualFanspeed1"] = manualFanspeed1;
doc["manualMosfet2"] = manualMosfet2;
doc["manualFanspeed2"] = manualFanspeed2;
doc["lights1"] = lights1;
doc["heater1"] = heater1;
doc["fan1"] = fan1;
doc["humidifier1"] = humidifier1;
doc["lights2"] = lights2;
doc["heater2"] = heater2;
doc["fan2"] = fan2;
doc["humidifier2"] = humidifier2;
doc["lights3"] = lights3;
doc["heater3"] = heater3;
//doc["fan2"] = fan2;
doc["humidifier3"] = humidifier3;
doc["saveInEEPROM"] = saveInEEPROM;

char data[2500];
size_t len = serializeJson(doc, data);
    //Serial.print("length: "); Serial.println(len);
    for (int i = 0; i < len;  i++){
    //Serial.print(data[i]);
}
//Serial.println(" ");
ws.textAll(data, len);
return;
}

void writeStringToEEPROM(int addrOffset, const String &strToWrite){
  if (strToWrite == NULL){
      //Serial.println("this string is empty");
  }
  else{  
    byte length = strToWrite.length();
    //Serial.print("len "); Serial.println(len);
    //Serial.print("addr offset ");Serial.println(addrOffset);
    //EEPROM.write(addrOffset, len);
    for (int i = 0; i < length; i++){
        //EEPROM.write(addrOffset + 1 + i, strToWrite[i]);
        EEPROM.write(addrOffset + i, strToWrite[i]);
        //Serial.println(strToWrite[i]);
    }
    EEPROM.write((length+addrOffset),NULL);
    //EEPROM.write((length+addrOffset),'\0');
    EEPROM.commit();
  }
return;
} 


// String readStringFromEEPROM(int addrOffset)
// {
//   int newStrLen = EEPROM.read(addrOffset);
//   char data[newStrLen + 1];
//   for (int i = 0; i < newStrLen; i++)
//   {
//     data[i] = EEPROM.read(addrOffset + 1 + i);
//   }
//   data[newStrLen] = '\0'; // !!! NOTE !!! Remove the space between the slash "/" and "0" (I've added a space because otherwise there is a display bug)
//   return String(data);
// }

void sendTempToClient (){
    StaticJsonDocument<100> doc;
    doc["temperature1"] = String(temp[0],2);
    doc["temperature2"] = String(temp[1],2);
    doc["temperature3"] = String(temp[2],2);
    doc["temperature4"] = String(temp[3],2);

    char data[100];
    size_t len = serializeJson(doc, data);
    //Serial.print("length: "); Serial.println(len);
    for (int i = 0; i < len;  i++){
    //Serial.print(data[i]);
    }
    //Serial.println(" ");
    ws.textAll(data, len);
    return;
}

    // if (dhtTemp[0] != 0){doc["temperature6"] =  dhtTemp[0];}
    // if (dhtTemp[1] != 0){doc["temperature7"] =  dhtTemp[1];}
    // if (humidity[0] != 0){doc["humid1"] =  humidity[0];}
    // if (humidity[1] != 0){doc["humid2"] =  humidity[1];}

    // if (temp[0] != 0){doc["temperature1"] = temp[0];}
    // if (temp[1] != 0){doc["temperature2"] = temp[1];}
    // if (temp[2] != 0){doc["temperature3"] = temp[2];}
    // if (temp[3] != 0){doc["temperature4"] = temp[3];}

void sendHumidityToClient(){
    StaticJsonDocument<200> doc;
    doc["temperature6"] =  String(dhtTemp[0],2);
    doc["temperature7"] =  String(dhtTemp[1],2);
    doc["temperature8"] =  String(dhtTemp[2],2);
    doc["humid1"] =  String(humidity[0],2);
    doc["humid2"] =  String(humidity[1],2);
    doc["humid3"] =  String(humidity[2],2);

    char data[200];
    size_t len = serializeJson(doc, data);
    //Serial.print("length: "); Serial.println(len);
    for (int i = 0; i < len;  i++){
    //Serial.print(data[i]);
    }
    //Serial.println(" ");
    ws.textAll(data, len);
    return;
}

void updateGraph (){
    if (probeCountT > 2){
        notifyClientsSingleObject("updateGraph", true);
    }
    else {
        notifyClientsSingleObject("updateGraph", false);
    }                
}


// void updateGraph2 (float temp){
//             String mergedString = "SD"+String(temp);
//             ws.textAll(mergedString);                
// }
