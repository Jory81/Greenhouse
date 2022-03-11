var gateway = `ws://${window.location.hostname}/ws`;
var websocket;

console.log(gateway);

var soiltemp2=0;
var airtemp2=0;
var Fanspeed2=0;
var humidity2=0;
var targetCelsius2=0;

var soiltemp1=0;
var airtemp1=0;
var Fanspeed1=0;
var humidity1=0;
var targetCelsius1=0;

window.addEventListener('load', onLoad);

function onLoad(event) {
	
    initWebSocket();
//    initButton();
}

// ----------------------------------------------------------------------------
// WebSocket handling
// ----------------------------------------------------------------------------

function initWebSocket() {
    console.log('Trying to open a WebSocket connection...');
    websocket = new WebSocket(gateway);
    websocket.onopen    = onOpen;
    websocket.onclose   = onClose;
    websocket.onmessage = onMessage;
}
 
// Called when a WebSocket connection is established with the server

function onOpen(event) {
 
    // Log connection state
    console.log("Connected");
	updateTimeOnopen();
	//document.getElementById("SetTab2").style.display = "none";	
	document.getElementById("Reboot").checked = true;
	var myJSON = JSON.stringify({requestInfo:true});
	websocket.send(myJSON);  
	//console.log(myJSON);
	//k = 0;
	//doSend("GGG"+k);

}
 
// Called when the WebSocket connection is closed
function onClose(event) {
    //console.log('Connection closed');
    setTimeout(initWebSocket, 2000);
}

function updateNumber(key, value) {
	var field = key;
	var obj = {};
	obj[field] = value;
	var myJSON = JSON.stringify(obj);
	websocket.send(myJSON);  
	//console.log(myJSON);
}

function updateText(key, value) {
	//console.log(key);
	//console.log(value);
	var field = key;
	var obj = {};
	obj[field] = value;
	var myJSON = JSON.stringify(obj);
	websocket.send(myJSON);  
	//console.log(myJSON);
}

function updateSlider(key, value){
	document.getElementById(key).innerHTML = value;
	var field = key;
	var obj = {};
	obj[field] = value;
	var myJSON = JSON.stringify(obj);
	websocket.send(myJSON);
	//console.log(myJSON);	
}

function updateButton(key, value) {
	var field = key;
	var obj = {};
	obj[field] = value;
	var myJSON = JSON.stringify(obj);
	websocket.send(myJSON);
	//console.log(myJSON);
}

function updateLightSchedule(key1, value1, key2, value2) {
	var field = key1;
	var field2 = key2;
	var obj = {};
	var obj2 = {};
	
	document.getElementById(key1).innerHTML = value1;
	document.getElementById(key2).innerHTML = value2;

	obj[field] = value1;
	obj2[field2] = value2;
	Object.assign(obj, obj2);

	var myJSON = JSON.stringify(obj);
	websocket.send(myJSON); 
	//console.log(myJSON); 
}
 
