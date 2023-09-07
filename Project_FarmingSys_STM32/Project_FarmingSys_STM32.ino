/*  ## Setup the board -
 *  Nucleo-L4R5ZI-P board with the Arduino IDE in the STM32duino wiki (https://github.com/stm32duino/wiki/wiki/Getting-Started).
 * 
 * 
 * Library Required  for UART communication with NodeMCU-
 *  SoftwareSerial.h
 * 
 * Connection details - 
 *   connect A0 to  temperature sensor T1 
  connect A1 to temperature sensor T2
  connect A2 to Smoke sensor 1
  connect A3 to smoke sensor 2
  connect A4 to  Mois sensor 1
  connect A5 to Mois sensor 2
  connect D6 to PIR sensor
  connect D5 to  Pump Relay
  connect buzzer at D8 
  connect D10 to D3 and D11 to D2 of NodemCU
 * 
 * 
 * Upload this code to the STM32 board (Nucleo-L4R5ZI-P)
 * Open the 
 */


// setup the threshhold values
#define TEMP1_TH 250  // analog value
#define TEMP2_TH 250

#define SMOKE1_TH 340   // analog Value
#define SMOKE2_TH 340
#define WifiMode 1  // 
#define WifiPriority 1  // set control priority to NodeMCU

#define MOIS_TH 800    // analog value 


// initializing the value in reset condition

int Hum_TH = MOIS_TH *2;
int EnPump = 0;
int EnSys = 0;
int PIRValue = 0;
int EnBuzz = 0;

uint8_t EnSys_W = 1;  // signals from wifi to enable system functions
uint8_t EnPump_W = 0;
uint8_t EnBuzz_W = 0;
//////////////////////////////////////////////////////// Global variables
#include <SoftwareSerial.h>
#define buzzerPin D8  // buzzer is connected to D8 pin 
#define pirPin D6     // PIR sensor is connected to D6 pin
SoftwareSerial mySerial(10, 11); // RX, TX  for UART communication with nodeMCU


long unsigned int lowIn;
long unsigned int pause = 1000;
boolean lockLow = true;
boolean takeLowTime;



int sumHum = 0;
int sumTemp = 0;

int sumSmoke = 2048;

////////////////////////////////////////////////////// Functions

int TempSensor(){
  int temp1 = analogRead(A0);
  int temp2 = analogRead(A1);
  sumTemp = temp1+temp2;
  if(temp1>TEMP1_TH || temp2>TEMP2_TH){
    return 1;
  }
  else {return 0;}
}


int SmokeSensor(){
  int smk1 = analogRead(A2);
  int smk2 = analogRead(A3);
  sumSmoke = smk1+smk2;
  if(smk1>SMOKE1_TH || smk2 >SMOKE2_TH){
    return 1;
  }
  else {return 0;}
}

int MoisSensor(){
    int Hum1 = analogRead(A4);
    int Hum2 = analogRead(A5);
    sumHum = Hum1+Hum2;
    if(sumHum > Hum_TH){
      return 5;
    }
    if(Hum1<Hum_TH/2){
      if(Hum2<Hum_TH/2){
        return 3;
      }
      else {
        return 2;
      }
    }
     else if(Hum2<MOIS_TH/2){
      return 1;
     }
     return 0;
}

void PIRSensor() {
   
   if(digitalRead(pirPin) == HIGH) {
      if(lockLow) {
         PIRValue = 1;
         lockLow = false;     
         //delay(20);
      }
      takeLowTime = true;
   }
   if(digitalRead(pirPin) == LOW) {
      if(takeLowTime){
         lowIn = millis();takeLowTime = false;
      }
      if(!lockLow && millis() - lowIn > pause) {
         PIRValue = 0;
         lockLow = true;
         //delay(20);
      }
   }
}

void Readconvert(){
    // receivedData = "data=01 11 01 11";  // 1 should be at end
    String receivedData = "";
  while (mySerial.available() > 0) {
    char receivedChar = mySerial.read(); // Read one character at a time from the buffer
    receivedData += receivedChar; // Add the character to the end of the receivedData string
  }
    int a,b,c,d;
 //Serial.println(receivedData); // Print the received data to the serial monitor
    if (receivedData.startsWith("data")) {
  //    Serial.print("Received data from Arduino: ");
    
   
  
  
  
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


// EnPump EnSys_w EnBuzz_w 11 
  if(a>0 && b>0 && c>0 && d>0){
    EnPump_W = a/10;
    EnSys_W = b/10;
    EnBuzz_W = c/10;
  }

  
  // print the values of the four integers
  Serial.print("EnPump_W = ");
  Serial.println(EnPump_W);
  Serial.print("EnSys_W = ");
  Serial.println(EnSys_W);
  Serial.print("EnBuzz_W = ");
  Serial.println(EnBuzz_W);

  
  }
}




