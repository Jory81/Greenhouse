void executeTask(byte function, boolean manualRelay, const int relayPin){
  switch (function){
    case 0: noFunction(manualRelay, relayPin); break; // do nothing - no function attached
    case 1: heater1Control(manualRelay, relayPin); break;
    case 2: heater2Control(manualRelay, relayPin); break;
    case 3: heater3Control(manualRelay, relayPin); break;
    case 4: heater4Control(manualRelay, relayPin); break;
    case 5: light1Control(manualRelay, relayPin); break;
    case 6: light2Control(manualRelay, relayPin); break;
    case 7: light3Control(manualRelay, relayPin); break;
    case 8: humidity1Control(manualRelay, relayPin); break;
    case 9: humidity2Control(manualRelay, relayPin); break;
    case 10: DHTsensors(manualRelay, relayPin); break;
    // case 10: humidity3Control(manualRelay, relayPin); break;
    default:    break;
    return;
}
}


void noFunction (boolean manualRelay, const int relayPin){
  bool relayReg = !(*portOutputRegister( digitalPinToPort(relayPin) ) & digitalPinToBitMask(relayPin));
      if (manualRelay != relayReg){ // checks if manualRelay is not equal to relay output. If it's not: if manualRelay is true turn On Relay, if false, turn Off relay.
        digitalWrite(relayPin, !manualRelay);
      }
}