// Called when a message is received from the server
function onMessage(event) {
let data = JSON.parse(event.data);
//console.log(data);

	var len = Object.keys(data).length;
	//console.log("length ");
	//console.log(len);

	for (var i = 0; i < len; i++) {
	var variable = Object.keys(data)[i];
	//console.log("this is the name "); console.log(variable);
	if (variable == 'probeTypeT'){document.getElementById('probeTypeT').value = data.probeTypeT;}
	else if (variable == 'probeCountT'){document.getElementById('probeCountT').value = data.probeCountT;}
	else if (variable == 'probeTypeH'){document.getElementById('probeTypeH').value = data.probeTypeH;}
	else if (variable == 'probeCountH'){document.getElementById('probeCountH').value = data.probeCountH;}
	else if (variable == 'relay1c'){document.getElementById('relay1Connected').checked = data.relay1c;}
	else if (variable == 'relay2c'){document.getElementById('relay2Connected').checked = data.relay2c;}
	else if (variable == 'relay3c'){document.getElementById('relay3Connected').checked = data.relay3c;}
	else if (variable == 'relay4c'){document.getElementById('relay4Connected').checked = data.relay4c;}
	else if (variable == 'relay5c'){document.getElementById('relay5Connected').checked = data.relay5c;}
	else if (variable == 'relay6c'){document.getElementById('relay6Connected').checked = data.relay6c;}

	else if (variable == 'relay1f'){document.getElementById('funcRelay1').value = data.relay1f;}
	else if (variable == 'relay2f'){document.getElementById('funcRelay2').value = data.relay2f;}
	else if (variable == 'relay3f'){document.getElementById('funcRelay3').value = data.relay3f;}
	else if (variable == 'relay4f'){document.getElementById('funcRelay4').value = data.relay4f;}
	else if (variable == 'relay5f'){document.getElementById('funcRelay5').value = data.relay5f;}
	else if (variable == 'relay6f'){document.getElementById('funcRelay6').value = data.relay6f;}

	else if (variable == 'fan1Connected'){document.getElementById('fan1Connected').checked = data.fan1Connected;}
	else if (variable == 'fan2Connected'){document.getElementById('fan2Connected').checked = data.fan2Connected;}
	else if (variable == 'graphUpdate'){document.getElementById('graphUpdate').value = data.graphUpdate;}
	else if (variable == 'tempUpdate'){document.getElementById('tempUpdate').value = data.tempUpdate;}
	else if (variable == 'measurements'){document.getElementById('measurements').value = data.measurements;}
	//else if (variable == 'PIDcontrol'){document.getElementById('PIDcontrol').checked = data.PIDcontrol;}

	else if (variable == 'calV1'){document.getElementById('calValue1').value = data.calV1;}
	else if (variable == 'calV2'){document.getElementById('calValue2').value = data.calV2;}
	else if (variable == 'calV3'){document.getElementById('calValue3').value = data.calV3;}
	else if (variable == 'calV4'){document.getElementById('calValue4').value = data.calV4;}
	else if (variable == 'calV5'){document.getElementById('calValue5').value = data.calV5;}
	else if (variable == 'calV6'){document.getElementById('calValue6').value = data.calV6;}
	else if (variable == 'calV7'){document.getElementById('calValue7').value = data.calV7;}
	else if (variable == 'calV8'){document.getElementById('calValue8').value = data.calV8;}
	//else if (variable == 'calValue9'){document.getElementById('calValue9').value = data.calValue9;}
	//else if (variable == 'calValue10'){document.getElementById('calValue10').value = data.calValue10;}
	//else if (variable == 'calValue11'){document.getElementById('calValue11').value = data.calValue11;}

	else if (variable == 'OUTPUT_MIN1'){document.getElementById('OUTPUT_MIN1').value = data.OUTPUT_MIN1;}
	else if (variable == 'OUTPUT_MAX1'){document.getElementById('OUTPUT_MAX1').value = data.OUTPUT_MAX1;}
	else if (variable == 'OUTPUT_MIN2'){document.getElementById('OUTPUT_MIN2').value = data.OUTPUT_MIN2;}
	else if (variable == 'OUTPUT_MAX2'){document.getElementById('OUTPUT_MAX2').value = data.OUTPUT_MAX2;}

	// else if (variable == 'KP'){document.getElementById('KP').value = data.KP;}
	// else if (variable == 'KI'){document.getElementById('KI').value = data.KI;}
	// else if (variable == 'KD'){document.getElementById('KD').value = data.KD;}

	else if (variable == 'lights1ON'){document.getElementById('lights1ON').innerHTML = data.lights1ON;}
	else if (variable == 'lights1OFF'){document.getElementById('lights1OFF').innerHTML = data.lights1OFF;}
	else if (variable == 'lights2ON'){document.getElementById('lights2ON').innerHTML = data.lights2ON;}
	else if (variable == 'lights2OFF'){document.getElementById('lights2OFF').innerHTML = data.lights2OFF;}
	else if (variable == 'lights3ON'){document.getElementById('lights3ON').innerHTML = data.lights3ON;}
	else if (variable == 'lights3OFF'){document.getElementById('lights3OFF').innerHTML = data.lights3OFF;}

	else if (variable == 'humidMin1'){document.getElementById('humidMin1').value = data.humidMin1;}
	else if (variable == 'humidMax1'){document.getElementById('humidMax1').value = data.humidMax1;}
	else if (variable == 'humidMin2'){document.getElementById('humidMin2').value = data.humidMin2;}
	else if (variable == 'humidMax2'){document.getElementById('humidMax2').value = data.humidMax2;}
	else if (variable == 'humidMin3'){document.getElementById('humidMin3').value = data.humidMin3;}
	else if (variable == 'humidMax3'){document.getElementById('humidMax3').value = data.humidMax3;}

	else if (variable == 'daySoilTemp1'){document.getElementById('daySoilTemp1').value = data.daySoilTemp1;}
	else if (variable == 'nightSoilTemp1'){document.getElementById('nightSoilTemp1').value = data.nightSoilTemp1;}
	else if (variable == 'tempRange1'){document.getElementById('tempRange1').value = data.tempRange1;}
	else if (variable == 'alarmRange1'){document.getElementById('alarmRange1').value = data.alarmRange1;}
	else if (variable == 'soilAlarm1'){document.getElementById('soilAlarm1').checked = data.soilAlarm1;}
	else if (variable == 'targetAirTemp1'){document.getElementById('targetAirTemp1').value = data.targetAirTemp1;}
	else if (variable == 'airAlarm1'){document.getElementById('airAlarm1').checked = data.airAlarm1;}

	else if (variable == 'daySoilTemp2'){document.getElementById('daySoilTemp2').value = data.daySoilTemp2;}
	else if (variable == 'nightSoilTemp2'){document.getElementById('nightSoilTemp2').value = data.nightSoilTemp2;}	
	else if (variable == 'tempRange2'){document.getElementById('tempRange2').value = data.tempRange2;}
	else if (variable == 'alarmRange2'){document.getElementById('alarmRange2').value = data.alarmRange2;}
	else if (variable == 'soilAlarm2'){document.getElementById('soilAlarm2').checked = data.soilAlarm2;}
	else if (variable == 'targetAirTemp2'){document.getElementById('targetAirTemp2').value = data.targetAirTemp2;}
	else if (variable == 'airAlarm2'){document.getElementById('airAlarm2').checked = data.airAlarm2;}

	else if (variable == 'daySoilTemp3'){document.getElementById('daySoilTemp3').value = data.daySoilTemp3;}
	else if (variable == 'nightSoilTemp3'){document.getElementById('nightSoilTemp3').value = data.nightSoilTemp3;}	
	else if (variable == 'tempRange3'){document.getElementById('tempRange3').value = data.tempRange3;}
	else if (variable == 'alarmRange3'){document.getElementById('alarmRange3').value = data.alarmRange3;}
	else if (variable == 'soilAlarm3'){document.getElementById('soilAlarm3').checked = data.soilAlarm3;}

	else if (variable == 'daySoilTemp4'){document.getElementById('daySoilTemp4').value = data.daySoilTemp4;}
	else if (variable == 'nightSoilTemp4'){document.getElementById('nightSoilTemp4').value = data.nightSoilTemp4;}	
	else if (variable == 'tempRange4'){document.getElementById('tempRange4').value = data.tempRange4;}
	else if (variable == 'alarmRange4'){document.getElementById('alarmRange4').value = data.alarmRange4;}
	else if (variable == 'soilAlarm4'){document.getElementById('soilAlarm4').checked = data.soilAlarm4;}

	else if (variable == 'fan1m'){document.getElementById('manualMosfet1').checked = data.fan1m;}
	else if (variable == 'manualFanspeed1'){document.getElementById('manualFanspeed1').value = data.manualFanspeed1;}
	else if (variable == 'fan2m'){document.getElementById('manualMosfet2').checked = data.fan2m;}
	else if (variable == 'manualFanspeed2'){document.getElementById('manualFanspeed2').value = data.manualFanspeed2;}
	else if (variable == 'fan1daySwitch'){document.getElementById('fan1daySwitch').checked = data.fan1daySwitch;}
	else if (variable == 'fan2daySwitch'){document.getElementById('fan2daySwitch').checked = data.fan2daySwitch;}

	else if (variable == 'relay1m'){document.getElementById('manualRelay1').checked = data.relay1m;}
	else if (variable == 'relay2m'){document.getElementById('manualRelay2').checked = data.relay2m;}
	else if (variable == 'relay3m'){document.getElementById('manualRelay3').checked = data.relay3m;}
	else if (variable == 'relay4m'){document.getElementById('manualRelay4').checked = data.relay4m;}
	else if (variable == 'relay5m'){document.getElementById('manualRelay5').checked = data.relay5m;}
	else if (variable == 'relay6m'){document.getElementById('manualRelay6').checked = data.relay6m;}

	else if (variable == 'lights1'){document.getElementById('lights1').checked = data.lights1;}
	else if(variable == 'heater1'){document.getElementById('heater1').checked = data.heater1;}
	else if(variable == 'fan1'){document.getElementById('fan1').checked = data.fan1;}
	else if(variable == 'humidifier1'){document.getElementById('humidifier1').checked = data.humidifier1;}

	else if (variable == 'lights2'){document.getElementById('lights2').checked = data.lights2;}
	else if(variable == 'heater2'){document.getElementById('heater2').checked = data.heater2;}
	else if(variable == 'fan2'){document.getElementById('fan2').checked = data.fan2;}
	else if(variable == 'humidifier2'){document.getElementById('humidifier2').checked = data.humidifier2;}

	else if (variable == 'lights3'){document.getElementById('lights3').checked = data.lights3;}
	else if(variable == 'heater3'){document.getElementById('heater3').checked = data.heater3;}
	else if(variable == 'heater4'){document.getElementById('heater4').checked = data.heater4;}
	//else if(variable == 'fan3'){document.getElementById('fan3').checked = data.fan3;}
	//else if(variable == 'humidifier3'){document.getElementById('humidifier3').checked = data.humidifier2;}

	else if (variable == 'saveInEEPROM'){document.getElementById('saveInEEPROM').checked = data.saveInEEPROM;}

	else if (variable == 'temperature1'){document.getElementById('temperature1').innerHTML = data.temperature1; soiltemp1 = data.temperature1; updateTime();}
	else if (variable == 'temperature2'){document.getElementById('temperature2').innerHTML = data.temperature2; soiltemp2 = data.temperature2;}
	else if (variable == 'temperature3'){document.getElementById('temperature3').innerHTML = data.temperature3;}
	else if (variable == 'temperature4'){document.getElementById('temperature4').innerHTML = data.temperature4;}
	else if (variable == 'temperature6'){document.getElementById('temperature6').innerHTML = data.temperature6; airtemp1 = data.temperature6;}
	else if (variable == 'temperature7'){document.getElementById('temperature7').innerHTML = data.temperature7; airtemp2 = data.temperature7;}
	//else if (variable == 'temperature8'){document.getElementById('temperature8').innerHTML = data.temperature8; airtemp3 = data.temperature8;}

	else if (variable == 'humid1'){document.getElementById('humid1').innerHTML = data.humid1; humidity1 = data.humid1;}
	else if (variable == 'humid2'){document.getElementById('humid2').innerHTML = data.humid2; humidity2 = data.humid2;}
	//else if (variable == 'humid3'){document.getElementById('humid3').innerHTML = data.humid3; humidity3 = data.humid3;}

	else if (variable == 'fanspeed1'){document.getElementById('fanspeed1').innerHTML = data.fanspeed1; Fanspeed1 = data.fanspeed1}
	else if (variable == 'fanspeed2'){document.getElementById('fanspeed2').innerHTML = data.fanspeed2; Fanspeed2 = data.fanspeed2}

	else if (variable == 'wifiID'){document.getElementById('wifiID').innerHTML = data.wifiID;}
	else if (variable == 'wifiPASS'){document.getElementById('wifiPASS').innerHTML = data.wifiPASS;}

	else if (variable == 'updateGraph'){if (data.updateGraph == true){drawChart1(); drawChart2();} else {drawChart1();};}

	else if (variable == 'recMsg'){document.getElementById('recMsg').checked = data.recMsg; setTimeout(msgRec, 800);}
	}
}


	function msgRec(){
		document.getElementById('recMsg').checked = false;
	}
	

