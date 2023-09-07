########## ES333 Microprocessors and Embedded System ############# 

**************** SMART FARMING SYSTEM ************************


1. Upload the code from Project_FarmingSys_STM32 folder to STM32 board.

2. Make connections - 
  connect A0 to  temperature sensor T1 
  connect A1 to temperature sensor T2
  connect A2 to Smoke sensor 1
  connect A3 to smoke sensor 2
  connect A4 to  Mois sensor 1
  connect A5 to Mois sensor 2
  connect D6 to PIR sensor
  connect D5 to  Pump Relay
  connect buzzer at D8 
  connect D10 to D3 and D11 to D2 of NodemCU

3. For NodeMCU -

	a) import library from zip file esp8266-oled-ssd1306-master.
	b) Upload code in the folder Server_and_Display_NodeMCU to NodeMCU.
	   Note - Make sure image.h file is in the same folder in which Server_and_Display.ino file is stored.   