#ifdef UK_433MHz


     #define EXPECTEDBITCOUNT 66
     #define EXPECTEDBYTECOUNT 9     

    //these 433MHz timings are guesses and not proven
    
    #define CDWIDTH_MIN 7600
    #define CDWIDTH_MAX 9500
    
    #define SHORTTIMING_MIN 40
    #define SHORTTIMING_MAX 79
    #define LONGTIMING_MIN 80
    #define LONGTIMING_MAX 120
    #define SYNCTIMING_MIN 175
    #define SYNCTIMING_MAX 1200
    
    #define ENDTIMING_MIN  0
    #define ENDTIMING_MAX  0

      
    #define CC1101_DEFVAL_IOCFG2     0x0C        // GDO2 Output Pin Configuration - Serial out (synchronous)
    #define CC1101_DEFVAL_IOCFG1     0x2E        // GDO1 Output Pin Configuration - not used
    #define CC1101_DEFVAL_IOCFG0     0x0E        // GDO0 Output Pin Configuration - Carrier Sense output
    #define CC1101_DEFVAL_FIFOTHR    0x0F        // RX FIFO and TX FIFO Thresholds - 64 bytes in FIFO
    #define CC1101_DEFVAL_SYNC1      0xD5        // Synchronization word, high byte  11010101 01001111
    #define CC1101_DEFVAL_SYNC0      0x4F        // Synchronization word, low byte
    #define CC1101_DEFVAL_PKTLEN     0x09        // Packet Length
    #define CC1101_DEFVAL_PKTCTRL1   0x00        // Packet Automation Control
    #define CC1101_DEFVAL_PKTCTRL0   0x12 //0x30        // Packet Automation Control - synchronous data
    #define CC1101_DEFVAL_ADDR       0x00        // Device Address
    #define CC1101_DEFVAL_CHANNR     0x00        // Channel Number
    #define CC1101_DEFVAL_FSCTRL1    0x0F        // Frequency Synthesizer Control (was 0x06)
    #define CC1101_DEFVAL_FSCTRL0    0x00        // Frequency Synthesizer Control
    
    // Carrier frequency = 433.919830 MHz
    #define CC1101_DEFVAL_FREQ2  0x10        // Frequency Control Word, High Byte
    #define CC1101_DEFVAL_FREQ1  0xB0        // Frequency Control Word, Middle Byte
    #define CC1101_DEFVAL_FREQ0  0x71        // Frequency Control Word, Low Byte

    #define CC1101_DEFVAL_DEVIATN    0x40      // Modem Deviation Setting (+/-25.390625kHz)

    #define CC1101_DEFVAL_MDMCFG4    0x59        // Modem Configuration (59 = data rate = 20kHz - actual data rate is 10kHz but due to bi-phase coding need to double the rate, RX bandwidth 325kHz)
    #define CC1101_DEFVAL_MDMCFG3    0x93        // Modem Configuration (now 93 = data rate = 20kHz)
    #define CC1101_DEFVAL_MDMCFG2    0x10        // Modem Configuration (GFSK, No Sync or Manchester coding)
    #define CC1101_DEFVAL_MDMCFG1    0x21        // Modem Configuration Channel spacing 100kHz
    #define CC1101_DEFVAL_MDMCFG0    0xF8        // Modem Configuration      
    #define CC1101_DEFVAL_AGCCTRL2   0x87       // AGC Control
    #define CC1101_DEFVAL_AGCCTRL1   0x58        // AGC Control
    #define CC1101_DEFVAL_AGCCTRL0   0x80        // AGC Control
    
    #define CC1101_DEFVAL_MCSM2      0x07        // Main Radio Control State Machine Configuration
    #define CC1101_DEFVAL_MCSM1      0x3C        // Main Radio Control State Machine Configuration
    #define CC1101_DEFVAL_MCSM0      0x18        // Main Radio Control State Machine Configuration
    #define CC1101_DEFVAL_FOCCFG     0x16        // Frequency Offset Compensation Configuration
    #define CC1101_DEFVAL_BSCFG      0x6C        // Bit Synchronization Configuration

    
    #define CC1101_DEFVAL_WOREVT1    0x87        // High Byte Event0 Timeout
    #define CC1101_DEFVAL_WOREVT0    0x6B        // Low Byte Event0 Timeout
    #define CC1101_DEFVAL_WORCTRL    0xFB        // Wake On Radio Control
    #define CC1101_DEFVAL_FREND1     0x56        // Front End RX Configuration
    #define CC1101_DEFVAL_FREND0     0x10        // Front End TX Configuration
    
    #define CC1101_DEFVAL_FSCAL3     0xE9        // Frequency Synthesizer Calibration
    #define CC1101_DEFVAL_FSCAL2     0x2A        // Frequency Synthesizer Calibration
    #define CC1101_DEFVAL_FSCAL1     0x00        // Frequency Synthesizer Calibration
    #define CC1101_DEFVAL_FSCAL0     0x1F        // Frequency Synthesizer Calibration
    
    #define CC1101_DEFVAL_RCCTRL1    0x41        // RC Oscillator Configuration
    #define CC1101_DEFVAL_RCCTRL0    0x00        // RC Oscillator Configuration
    
    #define CC1101_DEFVAL_FSTEST     0x59        // Frequency Synthesizer Calibration Control
    
    #define CC1101_DEFVAL_PTEST      0x7F        // Production Test
    #define CC1101_DEFVAL_AGCTEST    0x3F        // AGC Test
    
    #define CC1101_DEFVAL_TEST2      0x81        // Various Test Settings
    #define CC1101_DEFVAL_TEST1      0x35        // Various Test Settings
    #define CC1101_DEFVAL_TEST0      0x09        // Various Test Settings



      
 
