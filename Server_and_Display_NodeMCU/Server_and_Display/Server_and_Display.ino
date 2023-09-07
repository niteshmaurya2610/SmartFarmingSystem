#include <ESP8266WiFi.h>
// including required libs
#include <Wire.h>
#include<SH1106.h>
#include "images.h"

#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ArduinoOTA.h>
#include <ESP8266WebServer.h>
#include <SoftwareSerial.h>

//Rx and Tx pins
SoftwareSerial mySerial(D7, D3); // RX, TX


#ifndef STASSID
#define STASSID "N"
#define STAPSK  "ioup0834"
#endif

#define Thres_humid 1
#define Thres_smoke 680
#define Thres_temp 500
int temp=76, smoke = 32, humidity = 10, pir=1, pumpStatus =0; //inputs
const char* ssid = STASSID;
const char* password = STAPSK;

ESP8266WebServer server(80);

const char* www_username = "nitesh";
const char* www_password = "12345678";



bool LED1status = LOW; //sys
bool LED2status = LOW;  //sprinkler
bool Buzzstatus = LOW; //buzzer
uint8_t LED1pin = D7;
uint8_t LED2pin = D8;
uint8_t Buzzpin =D6;  //need to define pin
//float RPM, MaxRPM,t,t1,t2 = 0;

////////////////////////////
void handle_OnConnect() {
//    if (!server.authenticate(www_username, www_password)) {
//      return server.requestAuthentication();
//    }
  LED1status = LOW;
  LED2status = LOW;
  Buzzstatus = LOW;
  server.send(200, "text/html", SendHTML(LED1status,LED2status, Buzzstatus)); 
}

void handle_led1on() {
  LED1status = HIGH;
  server.send(200, "text/html", SendHTML(true,LED2status, Buzzstatus)); 
}

void handle_led1off() {
  LED1status = LOW;
  server.send(200, "text/html", SendHTML(false,LED2status, Buzzstatus)); 
}



void handle_pumpon() {
  LED2status = HIGH;
  server.send(200, "text/html", SendHTML(LED1status, true, Buzzstatus)); 
}

void handle_pumpoff() {
  LED2status = LOW;
  server.send(200, "text/html", SendHTML(LED1status, false, Buzzstatus)); 
}

void handle_buzzon() {
  Buzzstatus = HIGH;
  server.send(200, "text/html", SendHTML(LED1status, LED2status, true)); 
}

void handle_buzzoff() {
  Buzzstatus = LOW;
  server.send(200, "text/html", SendHTML(LED1status, LED2status,false)); 
}

void handle_NotFound(){
  server.send(404, "text/plain", "Not found");
}

/////////////////////////////////////////////////////////////////////////////////////