<script src="https://code.highcharts.com/highcharts.js"></script>

 	var chartT1 = new Highcharts.Chart({
            chart: { renderTo : 'chart-temperature1',
					backgroundColor: '#808080',
			},
            title: { text: 'Temperature History',
						style: {color: '#ffffff',}
					},
            series: [
                {
                    showInLegend: true,
                    name: 'Soil temp',
                    data: [],
                    color: '#ff0000'
                }, 
				{
                    showInLegend: true,
                    name: 'Air temp',
                    data: [],
                    color: '#ffff00'
                },
				{
                    showInLegend: true,
                    name: 'fan %',
                    data: [], 
                    color: '#00ff00'
                },
				{
                    showInLegend: true,
                    name: 'humidity %',
                    data: [], 
                    color: '#00ffff'
                }

            ],
			tooltip: {
				valueSuffix: '\xB0 C'
			},
			 plotOptions: {
				line: {
					lineWidth: 2,
					states: {
						hover: {
							lineWidth: 3
						}
					},
					
					marker: {
						enabled: false
					}
					
				}
			},
			legend: {
				itemStyle: {
					color: 'white'
				}  
			},
            xAxis: { 
               type: 'datetime',
               dateTimeLabelFormats: { second: '%H:%M:%S' },
			   title: { style: {color:"#ffffff"}},
			   labels: { style: {color:"#ffffff"}}
            },
            yAxis: {
                title: { text: 'Temperature (Celsius)',
						 style: {color:"#ffffff"}},
				labels: { style: {color:"#ffffff"}}
            },
            credits: { enabled: false }
        });
