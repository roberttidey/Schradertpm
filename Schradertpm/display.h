//#define USE_ADAFRUIT 1
#define USE_TEXTONLY 1

extern void ResetSignalRefreshTimer();

#ifndef USE_ADAFRUIT

	#include "SSD1306Ascii.h"
	#include "SSD1306AsciiWire.h"

	SSD1306AsciiWire display;

	void ShowTitle() {
		//display.clear();

		display.set1X(); // Normal 1:1 pixel scale
		//display.setTextColor(WHITE, BLACK);       // Draw white text

		display.setCursor(0, 0);
		#ifdef NissanLeaf
			display.println(F("Nissan Leaf TPMS"));
		#elif Dacia
			display.println(F("Dacia TPMS"));
		#elif Renault
			#ifdef Zoe
				display.println(F("Ren Zoe(early) TPMS"));
			#else
				display.println(F("Renault TPMS"));
			#endif
		#elif Citroen
			display.println(F("Citroen TPMS"));
		#elif Jansite
			display.println(F("Jansite TPMS"));
		#elif JansiteSolar
			display.println(F("JSolar TPMS"));
		#elif Ford
			display.println(F("Ford TPMS"));
		#elif PontiacG82009
			display.println(F("Pontiac G8 TPMS"));
		#elif Hyundai_i35
			display.println(F("Hyundai i35 TPMS"));
		#elif Schrader_C1100
			display.println(F("Hyundai Tucson"));
		#else
			display.println(F("Toyota TPMS"));
		#endif
		display.print(F(" JSM Solutions "));
		display.print(VERSION);
		//display.println(")");
	}

	char DisplayTimeoutBar(unsigned long TimeSinceLastUpdate) {
		int HowCloseToTimeout;
		HowCloseToTimeout = (int)(TimeSinceLastUpdate/(TPMS_TIMEOUT/5));

		switch(HowCloseToTimeout) {
			case 0: 
				//return(FONTBAR_7);
				return('5');
				break;
			case 1: 
				//return(FONTBAR_5);
				return('4');
				break;
			case 2: 
				//return(FONTBAR_3);
				return('3');
				break;
			case 3: 
				//return(FONTBAR_2);
				return('2');
				break;
			case 4: 
				//return(FONTBAR_1);
				return('1');
				break;
			default: 
				//return(FONTBAR_0);
				return('0');
				break;
					  
		}
	}


	int GetBlockStartX(byte i) {
		switch (i) {
			case 0:
				return(0);
				break;
			case 1:
				return(59);
				break;
			case 2:
				return(0);
				break;
			case 3:
				return(59);
				break;
		}
	}

  
	int GetBlockStartY(byte i) {
		switch (i) {
			case 0:
				return(2);
				break;
			case 1:
				return(2);
				break;
			case 2:
				return(5);
				break;
			case 3:
				return(5);
				break;
		}
	}

	void ClearDisplayBlock(byte i) {
		int x,y;

		x = GetBlockStartX(i);
		y = GetBlockStartY(i);

		display.setFont(Adafruit5x7);
		display.set2X();
		display.clearField(x,y,4);
		display.set1X();
		display.clearField(x,y+2,8);
     
	}




	void UpdateBlock(int i) {
		int x,y;
		char s[6];
		char t;

		if ((TPMS[i].LowPressure == true) || (TPMS[i].HighPressure == true)) {
			if (DisplayFlash) {
				strcpy(s,"    ");
			} else {
				#ifdef DISPLAY_PRESSURE_AS_BAR
					dtostrf(PSI_To_BAR(TPMS[i].TPMS_Pressure), 4, 2, s);
				elif DISPLAY_PRESSURE_AS_KPA
					dtostrf(PSI_To_KPA(TPMS[i].TPMS_Pressure), 3, 0, s);  //rounded to integer value
				#else
					dtostrf(TPMS[i].TPMS_Pressure, 3, 1, s);
				#endif
			}
		} else {
			#ifdef DISPLAY_PRESSURE_AS_BAR
				dtostrf(PSI_To_BAR(TPMS[i].TPMS_Pressure), 4, 2, s);
			#elif DISPLAY_PRESSURE_AS_KPA
				dtostrf(PSI_To_KPA(TPMS[i].TPMS_Pressure), 3, 0, s);  //rounded to integer value
			#else
				dtostrf(TPMS[i].TPMS_Pressure, 3, 1, s);
			#endif
		}
       
		x = GetBlockStartX(i);
		y = GetBlockStartY(i);
		display.setCursor(x, y);
        display.setFont(Adafruit5x7);
        display.set2X();
		//display.clearField(x,y,4);
		display.print(s);
		display.setCursor(x, y+2);
		display.setFont(Adafruit5x7);
		display.set1X();
		//display.clearField(x,y+2,8);

		#ifdef DISPLAY_TEMP_AS_FAHRENHEIT
			t = 'F';
		#else
			t = 'C';
		#endif

		if (TPMS[i].TPMS_Temperature == NO_VALID_TEMPERATURE) {
			display.print("  ---  ");
        } else {
			if ((TPMS[i].LowTemperature == true) || (TPMS[i].HighTemperature == true)) {
				if (DisplayFlash) {
					strcpy(s,"       ");
					display.print(s); 
				} else {
					#ifdef DISPLAY_TEMP_AS_FAHRENHEIT
						dtostrf(DegC_To_DegF(TPMS[i].TPMS_Temperature), 2, 0, s);
					#else
						dtostrf(TPMS[i].TPMS_Temperature, 2, 0, s);
					#endif
					display.print(" ");
					display.print(s);
					display.setFont(System5x7);
					display.print(char(128));  //degrees symbol
					display.setFont(Adafruit5x7);
					display.print(t);
					display.print("  ");
				}
			} else {
				#ifdef DISPLAY_TEMP_AS_FAHRENHEIT
					dtostrf(DegC_To_DegF(TPMS[i].TPMS_Temperature), 2, 0, s);
				#else
					dtostrf(TPMS[i].TPMS_Temperature, 2, 0, s);
                #endif
				display.print(" ");
				display.print(s);
				display.setFont(System5x7);
				display.print(char(128));  //degrees symbol
				display.setFont(Adafruit5x7);
				display.print(t);
				display.print("  ");
			}
		}
        display.setFont(System5x7);          
        display.print(DisplayTimeoutBar(millis() - TPMS[i].lastupdated));
	}
 
	void UpdateDisplay() {
		int i;
		int x = 0;
		int y = 0;
		char s[6];
  
		for (i = 0; i < 4; i++) {
      
			if (TPMS[i].TPMS_ID != 0) {
				//Only update the areas which need it to keep the timing overheads down
					UpdateBlock(i);
					if ((bitRead(TPMSChangeBits,i) == 1)) {
					bitClear(TPMSChangeBits,i);
				}
			}
		}
	}