#elif US_315MHz


    //these 315MHz timings are guesses and not proven
    

      #define EXPECTEDBITCOUNT 66
      #define EXPECTEDBYTECOUNT 9
          
      #define CDWIDTH_MIN 13500
      #define CDWIDTH_MAX 17500
    
      #define SHORTTIMING_MIN 80
      #define SHORTTIMING_MAX 139
      #define LONGTIMING_MIN 166
      #define LONGTIMING_MAX 250
      #define SYNCTIMING_MIN 350
      #define SYNCTIMING_MAX 1500

      #define ENDTIMING_MIN  140
      #define ENDTIMING_MAX  165


    #define CC1101_DEFVAL_IOCFG2     0x0C        // GDO2 Output Pin Configuration - Serial out (synchronous)
    #define CC1101_DEFVAL_IOCFG1     0x2E        // GDO1 Output Pin Configuration - not used
    #define CC1101_DEFVAL_IOCFG0     0x0E        // GDO0 Output Pin Configuration - Carrier Sense output
    #define CC1101_DEFVAL_FIFOTHR    0x0F        // RX FIFO and TX FIFO Thresholds - 64 bytes in FIFO
    #define CC1101_DEFVAL_SYNC1      0xD5        // Synchronization word, high byte  11010101 01001111
    #define CC1101_DEFVAL_SYNC0      0x4F        // Synchronization word, low byte
    #define CC1101_DEFVAL_PKTLEN     0x09        // Packet Length
    #define CC1101_DEFVAL_PKTCTRL1   0x00        // Packet Automation Control
    #define CC1101_DEFVAL_PKTCTRL0   0x12 //0x30        // Packet Automation Control - synchronous data
    #define CC1101_DEFVAL_ADDR       0x00        // Device Address
    #define CC1101_DEFVAL_CHANNR     0x00        // Channel Number
    #define CC1101_DEFVAL_FSCTRL1    0x0F        // Frequency Synthesizer Control (was 0x06)
    #define CC1101_DEFVAL_FSCTRL0    0x00        // Frequency Synthesizer Control
    
    // Carrier frequency = 314.979828 MHz
    #define CC1101_DEFVAL_FREQ2  0x0C        // Frequency Control Word, High Byte
    #define CC1101_DEFVAL_FREQ1  0x1D        // Frequency Control Word, Middle Byte
    #define CC1101_DEFVAL_FREQ0  0x57        // Frequency Control Word, Low Byte
      
    #define CC1101_DEFVAL_DEVIATN    0x40      // Modem Deviation Setting (+/-25.390625kHz)

    #define CC1101_DEFVAL_MDMCFG4    0x59        // Modem Configuration (59 = data rate = 20kHz - actual data rate is 10kHz but due to bi-phase coding need to double the rate, RX bandwidth 101.562500kHz)
    #define CC1101_DEFVAL_MDMCFG3    0x93        // Modem Configuration (now 93 = data rate = 20kHz)
    #define CC1101_DEFVAL_MDMCFG2    0x10        // Modem Configuration (GFSK, No Sync or Manchester coding)
    #define CC1101_DEFVAL_MDMCFG1    0x21        // Modem Configuration Channel spacing 100kHz
    #define CC1101_DEFVAL_MDMCFG0    0xF8        // Modem Configuration      
    #define CC1101_DEFVAL_AGCCTRL2   0x87       // AGC Control
    #define CC1101_DEFVAL_AGCCTRL1   0x58        // AGC Control
    #define CC1101_DEFVAL_AGCCTRL0   0x80        // AGC Control
    
    #define CC1101_DEFVAL_MCSM2      0x07        // Main Radio Control State Machine Configuration
    #define CC1101_DEFVAL_MCSM1      0x3C        // Main Radio Control State Machine Configuration
    #define CC1101_DEFVAL_MCSM0      0x18        // Main Radio Control State Machine Configuration
    #define CC1101_DEFVAL_FOCCFG     0x16        // Frequency Offset Compensation Configuration
    #define CC1101_DEFVAL_BSCFG      0x6C        // Bit Synchronization Configuration

    
    #define CC1101_DEFVAL_WOREVT1    0x87        // High Byte Event0 Timeout
    #define CC1101_DEFVAL_WOREVT0    0x6B        // Low Byte Event0 Timeout
    #define CC1101_DEFVAL_WORCTRL    0xFB        // Wake On Radio Control
    #define CC1101_DEFVAL_FREND1     0x56        // Front End RX Configuration
    #define CC1101_DEFVAL_FREND0     0x10        // Front End TX Configuration
    
    #define CC1101_DEFVAL_FSCAL3     0xE9        // Frequency Synthesizer Calibration
    #define CC1101_DEFVAL_FSCAL2     0x2A        // Frequency Synthesizer Calibration
    #define CC1101_DEFVAL_FSCAL1     0x00        // Frequency Synthesizer Calibration
    #define CC1101_DEFVAL_FSCAL0     0x1F        // Frequency Synthesizer Calibration
    
    #define CC1101_DEFVAL_RCCTRL1    0x41        // RC Oscillator Configuration
    #define CC1101_DEFVAL_RCCTRL0    0x00        // RC Oscillator Configuration
    
    #define CC1101_DEFVAL_FSTEST     0x59        // Frequency Synthesizer Calibration Control
    
    #define CC1101_DEFVAL_PTEST      0x7F        // Production Test
    #define CC1101_DEFVAL_AGCTEST    0x3F        // AGC Test
    
    #define CC1101_DEFVAL_TEST2      0x81        // Various Test Settings
    #define CC1101_DEFVAL_TEST1      0x35        // Various Test Settings
    #define CC1101_DEFVAL_TEST0      0x09        // Various Test Settings