function drawChart1(){
            var x = (new Date()).getTime();
			var currentCelsius = parseFloat(soiltemp1);
			var currentAirCelsius = parseFloat(airtemp1);
			var currentFanSpeed = parseFloat(Fanspeed1);
			var currentHumidity = parseFloat(humidity1);
			
			if(chartT1.series[0].data.length > 1440) {
				chartT1.series[0].addPoint([x, currentCelsius], true, true, true);
				chartT1.series[1].addPoint([x, currentAirCelsius], true, true, true);
				chartT1.series[2].addPoint([x, currentFanSpeed], true, true, true);
				chartT1.series[3].addPoint([x, currentHumidity], true, true, true);
			} else {
				chartT1.series[0].addPoint([x, currentCelsius], true, false, true);
				chartT1.series[1].addPoint([x, currentAirCelsius], true, false, true);
				chartT1.series[2].addPoint([x, currentFanSpeed], true, false, true);
				chartT1.series[3].addPoint([x, currentHumidity], true, false, true);
			}
            // if (targetCelsius1 > 0) {
            //     if(chartT1.series[1].data.length > 1440) {
            //         chartT1.series[1].addPoint([x, targetCelsius1], true, true, true);
            //     } else {
            //         chartT1.series[1].addPoint([x, targetCelsius1], true, false, true);
            //     }
            // }
			
//function drawChart1(temp
};

 	var chartT2 = new Highcharts.Chart({
            chart: { renderTo : 'chart-temperature2',
					backgroundColor: '#808080',
			},
            title: { text: 'Temperature History',
						style: {color: '#ffffff',}
					},
            series: [
				{
                    showInLegend: true,
                    name: 'Soil temp2',
                    data: [],
                    color: '#ff0000'
                }, 
				{
                    showInLegend: true,
                    name: 'Air temp2',
                    data: [],
                    color: '#ffff00'
                },
				{
                    showInLegend: true,
                    name: 'fan2 %',
                    data: [], 
                    color: '#00ff00'
                },
				{
                    showInLegend: true,
                    name: 'humidity2 %',
                    data: [], 
                    color: '#00ffff'
                }

            ],
			tooltip:
			{
				valueSuffix: '\xB0 C'
			},
			 plotOptions: {
				line: {
					lineWidth: 2,
					states: {
						hover: {
							lineWidth: 3
						}
					},
					
					marker: {
						enabled: false
					}
					
				}
			},
			legend: {
				itemStyle: {
					color: 'white'
				}  
			},
            xAxis: { 
               type: 'datetime',
               dateTimeLabelFormats: { second: '%H:%M:%S' },
			   title: { style: {color:"#ffffff"}},
			   labels: { style: {color:"#ffffff"}}
            },
            yAxis: {
                title: { text: 'Temperature (Celsius)',
						 style: {color:"#ffffff"}},
				labels: { style: {color:"#ffffff"}}
            },
            credits: { enabled: false }
        });