void buzzer() {
  tone(buzzerPin, 1500); // Set the frequency to 1000 Hz
  delay(1000); // Wait for 1 second
    tone(buzzerPin, 1000); // Set the frequency to 1000 Hz
  delay(1000); // Wait for 1 second
  noTone(buzzerPin); // Stop the sound
}




//////////////////////////////////////////////////////////////

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); // Initialize serial communication with the PC
  mySerial.begin(19200); // Initialize serial communication with the NodeMCU
  pinMode(A0,INPUT);  // T1
  pinMode(A1,INPUT);  //T2
  pinMode(A2,INPUT);  //Smoke 1
  pinMode(A3,INPUT);  //smoke 2
  pinMode(A4,INPUT);  // Mois 1
  pinMode(A5,INPUT);  //Mois 2
  
  
  pinMode(D6,INPUT);  // PIR sensor
  
  
  pinMode(D5,OUTPUT);   // Pump Relay
  pinMode(D9,OUTPUT);   // Pump Relay
  pinMode(buzzerPin,OUTPUT);  // buzzer at D8 
  //pinMode(D9,OUTPUT);  // Servo

  // connect D10 to D3 and D11 to D2 of NodemCU
}

int temp = 0;
int mois = 0;
int smoke = 0;


//uint8_t EnPump = 0;
//uint8_t EnBuzz = 0;

uint8_t pumpStatus = 0;  // pump is on or off

void loop() {
  // put your main code here, to run repeatedly:
  Readconvert();
  // data= "data=sumHum sumTemp sumSmoke PIRValue|pumpStatus|1"

  String data = "data="+String(sumHum)+" "+String(sumTemp)+" "+String(sumSmoke)+" "+String(PIRValue)+String(pumpStatus)+String(1);
  mySerial.println(data); // Send the variable containing the string to the NodeMCU


  bool x = digitalRead(D0);
  Serial.println(x);
  bool x2 = digitalRead(D2);
  bool x1 = digitalRead(D1);
  
  if(WifiMode){
    if(WifiPriority){
      EnSys = EnSys_W;
      EnPump = EnPump_W;
      EnBuzz = EnBuzz_W;
    }
    
    else if (x){
      EnSys = 1;
    }
    else if (EnSys_W){
      EnSys = 1;
    }
    else{
      EnSys = 0;
    }
    
    if (x2 && WifiPriority==0){
      EnPump = 1;
    }
    else if (EnPump_W){
      EnPump = 1;
    }
    else{
      EnPump = 0;
    }

    
    if (x1 && WifiPriority==0){
      EnBuzz = 1;
    }
    else if (EnBuzz_W){
      EnBuzz = 1;
    }
    else{
      EnBuzz = 0;
    }   
    
  }
  else {
    EnSys = digitalRead(D0);
    EnPump = digitalRead(D2);
    EnBuzz = digitalRead(D1);
  }
  if(EnSys){
     temp = TempSensor();
     mois = MoisSensor();
     smoke = SmokeSensor();

      if(EnPump){
        pumpStatus =1;
      }
     else if(mois == 5){
      pumpStatus =1;
     }
     else if(mois == 3){
      pumpStatus =0;
     }
     
      
     if(EnPump){
        pumpStatus =1;
      }
     else if(temp==1 && smoke==1){
      pumpStatus =1;
     }
     else{
      pumpStatus =0;
     }
     
    if(EnBuzz){
      PIRSensor();
      digitalWrite(D8,PIRValue);
    }
    else{
      digitalWrite(D8,PIRValue);
    }
    
     
    
    
      
  }
  else{
     EnPump = 0;
     EnBuzz = 0;
     pumpStatus = 0;
     digitalWrite(D8,0);
  }

  
if (pumpStatus){
  digitalWrite(D5,1);
}
else{
  digitalWrite(D5,0);
}
Serial.print(" sumHum sumTemp sumSmoke ");
Serial.print(sumHum);
Serial.print(" ");
Serial.print(sumTemp);
Serial.print(" ");
Serial.println(sumSmoke);
digitalWrite(D9,HIGH);

}