#else
	#include <Adafruit_GFX.h>
	#include <Adafruit_SSD1306.h>


	#define SCREEN_WIDTH 128 // OLED display width, in pixels
	#define SCREEN_HEIGHT 64 // OLED display height, in pixels
	Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT,  &Wire, -1);


	void ShowTitle() {
		display.clearDisplay();
		display.setFont(Adafruit5x7);
		display.setTextSize(1);             // Normal 1:1 pixel scale
		display.setTextColor(WHITE, BLACK);       // Draw white text

		display.setCursor(0, 0);
		#ifdef NissanLeaf
			display.print("Nissan Leaf TPMS");
		#elif Dacia
			display.print("Dacia TPMS");
		#elif Renault
			display.print("Renault TPMS");
		#elif Citroen
			display.print("Citroen TPMS");
		#elif Citroen
			display.println("Citroen TPMS Monitor");
		#elif Jansite
			display.println("Jansite TPMS Monitor");
		#elif JansiteSolar
			display.println("JSolar TPMS Monitor");
		#elif Citroen
			display.println("Citroen TPMS Monitor");
		#elif Ford
			display.print("Ford TPMS");
		#elif PontiacG82009
			display.print("Pontiac G8 TPMS");
		#else
			display.println("Toyota TPMS Monitor");
		#endif
    
		display.print(" JSM Solutions ");
		display.print(VERSION);
		//display.println(")");
	}
  
	void UpdateDisplay() {
		int i;
		int x = 0;
		int y = 0;
		char s[6];

		ShowTitle();

		display.setFont(Adafruit5x7);
		display.setTextSize(2);

		for (i = 0; i < 4; i++) {
			switch (i) {
				case 0:
					x = 0;
					y = 16;
					break;
				case 1:
					x = 64;
					y = 16;
					break;
				case 2:
					x = 0;
					y = 48;
					break;
				case 3:
					x = 64;
					y = 48;
					break;
			}
			display.setCursor(x, y);
  
			if (TPMS[i].TPMS_ID != 0) {
				dtostrf(TPMS[i].TPMS_Pressure, 3, 1, s);
				//sprintf(temperature,"%s F", str_temp);
				//sprintf(s,"%.1f",TPMS[i].TPMS_Pressure);
				display.print(s);
			}
		}
		display.display();
	}
   
#endif