function drawChart2(){
            var x = (new Date()).getTime();
			var currentCelsius = parseFloat(soiltemp2);
			var currentAirCelsius = parseFloat(airtemp2);
			var currentFanSpeed = parseFloat(Fanspeed2);
			var currentHumidity = parseFloat(humidity2);
			if(chartT2.series[0].data.length > 1440) {
				chartT2.series[0].addPoint([x, currentCelsius], true, true, true);
				chartT2.series[1].addPoint([x, currentAirCelsius], true, true, true);
				chartT2.series[2].addPoint([x, currentFanSpeed], true, true, true);
				chartT2.series[3].addPoint([x, currentHumidity], true, true, true);
			} else {
				chartT2.series[0].addPoint([x, currentCelsius], true, false, true);
				chartT2.series[1].addPoint([x, currentAirCelsius], true, false, true);
				chartT2.series[2].addPoint([x, currentFanSpeed], true, false, true);
				chartT2.series[3].addPoint([x, currentHumidity], true, false, true);
			}
            // if (targetCelsius2 > 0) {
            //     if(chartT2.series[1].data.length > 1440) {
            //         chartT2.series[1].addPoint([x, targetCelsius2], true, true, true);
            //     } else {
            //         chartT2.series[1].addPoint([x, targetCelsius2], true, false, true);
            //     }
            // }
			
//function drawChart1(temp
};