// website skeleton
String SendHTML(uint8_t system1, uint8_t sprinkler,uint8_t buzz){
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr +="<title>Smart Farming System</title>\n";
  ptr +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr +="body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h2 {color: #444444;margin: 20px auto 15px;} h3 {color: #444444;margin-bottom: 50px;}\n";
  ptr +=".button {display: block;width: 150px;background-color: #1abc9c;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
  ptr +=".button-on {background-color: #1abc9c;}\n";
  ptr += ".button-right {float: right; margin-right: 40px;}\n";
  ptr += ".button-left {float: left;margin-left: 40px;}\n";
  ptr +=".button-on:active {background-color: #16a085;}\n";
  ptr +=".button-off {background-color: #ff0000;}\n";
  ptr +=".button-off:active {background-color: #ff0000;}\n";
  ptr +="p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
  ptr +="p1 {font-size: 16px;color: #3c2b77;margin-bottom: 10px;}\n";
  ptr +="p-right {font-size: 16px;color: #3c2b77;margin-bottom: 10px;margin-left: 40px;text-align: right;}\n";
  ptr +="p-left {font-size: 16px;color: #3c2b77;margin-bottom: 10px;margin-right: 40px; text-align: left;}\n";

  //ptr +="q {font-size: 17px;color: #3c2b77;margin-bottom: 12px;}\n";
  
  ptr +="</style>\n";
  ptr +="</head>\n";
  ptr +="<body>\n";
  ptr +="<h1>Microprocessor and embedded systems Project</h1>";
  ptr +="<h3>WELCOME TO SMART FARMING SYSTEM</h3>\n";
  ptr +="<hr>";ptr +="<hr>\n";
   if(system1)
  {ptr +="<p>System status: ENABLED</p><a class=\"button button-off\" href=\"/sysOff\">DIS SYS</a>\n";}
  else
  {ptr +="<p>System status: DISABLED</p><a class=\"button button-on\" href=\"/sysOn\">EN SYS</a>\n";}

  


 if(sprinkler)
  {ptr +="<p>Pump Status: ON</p><a class=\"button button-off button-right\" href=\"/pumpOff\">Pump OFF</a>\n";}
  else
  {ptr +="<p>Pump Status: OFF</p><a class=\"button button-on button-right\" href=\"/pumpOn\">Pump ON</a>\n";}


  if(buzz)
  {ptr +="<a class=\"button button-off button-left\" href=\"/buzzOff\">RADAR EN</a><p>Buzzer Status: ON</p>\n";}
  else
  {ptr +="<a class=\"button button-on button-left\" href=\"/buzzOn\">RADAR DIS</a><p>Buzzer Status: OFF</p>\n";}



  
  if(temp){
  ptr +="<h2>  </h2>\n";
  ptr +="<p1>  Temperature :  </p1>";
  ptr+=  temp/6;  ptr+="  Celcius";}

  if(smoke){
  ptr +="<h2>  </h2>\n";
  ptr +="<p1>Smoke Intensity :  </p1>";
  ptr+=  smoke;  ptr+="  unit";}
  
if(humidity){
  ptr +="<h2>  </h2>\n";
  ptr +="<p1>Moisture :  </p1>";
  ptr+=  humidity;  ptr+="  unit";
  }
 if(pir){
  ptr +="<h2>  </h2>\n";
  ptr +="<h3> Cattle detected in the field. </h3>";
  }
 if(pumpStatus){
  ptr +="<h2>  </h2>\n";
  ptr +="<h3> Sprinkler is currently running. </h3>";
  }
  else{
    ptr +="<h2>  </h2>\n";
  ptr +="<h3> Sprinkler is currently not running. </h3>";
  }
 
  if (humidity<Thres_humid){
    ptr +="<h2>  </h2>\n";
    ptr +="<h3>Moisture level Low</h3>\n";
    ptr +="<p>Please turn on the pump</p>\n";
    }
    
  if(smoke>Thres_smoke && temp> Thres_temp){
    ptr +="<h2>  </h2>\n";
    ptr +="<h3>Fire Detetced</h3>\n";   
    }
    else if(smoke<Thres_smoke && temp> Thres_temp){
    ptr +="<h2>  </h2>\n";
    ptr +="<h3> Be Alert!</h3>\n";   
    }
    else if(smoke<Thres_smoke && temp<Thres_temp){
    ptr +="<h2>  </h2>\n";
    ptr +="<h3>Be Alert!</h3>\n";   
    }
    else if(humidity>Thres_humid){
    ptr +="<h2>  </h2>\n";
    ptr +="<h3>The system is running smoothly.</h3>\n";   
    }
  
//  ptr +="<h3> Hello all, from Vignesh </h3>\n";

  
  ptr +="<h3></h3>\n";
  ptr +="<h3>Thank You</h3>\n";
  ptr +="<hr>";
  ptr +="<hr>";
  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
}






///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