void light1Control(boolean manualRelay, const int relayPin){
  if (!manualRelay){
    if (currentMinutes > minutesLights1On && currentMinutes < minutesLights1Off){
      lights1=true;
      targetSoilTemp1=daySoilTemp1;
      if (fan1NightSwitch){
        manualFanspeed1 = OUTPUT_MIN1;
      }
      else {
        manualFanspeed1 = EEPROM.read(offsetof(storeInEEPROM, manualFanspeed1));
      }
    }
    else if (currentMinutes < minutesLights1On || currentMinutes > minutesLights1On){
      lights1=false;
      targetSoilTemp1=nightSoilTemp1;
      manualFanspeed1 = EEPROM.read(offsetof(storeInEEPROM, manualFanspeed1));
    }
  }
  bool relayReg = !(*portOutputRegister( digitalPinToPort(relayPin) ) & digitalPinToBitMask(relayPin));
      if (lights1 != relayReg){
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
      if (fan2NightSwitch){
        manualFanspeed2 = OUTPUT_MIN2;
      }
      else {
        manualFanspeed2 = EEPROM.read(offsetof(storeInEEPROM, manualFanspeed2));
      }
      //Serial.println("LIGHTS2_ON");
    }
    else if (currentMinutes < minutesLights2On || currentMinutes > minutesLights2On){
      lights2=false;
      targetSoilTemp2=nightSoilTemp2;
      manualFanspeed2 = EEPROM.read(offsetof(storeInEEPROM, manualFanspeed2));
      //Serial.println("LIGHTS2_OFF");
    }
  } 
  bool relayReg = !(*portOutputRegister( digitalPinToPort(relayPin) ) & digitalPinToBitMask(relayPin));

  if (lights2 != relayReg){
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
      targetSoilTemp4=daySoilTemp4;
      //Serial.println("LIGHTS2_ON");
    }
    else if (currentMinutes < minutesLights3On || currentMinutes > minutesLights3On){
      lights3=false;
      targetSoilTemp3=nightSoilTemp3;
      targetSoilTemp4=daySoilTemp4;
      //Serial.println("LIGHTS2_OFF");
    }
  } 
  bool relayReg = !(*portOutputRegister( digitalPinToPort(relayPin) ) & digitalPinToBitMask(relayPin));

  if (lights3 != relayReg){
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
  bool relayReg = !(*portOutputRegister( digitalPinToPort(relayPin) ) & digitalPinToBitMask(relayPin));
  if (heater1 != relayReg){
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
  bool relayReg = !(*portOutputRegister( digitalPinToPort(relayPin) ) & digitalPinToBitMask(relayPin));
  if (heater2 != relayReg){
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
    else if (temp[2] > targetSoilTemp3 + tempRange3){
      heater3 = false;
    }
  }
  bool relayReg = !(*portOutputRegister( digitalPinToPort(relayPin) ) & digitalPinToBitMask(relayPin));
  if (heater3 != relayReg){
    //heaterState2 = heater2;
    digitalWrite(relayPin, !heater3);
    notifyClientsSingleObject("heater3", heater3);
  }
  return;
}

void heater4Control(boolean manualRelay, const int relayPin){
  if (!manualRelay){
    if (temp[3] < targetSoilTemp4 - tempRange4){
      heater4 = true;
    }
    else if (temp[3] > targetSoilTemp4 + tempRange4){
      heater4 = false;
    }
  }
  bool relayReg = !(*portOutputRegister( digitalPinToPort(relayPin) ) & digitalPinToBitMask(relayPin));
  if (heater4 != relayReg){
    //heaterState2 = heater2;
    digitalWrite(relayPin, !heater4);
    notifyClientsSingleObject("heater4", heater4);
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
  bool relayReg = !(*portOutputRegister( digitalPinToPort(relayPin) ) & digitalPinToBitMask(relayPin));
    //if (humidifierState1 != humidifier1){
    if (humidifier1 != relayReg){
      //humidifierState1 = humidifier1;
      digitalWrite(relayPin, !humidifier1);
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
bool relayReg = !(*portOutputRegister( digitalPinToPort(relayPin) ) & digitalPinToBitMask(relayPin));
  // if (humidifierState2 != humidifier2){
  //   humidifierState2 = humidifier2;
  if (humidifier2 != relayReg){
    digitalWrite(relayPin, !humidifier2);
    notifyClientsSingleObject("humidifier2", humidifier2);
  }
  return;
}

// void humidity3Control(boolean manualRelay, const int relayPin){
// if (!manualRelay){
//   if (humidity[2] < humidMin3){
//     humidifier3 = true;
//   }
//   else if (humidity[2] > humidMax3){
//     humidifier3 = false;
//   }
// }
// bool relayReg = !(*portOutputRegister( digitalPinToPort(relayPin) ) & digitalPinToBitMask(relayPin));
//   // if (humidifierState2 != humidifier2){
//   //   humidifierState2 = humidifier2;
//   if (humidifier3 != relayReg){
//     digitalWrite(relayPin, humidifier3);
//     notifyClientsSingleObject("humidifier3", humidifier3);
//   }
//   return;
// }

void fan1Control(){ // CLIMATE 1 based on TargetAirTemp1 and value from DHT22 sensor.
if (manualMosfet1){
  if (fan1){
    outputVal1 = manualFanspeed1;
    }
  else {outputVal1 = 0;
  }
  if (msgFanState1){
    messageFanState1();
  }
}
else if (!manualMosfet1){
  fan1 = true;
  if (!PIDcontrol){
    if (humidifier1){
      outputVal1 = OUTPUT_MIN1;
    }
    else {
      if (dhtTemp[0] > targetAirTemp1 + alarmRange1){
        outputVal1 = OUTPUT_MAX1;
      }
      else if (dhtTemp[0] < targetAirTemp1){ //  - tempRange1
        outputVal1 = manualFanspeed1; // OUTPUT_MIN1
      }
      // else if (dhtTemp[0] > targetAirTemp1 - tempRange1 && dhtTemp[0] < targetAirTemp1 + tempRange1){
      //   outputVal1 = manualFanspeed1;
      // }
      else if (dhtTemp[0] > targetAirTemp1 && dhtTemp[0] < targetAirTemp1 + alarmRange1){
        outputVal1 = modifiedMap((dhtTemp[0]-targetAirTemp1), 0, alarmRange1, OUTPUT_MIN1, OUTPUT_MAX1);
      }
      }
  }
  else {
      temperature=dhtTemp[0];
      myPID.run(); //call every loop, updates automatically at certain time interval
  }
  if (outputVal1 == 0){
    fan1 = false;
  }
  else {
    fan1 = true;
  }
}

if (fanState1 != fan1 || msgFanState1 == true){
  fanState1 = fan1;
  messageFanState1();
}   
//Serial.println(outputVal1);
ledcWrite(ledChannel1, outputVal1);
fanspeed1 = map(outputVal1, 0, 255, 0, 100);
//Serial.println(fanspeed1);
notifyClientsSingleObjectByte("fanspeed1", fanspeed1);   
}

void messageFanState1(){
//digitalWrite(ONBOARD_LED, fan1);
msgFanState1 = false;
notifyClientsSingleObject("fan1", fan1);
return;
}

void fan2Control(){ // CLIMATE 1 based on TargetAirTemp1 and value from DHT22 sensor.
if (manualMosfet2){
  if (fan2){
    outputVal2 = manualFanspeed2;
    }
  else {
    outputVal2 = 0;
    }
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
      else if (dhtTemp[1] < targetAirTemp2){ //  - tempRange2
        outputVal2 = manualFanspeed2; // OUTPUT_MIN2
      }
      // else if (dhtTemp[1] > targetAirTemp2 - tempRange2 && dhtTemp[1] < targetAirTemp2 + tempRange2){
      //   outputVal2 = manualFanspeed2;
      // }
      else if (dhtTemp[1] > targetAirTemp2 && dhtTemp[1] < targetAirTemp2 + alarmRange2){
        outputVal2 = modifiedMap((dhtTemp[1]-targetAirTemp2), 0, alarmRange2, OUTPUT_MIN2, OUTPUT_MAX2);
      }
    if (outputVal2 == 0){
      fan2 = false;
    }
    else{
      fan2 = true;
    }   
  }
}

if (fanState2 != fan2 || msgFanState2 == true){
fanState2 = fan2;
messageFanState2();
}

//Serial.println(outputVal);
ledcWrite(ledChannel2, outputVal2);
fanspeed2 = map(outputVal2, 0, 255, 0, 100);
notifyClientsSingleObjectByte("fanspeed2", fanspeed2);   
}

void messageFanState2(){
msgFanState2 = false;
notifyClientsSingleObject("fan2", fan2);
return;
}

void DHTsensors(boolean manualRelay, const int relayPin){
bool relayReg = !(*portOutputRegister( digitalPinToPort(relayPin) ) & digitalPinToBitMask(relayPin));
if (relayReg == 0 && (sensorsOff)){
  digitalWrite(relayPin, false); // boolean resetHumidity = false;
  sensorsOff = false;
}

if (resetHumidity){
  digitalWrite(relayPin, true); // boolean resetHumidity is also true
    resetHumidity = false;
    humidityCounter = 0;
    sensorsOff = true;
    resetCount = resetCount + 1;
}
}