function playSound1(repeats){
		numberofRingsAlarm1=repeats;
		var sound = new Audio('/alarm.mp3');
		sound.play();
			if (numberofRingsAlarm1 > 1){
			refreshIntervalId1 = setInterval(function(){
			var sound = new Audio('/alarm.mp3');
			sound.play(); 
			timer1Counter = timer1Counter + 1;
			blockSound1(numberofRingsAlarm1, timer1Counter);
			}, 2900);
			}
	}			
	
	function blockSound1(numberofRings, timerCounter){
		if (timerCounter >= numberofRings){
		clearInterval(refreshIntervalId1);
		document.getElementById("1I1").checked = false;
		//console.log("switches alarm sign off");
		}
	}
/*
function setTab(fixedVariable) {
  var x = document.getElementById(fixedVariable);
  if (x.style.display === "none") {
    x.style.display = "block";
  } else {
    x.style.display = "none";
  }
}
*/

<script src="https://cdnjs.cloudflare.com/ajax/libs/moment.js/2.18.1/moment.min.js"></script>


function updateTimeOnopen(){
	var today = new Date();
	var hours = today.getHours()
	//console.log(hours);
	var minutes = today.getMinutes()
	//console.log(minutes);
	var obj = {};
	obj.hours=hours;
	obj.minutes=minutes;
	var myJSON = JSON.stringify(obj);
	//console.log(myJSON);
	websocket.send(myJSON); 

	
//var time =  + ":" +  + ":" + today.getSeconds();
//console.log(time);
}