void Readconvert(){
    // receivedData = "data=01 11 01 11";  // 1 should be at end
    String receivedData = "";
  while (mySerial.available() > 0) {
    char receivedChar = mySerial.read(); // Read one character at a time from the buffer
    receivedData += receivedChar; // Add the character to the end of the receivedData string
  }
    int a,b,c,d;

    if (receivedData.startsWith("data")) {
  //    Serial.print("Received data from Arduino: ");
     //Serial.println(receivedData); // Print the received data to the serial monitor
  //  String receivedData = "data=1234 1254 5675 1";
  
  
  
  int index = receivedData.indexOf('='); // find the index of the '=' character
  
  if (index != -1) { // if '=' found
    String numbers = receivedData.substring(index + 1); // extract the substring containing the numbers
    int space1 = numbers.indexOf(' '); // find the index of the first space
    int space2 = numbers.indexOf(' ', space1 + 1); // find the index of the second space
    int space3 = numbers.indexOf(' ', space2 + 1); // find the index of the third space
  
    // convert each substring to integer
    a = numbers.substring(0, space1).toInt();
    b = numbers.substring(space1 + 1, space2).toInt();
    c = numbers.substring(space2 + 1, space3).toInt();
    d = numbers.substring(space3 + 1).toInt();
  }


//  sumHum sumTemp sumSmoke PIRValue|pumpStatus|1
  if(a>0 && b>0 && c>0 && d>0){
    humidity = a;
    temp = b;
    smoke = c;
    pir = d/100;
    pumpStatus = d%100;
    pumpStatus = pumpStatus/10;
    
  }

  

  }
}


///////////////////////////////////////////
String IP ="http://";

SH1106 display(0x3C,D1,D2);


#define DEMO_DURATION 3000
typedef void (*Demo)(void);

//int demoMode = 0;

/////////

//
//const char* ssid = STASSID;
//const char* password = STAPSK;
//
//ESP8266WebServer server(80);

//const char* www_username = "cncFoam";
//const char* www_password = "cncSpecial";


//uint8_t LED1pin = D7;
//bool LED1status = LOW;
//float RPM, MaxRPM,t,t1,t2 = 0;
//uint8_t LED2pin = D8;
//bool LED2status = LOW;
//////////////////////////////////////////////// Functin for display

void drawFontFaceDemo() {
  display.clear();
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_10);
  display.drawString(0, 0, "EMBEDDED SYSTEMS");
  display.setFont(ArialMT_Plain_10);
  display.drawString(0, 15, "Smart Farming System");
  display.setFont(ArialMT_Plain_10);
  display.drawString(0, 35, "Team: Nitesh, Pratik,");
  display.setFont(ArialMT_Plain_10);
  display.drawString(0, 50, "Sandeep, Karan");
  display.display();
  delay(3000);
}


void drawTextAlignmentDemo() {
  display.clear();
  // Text alignment demo
  display.setFont(ArialMT_Plain_10);

  // The coordinates define the left starting point of the text
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.drawString(0, 10, "Welcome ");

  // The coordinates define the center of the text
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.drawString(64, 22, "To Smart Farming System");

  // The coordinates define the right end of the text
  display.setTextAlignment(TEXT_ALIGN_RIGHT);
  display.drawString(128, 33, "IIT Gandhinagar");
  display.display();
  delay(3000);
}

void fetchingIPAddress() {
  display.clear();
  // Text alignment demo
  display.setFont(ArialMT_Plain_10);

  // The coordinates define the left starting point of the text
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.drawString(20, 20, "Fetching IP Address....");
  display.display();
  delay(2000);
}

void displayIPAddress() {
  display.clear();
  // Text alignment demo
  display.setFont(ArialMT_Plain_10);

  // The coordinates define the left starting point of the text
    display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.drawString(0, 5, "  Open Website:  ");
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.drawString(0, 25, IP);
  display.display();
  delay(5000);
}

int prg = 0;
void drawProgressBarDemo() {
  display.clear();
  int progress = prg;
  // draw the progress bar
  display.drawProgressBar(0, 32, 120, 10, progress);

  // draw the percentage as String
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.drawString(64, 5, "Initializing...");
  display.drawString(64, 15, String(progress) + "%");
  display.display();
 
  
}


void drawImageDemo() {
  display.clear();
  // see http://blog.squix.org/2015/05/esp8266-nodemcu-how-to-create-xbm.html
  // on how to create xbm files
  display.drawXbm(44, 6, WiFi_Logo_width, WiFi_Logo_height, WiFi_Logo_bits);
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.drawString(25, 50, "IIT Gandhinagar");
  display.display();
  delay(3000);
}

