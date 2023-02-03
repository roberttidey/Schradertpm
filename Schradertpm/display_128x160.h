
#include "bitmap.h"
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735

#define TFT_CS 5
#define TFT_RST 3
#define TFT_DC 4



  Adafruit_ST7735 display = Adafruit_ST7735(TFT_CS,  TFT_DC, TFT_RST);

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
  display.setCursor(118,0);
  #ifdef US_315MHz
     display.print("315 MHz");
  #else
     display.print("433 MHz");
  #endif
}


void ShowTemperatureType()
{
    //display.setCursor(70,118);
    display.drawCircle(80, 120, 2, ST77XX_MAGENTA);
    display.setCursor(84,118);
    display.setTextColor(ST77XX_MAGENTA);
    display.setTextSize(1);
   #ifdef DISPLAY_TEMP_AS_FAHRENHEIT
      display.print(F("F"));
   #else
      display.print(F("C"));
   #endif
}

void DrawTitle()
{
  //background header
  display.fillRect(0, 0,159,16, ST77XX_BLUE);
  
  display.setCursor(2,0);
  display.setTextColor(ST77XX_YELLOW);
  display.setTextSize(1);
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
    #elif Citroen
       display.print("Citroen TPMS");
    #elif Ford
       display.print(F("Ford TPMS"));
    #elif PontiacG82009
       display.print(F("Pontiac G8 TPMS"));
    #elif Hyundai_i35
       display.print(F("Hyundai i35"));
    #elif Schrader_C1100
       display.print(F("Hyundai Tucson"));
    #else
       display.print(F("Toyota TPMS"));
    #endif
  PrintFreq();  
  display.setCursor(45,8);
  display.setTextColor(ST77XX_YELLOW);
  display.setTextSize(1);
  display.print(F(" JSMSolutions V"));
  display.print(VERSION);

  ShowTemperatureType();
  
}

void DrawBackground()
{
     int LineSplitCol = 72;
     
     display.drawBitmap(0, 8, car_bmp_128x60_car, 160, 128, ST77XX_WHITE);
     display.drawLine(8,LineSplitCol,54,LineSplitCol,ST77XX_BLUE);
     display.drawLine(108,LineSplitCol,154,LineSplitCol,ST77XX_BLUE);
}

void DrawSignal(byte Level, int x, int y)
{
 
       
  if (Level >=1)
  {
       display.fillRect(x+8, y ,4,4, ST77XX_YELLOW);
  }
  else
  {
     display.fillRect(x+8, y ,4,4, ST7735_BLACK);
     display.drawRect(x+8, y ,4,4, ST77XX_YELLOW);
  }
       
  if (Level >=2)
  {
       display.fillRect(x+11, y -2,4,6, ST77XX_YELLOW);
  }
  else
  {
      display.fillRect(x+11, y -2,4,6, ST7735_BLACK);
      display.drawRect(x+11, y -2,4,6, ST77XX_YELLOW);
  }
       
  if (Level >=3)
  {
     display.fillRect(x+14, y -4,4,8, ST77XX_YELLOW);
  }
  else
  {
     display.fillRect(x+14, y -4,4,8, ST7735_BLACK);
     display.drawRect(x+14, y -4,4,8, ST77XX_YELLOW);
  }
  
       
  if (Level >=4)
  {
      display.fillRect(x+17, y-6,4,10, ST77XX_YELLOW);
  }
  else
  {
      display.fillRect(x+17, y -6,4,10, ST7735_BLACK);
      display.drawRect(x+17, y -6,4,10, ST77XX_YELLOW);
  }
       
  if (Level >=5)
  {
      display.fillRect(x+20, y -8,4,12, ST77XX_YELLOW);
  }
  else
  {
     display.fillRect(x+20, y -8,4,12, ST7735_BLACK);
     display.drawRect(x+20, y -8,4,12, ST77XX_YELLOW);
  }  
      
}