function updateTime(){
        var momentNow = moment();
        $('#date-part').html(momentNow.format('YYYY MMMM DD') + ' '
                            + momentNow.format('dddd')
                             .substring(0,3).toUpperCase());
        $('#time-part').html(momentNow.format('HH:mm:ss'));		
		}

function openTab(fixedVariable, evt, cityName) {
if (fixedVariable == '1Z1'){
document.getElementById('1Z2').checked = false;
document.getElementById('1Z3').checked = false;
document.getElementById('1Z4').checked = false;
document.getElementById('1Z5').checked = false;
document.getElementById('1Z6').checked = false;
document.getElementById('1Z7').checked = false;
}
else if (fixedVariable == '1Z2'){
document.getElementById('1Z1').checked = false;
document.getElementById('1Z3').checked = false;
document.getElementById('1Z4').checked = false;
document.getElementById('1Z5').checked = false;
document.getElementById('1Z6').checked = false;
document.getElementById('1Z7').checked = false;
}
else if (fixedVariable == '1Z3'){
document.getElementById('1Z1').checked = false;
document.getElementById('1Z2').checked = false;
document.getElementById('1Z4').checked = false;
document.getElementById('1Z5').checked = false;
document.getElementById('1Z6').checked = false;
document.getElementById('1Z7').checked = false;
}
else if (fixedVariable == '1Z4'){
document.getElementById('1Z1').checked = false;
document.getElementById('1Z2').checked = false;
document.getElementById('1Z3').checked = false;
document.getElementById('1Z5').checked = false;
document.getElementById('1Z6').checked = false;
document.getElementById('1Z7').checked = false;
}
else if (fixedVariable == '1Z5'){
document.getElementById('1Z1').checked = false;
document.getElementById('1Z2').checked = false;
document.getElementById('1Z3').checked = false;
document.getElementById('1Z4').checked = false;
document.getElementById('1Z6').checked = false;
document.getElementById('1Z7').checked = false;
}
else if (fixedVariable == '1Z6'){
document.getElementById('1Z1').checked = false;
document.getElementById('1Z2').checked = false;
document.getElementById('1Z3').checked = false;
document.getElementById('1Z4').checked = false;
document.getElementById('1Z5').checked = false;
document.getElementById('1Z7').checked = false;
}
else if (fixedVariable == '1Z7'){
document.getElementById('1Z1').checked = false;
document.getElementById('1Z2').checked = false;
document.getElementById('1Z3').checked = false;
document.getElementById('1Z4').checked = false;
document.getElementById('1Z5').checked = false;
document.getElementById('1Z6').checked = false;
}

if (document.getElementById(fixedVariable).checked == true){
  var i, tabcontent, tablinks;
  tabcontent = document.getElementsByClassName("tabcontent");
  for (i = 0; i < tabcontent.length; i++) {
    tabcontent[i].style.display = "none";
  }
  tablinks = document.getElementsByClassName("tablinks");
  for (i = 0; i < tablinks.length; i++) {
    tablinks[i].className = tablinks[i].className.replace(" active", "");
  }
  document.getElementById(cityName).style.display = "block";
  evt.currentTarget.className += " active";
  }
  else {
  document.getElementById(fixedVariable).checked = false;
  document.getElementById(cityName).style.display = "none";
  }
}

function toggleoff(parentElement){
parentElement.style.display='none';
document.getElementById('1Z1').checked = false;
document.getElementById('1Z2').checked = false;
document.getElementById('1Z3').checked = false;
document.getElementById('1Z4').checked = false;
document.getElementById('1Z5').checked = false;
document.getElementById('1Z6').checked = false;
document.getElementById('1Z7').checked = false;
}
