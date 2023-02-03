
#include "bitmap.h"
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ILI9341.h>

#define TFT_DC 4
#define TFT_CS 5
#define TFT_RST 3

//Display Pixel Buffer
#define DB 3

//Tweak Colors
#define ILI9341_ORANGEM 0xF300
#define ILI9341_GREENM  0x129B
//#define ILI9341_GREENM  0x06C5

// Hardware SPI on Feather or other boards
Adafruit_ILI9341 display = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);


  byte DisplayTimeoutBar(unsigned long TimeSinceLastUpdate)
  {
      int HowCloseToTimeout;
      HowCloseToTimeout = (int)((TimeSinceLastUpdate)/(TPMS_TIMEOUT/5));

      switch(HowCloseToTimeout)
      {
        case 0: 
           //return(FONTBAR_7);
           return(5);
           break;
        case 1: 
           //return(FONTBAR_5);
           return(4);
           break;
        case 2: 
           //return(FONTBAR_3);
           return(3);
           break;
        case 3: 
           //return(FONTBAR_2);
           return(2);
           break;
        case 4: 
           //return(FONTBAR_1);
           return(1);
           break;
        default: 
           //return(FONTBAR_0);
           return(0);
           break;
                      
      }
  }

void PrintFreq()
{
  //display.fillRect(0,0,240,320,ILI9341_BLACK);
  //display.setCursor(118,0);
  display.setCursor(275,18);
  #ifdef US_315MHz
     display.setTextSize(1);
     display.print("315 MHz");
  #else
     display.print("433 MHz");
  #endif
}


  void DrawTitle()
{
  //background header
  display.fillRect(0, 0,320,25 + DB, ILI9341_GREENM);
  //display.fillRect(0, 215,320,25 + DB, ILI9341_GREENM);
    
  //display.setCursor(2,0);
 // display.setCursor(2,0 + DB);
  display.setCursor(80,4 + DB);
  display.setTextColor(ILI9341_BLACK);
  display.setTextSize(2);
    #ifdef NissanLeaf
       display.print(F("Nissan Leaf TPMS"));
    #elif Dacia
       display.print(F("Dacia TPMS"));
    #elif Renault
       #ifdef Zoe
          display.println(F("Ren Zoe(early) TPMS"));
       #else
          display.println(F("Renault TPMS"));
       #endif
    #elif Citroen
       display.print(F("Citroen TPMS"));
    #elif Jansite
       display.print(F("Jansite TPMS"));
    #elif JansiteSolar
       display.print(F("JSolar TPMS"));
    #elif Ford
       display.print(F("Ford F250 TPMS"));
    #elif PontiacG82009
       display.print(F("Porsche Carerra"));
    #elif Hyundai_i35
       display.print(F("Hyundai i35"));
    #elif Schrader_C1100
       display.print(F("Hyundai Tucson"));
    #else
       display.print(F("Toyota TPMS"));
    #endif
  PrintFreq();  
  display.setCursor(56,8 + DB);
  display.setTextColor(ILI9341_BLACK);
  display.setTextSize(1);
  display.print(" ");
  //display.print(VERSION);
}

void DrawBackground()
{
     int LineSplitCol = 136;
     
     //display.drawBitmap(0, 0, car_bmp_240x240_car, 240, 240, ILI9341_WHITE);
     //display.drawLine(8,LineSplitCol,115,LineSplitCol,ILI9341_ORANGEM);
     //display.drawLine(125,LineSplitCol,232,LineSplitCol,ILI9341_ORANGEM);
     display.drawLine(5,LineSplitCol,150,LineSplitCol,ILI9341_ORANGEM);
     display.drawLine(170,LineSplitCol,315,LineSplitCol,ILI9341_ORANGEM);

}