void DisplayInit()
{
    
  display.initR(INITR_GREENTAB);   // initialize a ST7735S chip, black tab
  //display.initR(0x06);  //fix for 2 row + 1 col offset issue (shows as random pixels in those areas)
  display.setRotation(3);
  display.fillScreen(ST7735_BLACK);
  display.setTextWrap(false);
}




    int GetBlockStartX(byte i)
  {
  

      switch (i)
      {
        case 0:
          return(2);
          break;
        case 1:
          return(102);
          break;
        case 2:
          return(2);
          break;
        case 3:
          return(102);
          break;
      }
  }

  
  int GetBlockStartY(byte i)
  {
  

      switch (i)
      {
        case 0:
          return(18);
          break;
        case 1:
          return(18);
          break;
        case 2:
          return(78);
          break;
        case 3:
          return(78);
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
                x = 59;
                y = 33 + 8;
                break;
        case 1:
                x = 101;
                y = 33 + 8;
                break;
        case 2:
                x = 59;
                y = 78 + 8;
                break;
        case 3:
                x = 101;
                y = 78 + 8;
                break;
        default:
                x = 128;
                y = 160;
                break;               
     }
     

     if (Warning == true)
     {
        col = ST77XX_RED;   
     }
     else
     {
        col = ST77XX_WHITE;   
     }

     display.fillRect(x,y,5,13,col);
     

     
     
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

     display.fillRect(x,y,56,8,ST7735_BLACK);
     display.fillRect(x+4,y,52,51,ST7735_BLACK);
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
             WheelShow(i,true);  //show wheel as red
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
             WheelShow(i,true);   //show wheel as red
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
           WheelShow(i,false);    //show wheel as normal (white)
        }

        
        x = GetBlockStartX(i);
        y = GetBlockStartY(i);
        
        //ID first
        display.setCursor(x+6, y);
        display.setTextSize(1);
        display.setTextColor(ST77XX_RED,ST7735_BLACK );
        sprintf(sID,"%08X",TPMS[i].TPMS_ID);
        //display.print(TPMS[i].TPMS_ID,HEX);
        display.print(sID);
        
        //tyre pressure
        display.setCursor(x + 6,y + 15);
        display.setTextSize(2);
        display.setTextColor(ST77XX_GREEN,ST7735_BLACK);
        display.print(s);

        //temperature
        display.setCursor(x + 26,y + 40);
        display.setTextSize(1);
        //display.setTextColor(ST77XX_GREEN,ST7735_BLACK);
        display.setTextColor(ST77XX_MAGENTA,ST7735_BLACK);

        #ifdef DISPLAY_TEMP_AS_FAHRENHEIT
           t = 'F';
        #else
           t = 'C';
        #endif

        
        if (TPMS[i].TPMS_Temperature == NO_VALID_TEMPERATURE)
        {
           display.print(" --- ");
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
                  dtostrf(DegC_To_DegF(TPMS[i].TPMS_Temperature), 5, 1, s);
               #else
                  dtostrf(TPMS[i].TPMS_Temperature, 5, 1, s);
               #endif
               display.print(s); 
               //display.print(t);
            }
          }
          else
          {
               #ifdef DISPLAY_TEMP_AS_FAHRENHEIT
                  dtostrf(DegC_To_DegF(TPMS[i].TPMS_Temperature), 5, 1, s);
               #else
                  dtostrf(TPMS[i].TPMS_Temperature, 5, 1, s);
               #endif
               display.print(s); 
               //display.print(t);
          }
        }
        
        //display vertical bars showing how long since last update 5 bars = recent 1 bar = nearing timeout (at timeout it will be removed from display altogether)        
        sig = DisplayTimeoutBar(millis() - TPMS[i].lastupdated);
        DrawSignal(sig, x,y+44);

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
//          if ((TPMS[i].LowPressure == true) || (TPMS[i].HighPressure == true) || (bitRead(TPMSChangeBits,i) == 1) || SignalRefreshNeeded)
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