#endif



void DecodeTPMS()
{
  int i;
  unsigned long id = 0;
  unsigned int status, pressure1, pressure2, temp, battery_low, counter, failed;
   double realpressure;
  float realtemp;
  bool IDFound = false;

  


  //note: incoming bit stream has been shifted right by 6 bits in DecodeBitArray to give integral number of bytes (for checksum)

  id = (unsigned long)(RXBytes[0]) << 26| (unsigned long)(RXBytes[1]) << 18 | (unsigned long)(RXBytes[2]) << 10 | (unsigned long)(RXBytes[3]) << 2 | (unsigned long)(RXBytes[4]) >> 6; 
  id = id & 0xFFFFFFF;

  GetPreferredIndexStr(id, Ref);
 
  status = RXBytes[4] & 0x3f; // status bits and 0 filler

  battery_low = (RXBytes[4] & 0x20) >> 5;
  counter = (RXBytes[4] & 0x18) >> 3;
  failed = RXBytes[4] & 0x01;

  pressure1 = RXBytes[5];
  pressure2 = RXBytes[6] ^ 0xff;
  temp = RXBytes[7];
  




  if (pressure1 != pressure2)
  {
    Serial.println(F("Pressure check mis-match"));
    return;
  }

  realpressure = (double) pressure1;
  realpressure = ((realpressure - 40) * 0.363) - 0.735;  //default to psi, kpa would be (pressure1 - 40.0) * 2.48
  if (realpressure < 0.0)
    realpressure = 0.0;
  realtemp = (float)temp;
  realtemp = realtemp - 40.0;
//  #ifdef DISPLAY_TEMP_AS_FAHRENHEIT
//     realtemp = ((realtemp * 9.0)/5.0) + 32.0;
//  #endif

#ifdef SHOWVALIDTPMS
  Serial.print(F("Pos: "));
  Serial.print(Ref);
  Serial.print(F("   ID: "));
  Serial.print(id, HEX);
  Serial.print(F("   Status: 0x"));
  Serial.print(status,HEX);
  Serial.print(F(" [Battery status: "));
  Serial.print(battery_low);
  Serial.print(F("   Counter: "));
  Serial.print(counter);
  Serial.print(F("   Failed status: "));
  Serial.print(failed);  
  Serial.print(F("]   Temperature: "));
  Serial.print(realtemp);
  Serial.print(F("   Tyre Pressure: "));
  Serial.print(realpressure);
  Serial.print(F(" (psi)  "));
  Serial.print(realpressure/PSI2BAR);
  Serial.print(F(" (bar)"));
  Serial.println(F(""));
#endif

  //DisplayStatusInfo();


  MatchIDandUpdate(id,status, realtemp, realpressure);



  #ifdef SHOWVALIDTPMS
     Serial.println(F(""));
  #endif


  //UpdateDisplay();
}




