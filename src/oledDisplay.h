void displayOledScreen(float temp1, float temp2, float temp3, float temp4){
  displayTime = !displayTime;
  if (!displayTime){
  display.clearDisplay();
  display.setTextSize(1);
  char buffer[40];
  if (probeCountT >= 1){
   display.setCursor(0,8);
  sprintf(buffer, "T1: %2.1f   %2.1f", temp1, calibrationValue[0]);  
  display.print(buffer);
  }
  if (probeCountT >= 2){
  display.setCursor(0,16);
  sprintf(buffer, "T2: %2.1f   %2.1f", temp2, calibrationValue[1]);  
  display.print(buffer);
  }
  if (probeCountT >= 3){
  display.setCursor(0,24);
  sprintf(buffer, "T3: %2.1f   %2.1f", temp3, calibrationValue[2]);  
  display.print(buffer);
  }
  if (probeCountT >= 4){
  display.setCursor(0,32);
  sprintf(buffer, "T4: %2.1f   %2.1f", temp4, calibrationValue[3]);  
  display.print(buffer);
  }
  if (probeCountH >= 1){
  display.setCursor(0,40);
  sprintf(buffer, "DHT1: %2.1f   %2.1f", dhtTemp[0], humidity[0]);  
  display.print(buffer);
  }
  if (probeCountH >= 1){ 
  display.setCursor(0,48);
  sprintf(buffer, "DHT2: %2.1f   %2.1f", dhtTemp[1], humidity[1]);  
  display.print(buffer);
  }

  //   display.setCursor(0,10);
  // display.print("t1");
  // display.setCursor(0,46); // 54 is fit
  // display.print("soil");
  // display.setCursor(96,8); 
  // display.print(targetSoilTemp1); display.print((char)247); //display.print((char)247);
  // display.setCursor(96,16); 
  // display.print(fanspeed1); display.print("%");
  // uint8_t calposition = 54-(probeCountT*8);
  // display.setCursor(96, calposition);
  // display.print("cal:");
  // for (int sensor = 0; sensor < probeCountT; sensor++){
  //   uint8_t yposition = (calposition+8)+sensor*8;
  //   display.setCursor(96, yposition);
  //   display.print(calibrationValue[sensor], 1);
  // }

  display.setCursor(0,0);
  // if (alarmMessage == 0){ 
  if (wifiStationMode){
  //display.print("wifi "); display.println(WiFi.localIP());
  display.print("wifi "); display.print(WiFi.localIP());  
  }
  else {
  display.print("soft-AP "); display.println(WiFi.softAPIP());  
  } 
  // }
  // else if (alarmMessage > 0){
  //   switch(alarmMessage){
  //     case 1: {display.print("reached targetTemp");} break;
  //     case 2: {display.print("exceeded tempOffset");} break;
  //     case 3: {display.print("reached alarmTimer");} break;
  //   }
  // }

  // if (probeCountT > 2){
  //   display.setCursor(0,56);
  //   display.print("T3:"); display.print(temp3, 0);
  //     if (probeCountT > 3){
  //     display.print(" T4:"); display.print(temp4, 0);  
  //     }
  // }
  // display.setTextSize(3); 
  // display.setCursor(24,8);
  // display.print(temp1, 0); display.setTextSize(1); display.println((char)247);
  // display.setCursor(24,32); 
  // display.setTextSize(3);
  // display.print(temp2, 0); display.setTextSize(1); display.println((char)247);
  display.display();
  }
  else if (displayTime){
    display.clearDisplay();
    display.setCursor(0,0);
    char buffer[40];
    sprintf(buffer, "time %02d:%02d", dateHour, dateMinute);  
    display.print(buffer);
    display.setCursor(0,10);
    //char buffer2[40];
    sprintf(buffer, "light1 %u-%u: %u", minutesLights1On, minutesLights1Off, currentMinutes); 
    display.print(buffer); 
    display.setCursor(0,18);
    //char buffer2[40];
    sprintf(buffer, "light2 %u-%u: %u", minutesLights2On, minutesLights2Off, currentMinutes); 
    display.print(buffer);
    display.setCursor(0,28);
    // Reads pin settings from register
    bool relayReg1 = !(*portOutputRegister( digitalPinToPort(RELAYPIN1) ) & digitalPinToBitMask(RELAYPIN1));
    bool relayReg2 = !(*portOutputRegister( digitalPinToPort(RELAYPIN2) ) & digitalPinToBitMask(RELAYPIN2));
    bool relayReg3 = !(*portOutputRegister( digitalPinToPort(RELAYPIN3) ) & digitalPinToBitMask(RELAYPIN3));
    bool relayReg4 = !(*portOutputRegister( digitalPinToPort(RELAYPIN4) ) & digitalPinToBitMask(RELAYPIN4));
    bool relayReg5 = !(*portOutputRegister( digitalPinToPort(RELAYPIN5) ) & digitalPinToBitMask(RELAYPIN5));
    bool relayReg6 = !(*portOutputRegister( digitalPinToPort(RELAYPIN6) ) & digitalPinToBitMask(RELAYPIN6));
    sprintf(buffer, "Relay 1/2:   %d / %d", relayReg1, relayReg2);
    display.print(buffer);
    display.setCursor(0,36);
    sprintf(buffer, "Relay 3/4:   %d / %d", relayReg3, relayReg4);
    display.print(buffer);
    display.setCursor(0,44);
    sprintf(buffer, "Relay 5/6:   %d / %d", relayReg5, relayReg6);
    display.print(buffer);
    display.setCursor(0,54);
    //char buffer[40];
    sprintf(buffer, "uptime %02d:%02d:%02d", days, hours, minutes);    
    display.print(buffer);
  display.display();
  }
    
  }