//Demo demos[] = {drawFontFaceDemo, drawTextFlowDemo, drawTextAlignmentDemo, drawRectDemo, drawCircleDemo, drawProgressBarDemo, drawImageDemo};
Demo demos[] = {drawFontFaceDemo, drawTextAlignmentDemo, drawProgressBarDemo, drawImageDemo, displayIPAddress, fetchingIPAddress};
//int demoLength = (sizeof(demos) / sizeof(Demo));
//long timeSinceLastModeSwitch = 0;


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void setup() {
   Serial.begin(9600); // Initialize serial communication with the PC
  mySerial.begin(19200); // Initialize SoftwareSerial communication with the Arduino

  delay(1000);
  Serial.println("*****************");
  Serial.println("***   Welcome to Smart Farming System   ***");
  delay(500);
  pinMode(D5,INPUT_PULLUP);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("WiFi Connect Failed! Rebooting...");
    
    ESP.restart();
  }
  ArduinoOTA.begin();

  pinMode(LED1pin, OUTPUT);
  pinMode(LED2pin, OUTPUT);
  server.on("/", handle_OnConnect);
  server.on("/sysOn", handle_led1on);
  server.on("/sysOff", handle_led1off);
  server.on("/pumpOn", handle_pumpon);
  server.on("/pumpOff", handle_pumpoff);
  server.on("/buzzOn", handle_buzzon);
  server.on("/buzzOff", handle_buzzoff);
  server.onNotFound(handle_NotFound);

  server.begin();
  delay(500);
  Serial.print("Open   ");
  IPAddress ip1 = WiFi.localIP();
  IP = IP+ ip1.toString();
  Serial.print(IP); 
  Serial.println("/   in the browser to see the web page.");
  LED1status=0;

//  WiFi.mode(WIFI_STA);
//  WiFi.begin(ssid, password);
//  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
//    Serial.println("WiFi Connect Failed! Rebooting...");
//   
//    ESP.restart();
//  }
//  ArduinoOTA.begin();
//
//  pinMode(LED1pin, OUTPUT);
//  pinMode(LED2pin, OUTPUT);
//  server.on("/", handle_OnConnect);
//  server.on("/cncScrewon", handle_led1on);
//  server.on("/cncScrewoff", handle_led1off);
//  server.onNotFound(handle_NotFound);
//
//  server.begin();
//  delay(500);
//  Serial.print("Open   http://");
//  Serial.print(WiFi.localIP());
//  Serial.println("/   in the browser to see and control the spirometer");
//  LED1status=0;


    /////////// Initialising the UI will init the display too.
  display.init();

  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);


  demos[3]();
  demos[0]();
  demos[1]();
//  demos[2]();
  while(prg != 100)
  {
     demos[2]();
    delay(50);
    prg = prg + 1;
  }
  demos[5]();
  demos[4]();
  
  //////////////
}

void loop() {
  ArduinoOTA.handle();
  server.handleClient();


  Readconvert();
  // EnPump EnSys_w EnBuzz_w 11
 String data = "data="+String(LED2status)+String(1)+" "+String(LED1status)+String(1)+" "+String(Buzzstatus)+String(1)+" "+String(1)+String(1);
   Serial.print("humidity = ");
  Serial.println(humidity);
  Serial.print("temp = ");
  Serial.println(temp);
  Serial.print("smoke = ");
  Serial.println(smoke);

  mySerial.println(data); // Send the variable containing the string to the Arduino
  // print the values of the four integers



  if(LED1status){
    if(LED2status){
//      Serial.println("Sprinkler started.");
      digitalWrite(LED2pin, HIGH);
      delay(200);
    }
   if(!LED2status){
    digitalWrite(LED2pin, LOW);
    delay(200);
    }
    if(Buzzstatus){
//      Serial.println("Buzzer on.");
    digitalWrite(Buzzpin, HIGH);
    delay(200);
    }
    if(!Buzzstatus){
    digitalWrite(Buzzpin, LOW);
    delay(200);
    }   
  }
  else{
//    Serial.println("System is disabled!");
    digitalWrite(LED1pin, HIGH);
    LED2status=LOW;
    LED2status=LOW; 
    delay(200);}
}
