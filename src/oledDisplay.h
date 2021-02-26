void displayOledScreen(float temp1, float temp2, float temp3, float temp4){

  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0,22);// 30 is fit
  display.print("bbq");
  display.setCursor(0,46); // 54 is fit
  display.print("food");
  display.setCursor(96,8); 
  display.print(targetTemperature1); display.print((char)247); //display.print((char)247);
  display.setCursor(96,16); 
  display.print(fanSpeed); display.print("%");
  uint8_t calposition = 54-(sensorAmount*8);
  display.setCursor(96, calposition);
  display.print("cal:");
  for (int sensor = 0; sensor < sensorAmount; sensor++){
    uint8_t yposition = (calposition+8)+sensor*8;
    display.setCursor(96, yposition);
    display.print(calibrationValue[sensor], 1);
  }
  display.setCursor(0,0);
  if (alarmMessage == 0){  
     if (wifiStationMode){
     //display.print("wifi "); display.println(WiFi.localIP());
     display.print("wifi "); display.print(WiFi.localIP());  
     }
     else {
     display.print("soft-AP "); display.println(WiFi.softAPIP());  
     }
  }
  else if (alarmMessage > 0){
    switch(alarmMessage){
      case 1: {display.print("reached targetTemp");} break;
      case 2: {display.print("exceeded tempOffset");} break;
      case 3: {display.print("reached alarmTimer");} break;
    }
  }

  if (sensorAmount > 2){
    display.setCursor(0,56);
    display.print("T3:"); display.print(temp3, 0);
      if (sensorAmount > 3){
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