void DrawSignal(byte Level, int x, int y)
{
 
       
  if (Level >=1)
  {
     display.fillRect(x+8, y ,4,4, ILI9341_GREENM);
  }
  else
  {
     display.fillRect(x+8, y ,4,4, ILI9341_BLACK);
     display.drawRect(x+8, y ,4,4, ILI9341_GREENM);
  }
       
  if (Level >=2)
  {
     display.fillRect(x+11, y -2,4,6, ILI9341_GREENM);
  }
  else
  {
     display.fillRect(x+11, y -2,4,6, ILI9341_BLACK);
     display.drawRect(x+11, y -2,4,6, ILI9341_GREENM);
  }
       
  if (Level >=3)
  {
     display.fillRect(x+14, y -4,4,8, ILI9341_GREENM);
  }
  else
  {
     display.fillRect(x+14, y -4,4,8, ILI9341_BLACK);
     display.drawRect(x+14, y -4,4,8, ILI9341_GREENM);
  }
  
       
  if (Level >=4)
  {
     display.fillRect(x+17, y-6,4,10, ILI9341_GREENM);
  }
  else
  {
     display.fillRect(x+17, y -6,4,10, ILI9341_BLACK);
     display.drawRect(x+17, y -6,4,10, ILI9341_GREENM);
  }
       
  if (Level >=5)
  {
     display.fillRect(x+20, y -8,4,12, ILI9341_GREENM);
  }
  else
  {
     display.fillRect(x+20, y -8,4,12, ILI9341_BLACK);
     display.drawRect(x+20, y -8,4,12, ILI9341_GREENM);
  }  
      
}

unsigned long testText()
{
  display.fillScreen(ILI9341_BLACK);
  unsigned long start = micros();
  display.setCursor(0, 0);
  display.setTextColor(ILI9341_WHITE);
  display.setTextSize(1);
  display.println("Hello World!");
  display.setTextColor(ILI9341_YELLOW);
  display.setTextSize(2);
  display.println(1234.56);
  display.setTextColor(ILI9341_RED);
  display.setTextSize(3);
  display.println(0xDEADBEEF, HEX);
  display.println();
  display.setTextColor(ILI9341_GREEN);
  display.setTextSize(5);
  display.println("Groop");
  display.setTextSize(2);
  display.println("I implore thee,");
  display.setTextSize(1);
  display.println("my foonting turlingdromes.");
  display.println("And hooptiously drangle me");
  display.println("with crinkly bindlewurdles,");
  display.println("Or I will rend thee");
  display.println("in the gobberwarts");
  display.println("with my blurglecruncheon,");
  display.println("see if I don't!");

  delay(5000);
  return micros() - start;
}