bool ValidateTimings_Rev()
{


  unsigned int BitWidth;
  unsigned int BitWidthNext;
  unsigned int BitWidthNextPlus1;
  unsigned int BitWidthPrevious;
  unsigned int diff = TimingsIndex - CheckIndex;
  //unsigned long tmp;
  bool WaitingTrailingZeroEdge = false;
  int ret;
  byte SyncCount;
  int ByteCount = 0;
  byte crcResult = 0;
  bool TailFound = false;
  int Index;

  StartDataIndex = 0;

  if (diff < EXPECTEDBITCOUNT)
  { //not enough in the buffer to consider a valid message
    #ifdef SHOWDEBUGINFO
      Serial.print(F("Insufficient data in buffer ("));
      Serial.print(diff);
      Serial.println(F(")"));
    #endif
    return(false);
  }


  //start from back of buffer and find first 'stop' marker (~150) which also has a valid data bit just before it
  for (Index = TimingsIndex-1; Index >= 0; Index--)
  {
     BitWidth = Timings[Index];
     BitWidthPrevious = Timings[Index-1];
     if (IsEndMarker(BitWidth) && (IsValidLong(BitWidthPrevious) || IsValidShort(BitWidthPrevious)))
     {
        TailFound = true;
        Index--;
        break;
     }
    
  }



  if (TailFound == false)
  {
    #ifdef SHOWDEBUGINFO
      Serial.println(F("No valid bits found in data input"));
    #endif
    return(false);    
  }


  BitIndex = EXPECTEDBITCOUNT -1;
  BitCount = 0;
   
  while ((Index > 0) && (BitCount < EXPECTEDBITCOUNT))
  { //something in buffer to process...
    StartDataIndex = Index;
    ret = ValidateBit(Index);
    switch (ret)
    {
        case BITISUNDEFINED:
          //invalid bit
          return(false);
          break;

        case BITISSHORT:
          if (WaitingTrailingZeroEdge)
          {
            //BitTimings[BitIndex] = BitWidth;
            IncomingBits[BitIndex--] = 0;
            BitCount++;
            WaitingTrailingZeroEdge = false;
          }
          else
          {
            WaitingTrailingZeroEdge = true;
          }
          break;

        case BITISLONG:
          if (WaitingTrailingZeroEdge)
          { //shouldn't get a long pulse when waiting for the second short pulse (i.e. expecting bit = 0)
            //try to resync from here?
            return(false);
          }
          else
          {
            //BitTimings[BitIndex] = BitWidth;
            IncomingBits[BitIndex--] = 1;
            BitCount++;
          }
          break;

        case 2:
          return(false);
          break;
      }
    
    Index--;
  }



  if (BitCount == EXPECTEDBITCOUNT)
  {

      ByteCount = DecodeBitArray(6);

      crcResult = Compute_CRC8(8,0x13, 0x00);

      if (crcResult != RXBytes[8])
      {
         #ifdef SHOWDEBUGINFO
          Serial.print(F("CRC calc: "));
          Serial.println(crcResult, HEX);
          Serial.print(F("  CRC rcvd: "));
          Serial.println(RXBytes[8], HEX);
          Serial.println(F("CRC Check failed"));
        #endif
        //PrintData(BitCount);
        #ifdef IGNORECHECKSUMERRORS
          DecodeTPMS();
          TPMS_Changed = true;  //indicates the display needs to be updated.
        #endif
        return(false);
      }
      else
      {
        //decode the message...
        Serial.println(F("CRC Check OK"));
        DecodeTPMS();
        TPMS_Changed = true;  //indicates the display needs to be updated.
        return(true);
      }


  }
  else
  {
    return(false);
  }

  
}
  

