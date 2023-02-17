//Changes:
//V1.0 - Originally published version
//V2.0 - Altered to allow support for spare tyre reporting (although not displayed on LCD display)
//V3.0 - Added #define in globals.h to switch between BAR and PSI
//V4.0 - Now uses synchronous data from CC1101 and option of using either the SPI or the hardware pin for the CD signal (#define USE_HW_CD in globals.h). Also tideied up debug information if required (globals.h #defines)
//V5.0 - Corrected possible sync error when looking for start of data stream. Fix should improve hit rate on valid TPMS detection
//V5.1 - Added freq offset updates (SWRA159) - removed again(makes it worse!)
//V5.1.1 - Added functions for extracting RSSI as dbm
//V6.0 - Added code for options for US (315MHz) or UK (433MHz), also for different sensor types (PMV-C210, PMV-107J)
//V6.1 - Added support for Seeeduino Xiao
//V6.2 - Included support for Renault (Renault Clio, Renault Captur and maybe Dacia Sandero?) as an option (to be verified)
//V6.3 - Included support for Citroen (possibly also includes Peugeot and likely Fiat, Mitsubishi, VDO-types) as an option (to be verified)
//V6.4 - Some code rationalisation, all sensor timings now in Configs.h
//V6.5 - Tried to improve PMV-107J decode by starting at the end of the buffer and working forwards
//V6.6 - Added other car types (under development), changed PMV-107J frequency deviation, added #define as option to only detect specified IDs, stripped leading 4 bits from ID (as per 'Glopes' comment) for Toyota, moved some globals settings to Config.h, changed AGC settings 
//V6.7 - Adjusted PMV_C210 presure calculation to lower PSI by 0.3 (as suggested by Steven Andrews by comparing with Autel programmer). 
//     - Added option for displaying Fahrenheit (#ifdef DISPLAY_TEMP_AS_FAHRENHEIT in configs.h) rather than centigrade.
//     - Re-introduced flashing display if pressures outside of pre-set limits (enabled by #define ENABLE_PRESSURE_ALARMS in configs.h. PressureLowLimits and PressureHighLimits in config.h should be set as required)
//V7.0 - Included option for 1.8" TFT display (requires Seeeduino Xiao for extra memory)
//V8.0 - Added optional audible alarm for pressure alarms. Improved display flashing. Tested with Nissan Leaf 433MHz OK
//V9.0 - Added Pontiac (US) decoder option, moved all CC1101 configs to sensor-spcific include files, improved timer handling. Added temperature compensation for pressure limits (set pressure limits reference temperature in configs.h). Added kPA pressure display option.
//       Pressures always stored locally as psi and temperatures as deg C - including limits (converted if required for the display).
//       Tyre pressure and temperature limits are assumed to be in the required units e.g. bar, kpa, psi, degC, degF depending on config choice.
//       Added support for 240x240 round display (Steven Andrews' display code)
//V9.1 - Added provisional support for Arduino Nano 33 IOT board
//V9.2 - Fixed bugs in audible alarm for temperatures.. Fixed missing F/C for round display.
//V9.3 - Corrected some title positioning for display.h, Added support for Renault Zoe (pre 07/2019)
//V9.4 - Corrected compile error when not using display - thanks Zaran on Hackster.io
//V9.5 - Changes to cope with multiple tyre sets (Summer/Winter). Corrected bug in ID matching (IDs were allocated to PROGMEM but referenced as SRAM for lookups), this meant tyre position was not being reported correctly. Moved Ref[3] to globals.
//V9.6 - Fixed bug where lcd display was partially obliterated if TYRECOUNT > 4 (the default is now 5)
//V9.7 - Fixed bug in Ford.h which caused out of range temperture values to be displayed on screen when temperture below 0C. 
//       Changes to support new larger screen type (supplied by Steven Andrews' - thank you!). 2.4inch LCD Display Module 65K RGB Colors, ILI9341 2.4" TFT 240320. Requires Adafruit ILI9341 library.
//V9.8 - Added support for Truck Solar TPMS and Mega256 build
//V9.9 - Added support for Ford strange Ford E-series (Tom Logan). It appears temperature format is reported in 2 ways depending upon whether or not the sensor was manually triggered (or not)
//V10.0 - Improved Ford incoming message handling
//V10.1 - Changed Ford pressure decode in line with actual E-series Schrader sensor feedback. This sensor also seems to delay sending temperatures after sleeping and sends a counter instead.
//V10.2 - Missing conditional for 2.4" screen in setup in .ino file (line 350) preventing initialisation of display (USE_24_INCH_ILI9341_DISPLAY). Causes screen to continually display white.
//V10.3 - Changed PMV-107J & PMV-C210 to try to improve detections. Code now walks through the received bit stream until a valid checksum match is found (or runs out of data)
//        This avoids the need to look for sync bits which may not be reliable at the start of the bit stream (includes change to common.h)
//V10.4 - Added trial for Toyota Sienna (TRW C070 sensor)
//V10.5 - Improvements to Toyota Sienna TRW C070 detection/validation. Added Hyundai I35 as option for this same sensor. 
//        Fixed temperature display bug (thanks Larry on Hackster for pointing this out.) - it would overflow/underflow with certain readings
//        Feedback from Alistair Clark on Hackster indicates that the C210 decode also works for Toyota Corolla (2019-22 PMV-C215 sensor)
//V10.6 - Corrected slight bug in display.h for Hyundai (should have been newline print)/ Thanks to grigata on Hackster for pointing this out.
//          Reported by gamer765 that PMV-107J also works for PMV-C11A sensors.