void DisplayInit()
{
    
  //display.initR(INITR_BLACKTAB);   // initialize a ILI9341S chip, black tab
  //display.setRotation(3);

  //writeRegister (0x51, 0);
  //testText();
  display.setTextWrap(false);
  display.begin();
  display.invertDisplay(0);
  display.setRotation(3);
  display.fillScreen(ILI9341_BLACK);

}




    int GetBlockStartX(byte i)
  {
  

      switch (i)
      {
        case 0:
          return(5);
          break;
        case 1:
          return(170);
          break;
        case 2:
          return(5);
          break;
        case 3:
          return(170);
          break;
      }
  }

  
  int GetBlockStartY(byte i)
  {
  

      switch (i)
      {
        case 0:
          return(38);
          break;
        case 1:
          return(38);
          break;
        case 2:
          return(144);
          break;
        case 3:
          return(144);
          break;
          
      }
  }



  void WheelShow(int i,bool Warning)
  {
     int x, y;
     unsigned int col;
     
     switch (i)
     {
        case 0:
                x = 89;
                y = 74;
                break;
        case 1:
                x = 152;
                y = 74;
                break;
        case 2:
                x = 89;
                y = 141;
                break;
        case 3:
                x = 152;
                y = 141;
                break;
        default:
                x = 340;
                y = 240;
                break;               
     }
     

     if (Warning == true)
     {
        col = ILI9341_RED;   
     }
     else
     {
        col = ILI9341_WHITE;   
     }

     //display.fillRect(x,y,7,19,col);
     

     
     
  }

    void ClearDisplayBlock(byte i)
  {
     int x,y;

     x = GetBlockStartX(i);
     y = GetBlockStartY(i);

     //display.setFont(Adafruit5x7);
//     display.setTextSize(2);    //   display.set2X();
//     display.clearField(x,y,4);
//     display.setTextSize(1);    //   display.set1X();
//     display.clearField(x,y+2,8);

     //display.fillRect(x,y,56,8,ILI9341_BLACK);
     display.fillRect(x+0,y+8,144,85,ILI9341_BLACK);
     WheelShow(i,false);
     
  }

  void UpdateBlock(int i)
  {
        int x,y;
        char s[6], sID[9];
        byte sig;
        char t;

        if ((TPMS[i].LowPressure == true) || (TPMS[i].HighPressure == true))
        {
          if (DisplayFlash)
          {
             strcpy(s,"    ");
             WheelShow(i,true);
          }
          else
          {
             #ifdef DISPLAY_PRESSURE_AS_BAR
                dtostrf(PSI_To_BAR(TPMS[i].TPMS_Pressure), 4, 2, s);
             #elif DISPLAY_PRESSURE_AS_KPA
                dtostrf(PSI_To_KPA(TPMS[i].TPMS_Pressure), 4, 0, s);  //rounded to integer value
             #else
                dtostrf(TPMS[i].TPMS_Pressure, 3, 1, s);
             #endif
             WheelShow(i,true);  
          }
        }
        else
        {
           #ifdef DISPLAY_PRESSURE_AS_BAR
              dtostrf(PSI_To_BAR(TPMS[i].TPMS_Pressure), 4, 2, s);
           #elif DISPLAY_PRESSURE_AS_KPA
              dtostrf(PSI_To_KPA(TPMS[i].TPMS_Pressure), 4, 0, s);  //rounded to integer value
           #else
              dtostrf(TPMS[i].TPMS_Pressure, 3, 1, s);
           #endif
           WheelShow(i,false);
        }

        
        x = GetBlockStartX(i);
        y = GetBlockStartY(i);
        
        //ID first
        display.setCursor(x+50, y);
        display.setTextSize(1);
        display.setTextColor(ILI9341_GREENM,ILI9341_BLACK );
        sprintf(sID,"%08X",TPMS[i].TPMS_ID);
        //display.print(TPMS[i].TPMS_ID,HEX);
        display.print(sID);
                        
        //tyre pressure
        display.setCursor(x,y + 18);
        display.setTextSize(6);
        display.setTextColor(ILI9341_ORANGEM,ILI9341_BLACK);
        display.print(s);

        //temperature
        display.setCursor(x + 30, y + 67);
        display.setTextSize(3);
        display.setTextColor(ILI9341_ORANGEM,ILI9341_BLACK);

        #ifdef DISPLAY_TEMP_AS_FAHRENHEIT
           t = 'F';
        #else
           t = 'C';
        #endif

        
        if (TPMS[i].TPMS_Temperature == NO_VALID_TEMPERATURE)
        {
           display.print("  ---  ");
        }
        else
        {
          if ((TPMS[i].LowTemperature == true) || (TPMS[i].HighTemperature == true))
          {
            if (DisplayFlash)
            {
               strcpy(s,"     ");
               display.print(s); 
            }
            else
            {
               #ifdef DISPLAY_TEMP_AS_FAHRENHEIT
                  dtostrf(DegC_To_DegF(TPMS[i].TPMS_Temperature), 2, 1, s);
               #else
                  dtostrf(TPMS[i].TPMS_Temperature, 2, 1, s);
               #endif
               display.print(s); 
               display.print(t);
            }
          }
          else
          {
               #ifdef DISPLAY_TEMP_AS_FAHRENHEIT
                  dtostrf(DegC_To_DegF(TPMS[i].TPMS_Temperature), 2, 1, s);
               #else
                  dtostrf(TPMS[i].TPMS_Temperature, 2, 1, s);
               #endif
               display.print(s); 
               display.print(t);
          }
        }
        
        //display vertical bars showing how long since last update 5 bars = recent 1 bar = nearing timeout (at timeout it will be removed from display altogether)        
        sig = DisplayTimeoutBar(millis() - TPMS[i].lastupdated);
        DrawSignal(sig, x+15,y+4);

  }

    void UpdateDisplay()
  {
    int i;
    int x = 0;
    int y = 0;
    char s[6];


    for (i = 0; i < 4; i++)
    {
      
      if (TPMS[i].TPMS_ID != 0)
      {
          //Only update the areas which need it to keep the timing overheads down
//          if ((TPMS[i].LowPressure == true) || (TPMS[i].HighPressure == true) || (bitRead(TPMSChangeBits,i) == 1))
//          {
             //ClearDisplayBlock(i);
               UpdateBlock(i);
               if ((bitRead(TPMSChangeBits,i) == 1))
               {
                  bitClear(TPMSChangeBits,i);
               }
//          }

      }


    }


  }

  void DisplayWarning(char* msg, int x, int y)
  {
        display.setCursor(x ,y);
        display.setTextSize(1);
        display.print(msg);
    
  }

  void ScreenSetup()
  {
      DisplayInit();
      DrawBackground();
      DrawTitle();  
  }
