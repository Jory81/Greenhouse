void displayOledScreen(float temp1, float temp2, float temp3, float temp4){
  displayTime = !displayTime;
  if (!displayTime){
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0,22);// 30 is fit
  display.print("soil");
  display.setCursor(0,46); // 54 is fit
  display.print("soil");
  display.setCursor(96,8); 
  display.print(targetSoilTemp1); display.print((char)247); //display.print((char)247);
  display.setCursor(96,16); 
  display.print(fanspeed1); display.print("%");
  uint8_t calposition = 54-(probeCountT*8);
  display.setCursor(96, calposition);
  display.print("cal:");
  for (int sensor = 0; sensor < probeCountT; sensor++){
    uint8_t yposition = (calposition+8)+sensor*8;
    display.setCursor(96, yposition);
    display.print(calibrationValue[sensor], 1);
  }
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

  if (probeCountT > 2){
    display.setCursor(0,56);
    display.print("T3:"); display.print(temp3, 0);
      if (probeCountT > 3){
      display.print(" T4:"); display.print(temp4, 0);  
      }
  }
  display.setTextSize(3); 
  display.setCursor(24,8);
  display.print(temp1, 0); display.setTextSize(1); display.println((char)247);
  display.setCursor(24,32); 
  display.setTextSize(3);
  display.print(temp2, 0); display.setTextSize(1); display.println((char)247);
  display.display();
  }
  else if (displayTime){
    display.clearDisplay();
    display.setCursor(0,0);
    char buffer[40];
    sprintf(buffer, "time %02d:%02d", totalHours, totalMinutes);  
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
    bool lights1Reg = !(*portOutputRegister( digitalPinToPort(RELAYPINLIGHTS1) ) & digitalPinToBitMask(RELAYPINLIGHTS1));
    bool lights2Reg = !(*portOutputRegister( digitalPinToPort(RELAYPINLIGHTS2) ) & digitalPinToBitMask(RELAYPINLIGHTS2));
    bool heater1Reg = !(*portOutputRegister( digitalPinToPort(RELAYPINHEATER1) ) & digitalPinToBitMask(RELAYPINHEATER1));
    bool heater2Reg = !(*portOutputRegister( digitalPinToPort(RELAYPINHEATER2) ) & digitalPinToBitMask(RELAYPINHEATER2));
    bool opt1Reg = !(*portOutputRegister( digitalPinToPort(RELAYPINOPTIONAL1) ) & digitalPinToBitMask(RELAYPINOPTIONAL1));
    bool opt2Reg = !(*portOutputRegister( digitalPinToPort(RELAYPINOPTIONAL2) ) & digitalPinToBitMask(RELAYPINOPTIONAL2));
    sprintf(buffer, "Relay heaters:  %d / %d", heater1Reg, heater2Reg);
    display.print(buffer);
    display.setCursor(0,36);
    sprintf(buffer, "Relay lights:   %d / %d", lights1Reg, lights2Reg);
    display.print(buffer);
    display.setCursor(0,44);
    sprintf(buffer, "Relay optional: %d / %d", opt1Reg, opt2Reg);
    display.print(buffer);
    display.setCursor(0,54);
    //char buffer[40];
    sprintf(buffer, "uptime %02d:%02d:%02d", days, hours, minutes);    
    display.print(buffer);
  display.display();
  }
    
  }