#define VERSION "10.6"

#ifdef ESP8266
	#include "BaseConfig.h"
#endif
#include <SPI.h>
#include "configs.h"
#include "globals.h"
#ifdef ESP8266
	#include <TickTwo.h>
#else
	#include <Ticker.h>
#endif

#include "CommonFunctionDeclarations.h"

#ifdef USE_LCDDISPLAY
	#ifdef USE_1_INCH_YB_I2C_DISPLAY 
		#include <Wire.h>
		#include "display.h"
	 #elif USE_2_INCH_ST7753_DISPLAY 
		#include "display_128x160.h"
	 #elif USE_2_INCH_ST7789_DISPLAY
		#include "display_240x240round.h"
	 #elif USE_24_INCH_ILI9341_DISPLAY
		#include "display_240x320.h"
	 #endif
#endif

#ifdef ENABLE_AUDIBLE_ALARM
	 #include "AudibleAlarm.h"
#endif

#ifdef Toyota_PMV_C210
	 #include "Toyota_PMV_C210.h"
#elif Toyota_PMV_107J
	 #include "Toyota_PMV_107J.h"
#elif defined(Toyota_TRW_C070) || defined(Hyundai_i35)
	 #include "Toyota_TRW_C070.h"
#elif defined Schrader_C1100
	 #include "Schrader_C1100.h"
#elif NissanLeaf
	 #include "Renault.h"
#elif Dacia
	 #include "Renault.h"
#elif  Renault
	 #include "Renault.h"
#elif Citroen
	 #include "Citroen.h"
#elif Ford
	 #include "Ford.h"
#elif  Jansite
	 #include "Jansite.h"
#elif  JansiteSolar
	 #include "JansiteSolar.h"
#elif PontiacG82009
	 #include "PontiacG82009.h"
#elif TruckSolar
	 #include "TruckSolar.h"
#endif

#include "cc1101.h"
#include "Common.h"

#ifdef ESP8266
	TickTwo displayflashtimer(DisplayTimerExpired,NOBLANK_MS, 0, MILLIS);
	TickTwo SignalRefreshTimer(SignalRefreshRequired, SIGNALREFRESHTIMING, 0, MILLIS);
#else
	Ticker displayflashtimer(DisplayTimerExpired,NOBLANK_MS, 0, MILLIS);
	Ticker SignalRefreshTimer(SignalRefreshRequired, SIGNALREFRESHTIMING, 0, MILLIS);
#endif

void UpdateTimers() {
	#ifdef USE_LCDDISPLAY
		 SignalRefreshTimer.update();
	#endif
	
	#if defined(ENABLE_PRESSURE_ALARMS) || defined(ENABLE_TEMPERATURE_ALARMS)
		 displayflashtimer.update();
		 #ifdef ENABLE_AUDIBLE_ALARM
			if (Audible_Alarm_Running) {
				 AudibleAlarmSoundTimer.update();  //check the timers
				 AudibleAlarmReminderTimer.update();
			}

		 #endif
		 
	#endif

}

void CheckForScreenUpdates() {
	UpdateTimers();
	#ifdef USE_LCDDISPLAY
		#if defined(ENABLE_PRESSURE_ALARMS) || defined(ENABLE_TEMPERATURE_ALARMS)
			if (DisplayFlashExpired || (TPMS_Changed == true) || Check_TPMS_Timeouts() || SignalRefreshNeeded ) {
				//display update required
				UpdateDisplay();
				if (DisplayFlashExpired) {
					  DisplayFlashExpired = false;
				}
				TPMS_Changed = false;
				if (SignalRefreshNeeded == true) {
					 SignalRefreshNeeded = false;
				}
			}
		#else
			if ((TPMS_Changed == true) || Check_TPMS_Timeouts() || SignalRefreshNeeded ) {
				UpdateDisplay();
				TPMS_Changed = false;
				if (SignalRefreshNeeded == true) {
					 SignalRefreshNeeded = false;
				}
			} 
		#endif
	#endif
}