bool ValidateTimings_Old()
{


  unsigned int BitWidth;
  unsigned int BitWidthNext;
  unsigned int BitWidthNextPlus1;
  unsigned int BitWidthPrevious;
  unsigned int diff = TimingsIndex - CheckIndex;
  //unsigned long tmp;
  bool WaitingTrailingZeroEdge = false;
  int ret;
  byte SyncCount;
  int ByteCount = 0;
  byte crcResult = 0;

  StartDataIndex = 0;

  if (diff < EXPECTEDBITCOUNT)
  { //not enough in the buffer to consider a valid message
    #ifdef SHOWDEBUGINFO
      Serial.print(F("Insufficient data in buffer ("));
      Serial.print(diff);
      Serial.println(F(")"));
    #endif
    return(false);
  }

  SyncFound = false;
  SyncCount = 0;

  while ((diff > 0) && (BitCount < EXPECTEDBITCOUNT))
  { //something in buffer to process...
    diff = TimingsIndex - CheckIndex;

    BitWidth = Timings[CheckIndex];

    if (SyncFound == false)
    {
      switch (SyncCount)
      {
        case 0:
          if (IsValidSync(BitWidth))
          {
            //SyncFound = true;
            SyncCount++;
            StartDataIndex = CheckIndex + 1;
          }
        case 1:
          if (IsValidSync(BitWidth))
          {//second sync bit, so stay in this state
            StartDataIndex = CheckIndex + 1;
          }
          else
          {
            if (IsValidLong(BitWidth))
            {//valid final sync bit
              SyncFound = true;
              BitIndex = 0;
              BitCount = 0;
              WaitingTrailingZeroEdge = false;
              StartDataIndex = CheckIndex + 1;
            }
            else
            {//invalid bit so go back to look for initial sync bit again
              SyncCount= 0;
              StartDataIndex = CheckIndex + 1;                         
            }
          }
        
      }

    }
    else
    {
      ret = ValidateBit();
      switch (ret)
      {
        case -1:
          //invalid bit
//          BitIndex = 0;
//          BitCount = 0;
//          WaitingTrailingZeroEdge = false;
//          StartDataIndex = CheckIndex + 1;
          SyncFound = false;
          SyncCount = 0;
          break;

        case 0:
          if (WaitingTrailingZeroEdge)
          {
            //BitTimings[BitIndex] = BitWidth;
            IncomingBits[BitIndex++] = 0;
            BitCount++;
            WaitingTrailingZeroEdge = false;
          }
          else
          {
            WaitingTrailingZeroEdge = true;
          }
          break;

        case 1:
          if (WaitingTrailingZeroEdge)
          { //shouldn't get a long pulse when waiting for the second short pulse (i.e. expecting bit = 0)
            //try to resync from here?
            BitIndex = 0;
            BitCount = 0;
            WaitingTrailingZeroEdge = false;
            CheckIndex--;  //recheck this entry
            //StartDataIndex = CheckIndex + 1;
            StartDataIndex = CheckIndex;
            SyncFound = false;
            SyncCount = 0;
          }
          else
          {
            //BitTimings[BitIndex] = BitWidth;
            IncomingBits[BitIndex++] = 1;
            BitCount++;
          }
          break;

        case 2:
          SyncFound = false;
          SyncCount = 0;
          CheckIndex--;  //recheck this entry
          BitIndex = 0;
          BitCount = 0;
          WaitingTrailingZeroEdge = false;
          StartDataIndex = CheckIndex;
          break;
      }
    }
    CheckIndex++;
  }


  if (BitCount >= EXPECTEDBITCOUNT)
  {

      ByteCount = DecodeBitArray(6);

      crcResult = Compute_CRC8(8,0x13, 0x00);

      if (crcResult != RXBytes[8])
      {
         #ifdef SHOWDEBUGINFO
          Serial.print(F("CRC calc: "));
          Serial.println(crcResult, HEX);
          Serial.print(F("  CRC rcvd: "));
          Serial.println(RXBytes[8], HEX);
          Serial.println(F("CRC Check failed"));
        #endif
        //PrintData(BitCount);
        #ifdef IGNORECHECKSUMERRORS
          DecodeTPMS();
          TPMS_Changed = false;  //don't update display for csum error
        #endif
      }
      else
      {
        //decode the message...
        Serial.println(F("CRC Check OK"));
        DecodeTPMS();
        TPMS_Changed = true;  //indicates the display needs to be updated.
      }

    
      return(true);
  }
  else
  {
    return(false);
  }
  
}