void SendDebug(String Mess) {
	Serial.println(Mess);
}

#ifdef ESP8266
	void handleGetData() {
		String response;
		int i;
		
		for(i = 0; i < TYRECOUNT; i++) {
			response += String(i) + ",";
			response += String(TPMS[i].TPMS_ID) + ",";
			response += String(TPMS[i].TPMS_Pressure) + ",";
			response += String(TPMS[i].TPMS_Temperature) + "<BR>";
		}	
		server.send(200, "text/html", response);
	}

	void handleSetMode() {
		int setMode = server.arg("mode").toInt();
		if(setMode == 1) useTestTimings = 1;
		if(setMode == 0) useTestTimings = 0;
		Serial.println("SetMode:" + String(setMode));
		server.send(200, "text/html", "useTestTimings:" + String(useTestTimings));
	}


	void setupStart() {
	}

	void extraHandlers() {
		server.on("/getData", handleGetData);
		server.on("/setMode", handleSetMode);
	}
#endif

#ifdef ESP8266
void setupEnd() {
#else
	void setup() {
#endif		
	byte resp;
	unsigned int t;
	int LEDState = LOW;
	int i;
	int mcount;
	int regfail;

	//SPI CC1101 chip select set up
	pinMode(CC1101_CS, OUTPUT);
	digitalWrite(CC1101_CS, HIGH);
	pinMode(LED_RX, OUTPUT);
	pinMode(RXPin, INPUT);
	pinMode(CDPin, INPUT);

	delay(2000);

	#ifdef ENABLE_AUDIBLE_ALARM
		pinMode(AUDIBLE_ALARM_PIN, OUTPUT);
		digitalWrite(AUDIBLE_ALARM_PIN,!Audible_AlarmPin_Active);
		SPI.begin();

		AudibleAlarm(true);  //audible alarm test
		delay(200);
		AudibleAlarm(false);
		delay(300);
		AudibleAlarm(true);  //audible alarm test
		delay(200);
		AudibleAlarm(false);
		delay(500);
		AudibleAlarmReminderTimer.stop();
		delay(800);
	#else
		SPI.begin();
		delay(2000);
	#endif

	Serial.println(F(""));
	Serial.println(F(""));
	Serial.println(F("########################################################################"));
	Serial.println(F(""));
	Serial.println(F("STARTING..."));
	Serial.println(PROC_TYPE);

#ifdef USE_LCDDISPLAY 
	#ifdef USE_1_INCH_YB_I2C_DISPLAY 
		#if USE_ADAFRUIT
			if (!display.begin(SSD1306_EXTERNALVCC, I2C_ADDRESS)) {
				Serial.println(F("SSD1306 allocation failed"));
				while(1) {
					delay(10);// Don't proceed, loop forever
				}
			}
			display.clearDisplay();
			display.display();
		#else
			#ifdef ESP8266
				Wire.begin(I2CSDAPin, I2CSCLPin);
			#else
				Wire.begin();
			#endif
			Wire.setClock(400000L);
			display.begin(&Adafruit128x64, I2C_ADDRESS);
			display.setFont(Adafruit5x7);
			display.clear();
			ShowTitle();
		#endif
		Serial.println(F("SSD1306 initialised OK"));
	 #endif
#endif

	//initialise the CC1101

	Serial.print(F("Resetting CC1101 "));
	byte retrycount = 0;
	while (retrycount < 5) {
		 Serial.print(F("."));
		 CC1101_reset();
		 if (readConfigReg(0) == 0x29)
				break;
		 retrycount++;
		 delay(5);
	}
	Serial.println(F(""));

	if (readConfigReg(0) == 0x29) {
		Serial.println(F("CC1101 reset successful"));
	}
	else {
		Serial.println(F("CC1101 reset failed. Try rebooting"));
		#ifdef USE_LCDDISPLAY 
			 #ifdef USE_2_INCH_ST7753_DISPLAY
					DisplayInit();
					DrawTitle(); 
					DisplayWarning("CC1101 reset failed", 0, 64);
					DisplayWarning("Power off/on", 0,80); 
					while(1) {
						delay(10);// Don't proceed, loop forever
					}
			 #endif
		#endif   
	}


	ConfigureCC1101();
	Serial.print(F("CC1101 configured for "));
	#ifdef US_315MHz
		 Serial.print (F("US (315MHz)"));
	#else
		 Serial.print (F("UK (433MHz)"));
	#endif

	#ifdef Toyota_PMV_C210
		 Serial.println (F(" and PMV-C210 TPMS sensor"));
	#elif Toyota_PMV_107J
		 Serial.println (F(" and PMV-107J TPMS sensor"));
	#elif Toyota_TRW_C070
		 Serial.println (F(" and TRW-C070 TPMS sensor"));
	#elif Hyundai_i35
		 Serial.println (F(" and Hyundai i35 (TRW-C070) TPMS sensor"));
	#elif NissanLeaf
		 Serial.println (F(" and Nissan Leaf(Renault) TPMS sensor"));
	#elif Dacia
		 Serial.println (F(" and Dacia (Renault) TPMS sensor"));    
	#elif Renault
			 #ifdef Zoe
					Serial.println("and Renault Zoe(pre 07/2019 X10) TPMS sensor");
			 #else
					Serial.println("and Renault TPMS sensor");
			 #endif
	#elif Citroen
		 Serial.println (F(" and Citroen TPMS sensor"));
	#elif Ford
		 Serial.println (F(" and Ford TPMS sensor"));
	#elif Jansite
		 Serial.println (F(" and Jansite TPMS sensor"));
	#elif JansiteSolar
		 Serial.println (F(" and Jansite-Solar TPMS sensor"));
	#elif PontiacG82009
		 Serial.println (F(" and Pontiac TPMS sensor"));
	#endif

	setIdleState();
	digitalWrite(LED_RX, LED_OFF);

	resp = readStatusReg(CC1101_PARTNUM);
	Serial.print(F("CC1101 Part no: "));
	Serial.println(resp, HEX);

	resp = readStatusReg(CC1101_VERSION);
	Serial.print(F("CC1101 Version: "));
	Serial.println(resp, HEX);

	regfail = VerifyCC1101Config();
	if (regfail > 0)
	{
		Serial.print(F("Config verification fail #"));
		Serial.println(regfail);
	}
	else {
		 Serial.println(F("Config verification OK"));
	}

	#ifdef USE_LCDDISPLAY 
		#ifdef USE_2_INCH_ST7753_DISPLAY
			ScreenSetup();
		#elif USE_2_INCH_ST7789_DISPLAY
			ScreenSetup();
		#elif USE_24_INCH_ILI9341_DISPLAY
			ScreenSetup();
		#endif
	#endif 

	digitalWrite(LED_RX, LED_ON);
	LEDState = HIGH;

	pinMode(DEBUGPIN, OUTPUT);
	digitalWrite(DEBUGPIN, LOW);

	InitTPMS();
	PrintIDs();
	digitalWrite(LED_RX, LED_OFF);

	//Calibrate();
	LastCalTime = millis();
	DisplayTimer = millis();
	DisplayFlash  = false;
	
	setRxState();
	Flush_RX_FIFO(true);

	 #if defined(ENABLE_PRESSURE_ALARMS) || defined(ENABLE_TEMPERATURE_ALARMS)
		 displayflashtimer.start();
	#else
		 displayflashtimer.stop();
	#endif

	#ifdef USE_LCDDISPLAY
			SignalRefreshTimer.start();
	#endif
}

void stateMachine() {
	int i;
	static long lastts = millis();
	int regfail;
	switch(state) {
		case STATE_CALCHECK :
			if (millis() - LastCalTime > CAL_PERIOD_MS) {
					setIdleState();  //configuration is set to auto-cal when goimg from Idle to RX
					Calibrate();
					LastCalTime = millis();
					setRxState();      
			}
			if(useTestTimings) {
				state = STATE_TEST;
			} else {
				state = STATE_WAITCARRIERLOW;
			}
			break;
		case STATE_TEST :
				//Used for hard-coded test time testing only
				if ((millis() - lastts) >= 10000) {
					//run test every 10 seconds
					UpdateTimers(); 
					InitDataBuffer();
					ReceiveMessage();
					lastts = millis();  //reset 10 second timer
				}
				#ifdef USE_LCDDISPLAY
					 CheckForScreenUpdates();
				#endif
				state = STATE_CALCHECK;
			break;
		case STATE_WAITCARRIERLOW :
			// check timers...
			if(GetCarrierStatus() == false) {
				state = STATE_WAITCARRIERHIGH;
			} else {
				#ifdef USE_LCDDISPLAY
					 CheckForScreenUpdates();
				#endif
				state = STATE_CALCHECK;
			}
			break;
		case STATE_WAITCARRIERHIGH :
			UpdateTimers(); 
			InitDataBuffer();
			if(GetCarrierStatus() == true) 	{
				ReceiveMessage();
				#ifdef USE_LCDDISPLAY
					 CheckForScreenUpdates();
				#endif
				state = STATE_CALCHECK;
			} else {
				if (Get_RX_FIFO_Count() > 0) {
					Flush_RX_FIFO(true);
				}
				CheckForScreenUpdates();
				delay(1);
			}
			break;
	}
}

void loop() {
	stateMachine();
#ifdef ESP8266
	server.handleClient();
#endif
}
	