bool ValidateTimings()
{


  unsigned int BitWidth;
  unsigned int BitWidthNext;
  unsigned int BitWidthNextPlus1;
  unsigned int BitWidthPrevious;
  unsigned int diff = TimingsIndex;
  //unsigned long tmp;
  bool WaitingTrailingZeroEdge = false;
  int ret;
  int ByteCount = 0;
  byte crcResult = 0;
  bool ConversionFinished = false;

  StartDataIndex = 0;

  if (diff < EXPECTEDBITCOUNT)
  { //not enough in the buffer to consider a valid message
    #ifdef SHOWDEBUGINFO
      Serial.print(F("Insufficient data in buffer ("));
      Serial.print(diff);
      Serial.println(F(")"));
    #endif
    return(false);
  }


  CheckIndex = 0;

  //while ((diff > 0) && (BitCount < EXPECTEDBITCOUNT))
  while ((diff > 0) && (BitCount < MAXBITS) && (ConversionFinished == false))
  { //something in buffer to process...
    diff = TimingsIndex - CheckIndex;

    BitWidth = Timings[CheckIndex];



      ret = ValidateBit();
      switch (ret)
      {
        case BITISUNDEFINED:
          //invalid bit
          if (BitCount >= EXPECTEDBITCOUNT)
          {
            ConversionFinished = true;
            break;
          }
             
          BitIndex = 0;
          BitCount = 0;
          WaitingTrailingZeroEdge = false;
          StartDataIndex = CheckIndex + 1;

          break;

        case BITISSHORT:
          if (WaitingTrailingZeroEdge)
          {
            //BitTimings[BitIndex] = BitWidth;
            IncomingBits[BitIndex++] = 0;
            BitCount++;
            WaitingTrailingZeroEdge = false;
          }
          else
          {
            WaitingTrailingZeroEdge = true;
          }
          break;

        case BITISLONG:
          if (WaitingTrailingZeroEdge)
          { //shouldn't get a long pulse when waiting for the second short pulse (i.e. expecting bit = 0)
            //try to resync from here?
            if (BitCount >= EXPECTEDBITCOUNT)
            {
              ConversionFinished = true;
              break;
            }
            BitIndex = 0;
            BitCount = 0;
            WaitingTrailingZeroEdge = false;
            CheckIndex--;  //recheck this entry
            //StartDataIndex = CheckIndex + 1;
            StartDataIndex = CheckIndex;
          }
          else
          {
            //BitTimings[BitIndex] = BitWidth;
            IncomingBits[BitIndex++] = 1;
            BitCount++;
          }
          break;

        case BITISSYNC:
          if (BitCount >= EXPECTEDBITCOUNT)
          {
            ConversionFinished = true;
            break;
          }
          BitIndex = 0;
          BitCount = 0;
          WaitingTrailingZeroEdge = false;
          StartDataIndex = CheckIndex + 1;
          break;
      }
      CheckIndex++;
    }




  if (BitCount >= EXPECTEDBITCOUNT)
  {
     int Attempts = BitCount - EXPECTEDBITCOUNT+1;
     int StartPoint = 0;  //shift start point

     //keep shifting the data until a valid checksum or run out of data...
     do
     {
        ByteCount = DecodeBitArray(StartPoint,6);
        crcResult = Compute_CRC8(8,0x13, 0x00);
      
        Attempts--;
        StartPoint++;  

     } while((crcResult != RXBytes[8]) && (Attempts > 0));

      //ByteCount = DecodeBitArray(ShiftPlaces);

      //crcResult = Compute_CRC8(8,0x13, 0x00);

      if (crcResult != RXBytes[8])
      {
         #ifdef SHOWDEBUGINFO
          Serial.print(F("CRC calc: "));
          Serial.println(crcResult, HEX);
          Serial.print(F("  CRC rcvd: "));
          Serial.println(RXBytes[8], HEX);
          Serial.println(F("CRC Check failed"));
        #endif
        //PrintData(BitCount);
        #ifdef IGNORECHECKSUMERRORS
          DecodeTPMS();
          TPMS_Changed = false;  //don't update display for csum error
        #endif
      }
      else
      {
        //decode the message...
        Serial.println(F("CRC Check OK"));
        
        DecodeTPMS();
        TPMS_Changed = true;  //indicates the display needs to be updated.
      }

    
      return(true);
  }
  else
  {
    return(false);
  }
  
}
