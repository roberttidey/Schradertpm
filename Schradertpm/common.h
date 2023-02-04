#ifdef ESP8266
	extern TickTwo SignalRefreshTimer;
	extern TickTwo displayflashtimer;
#else
	extern Ticker SignalRefreshTimer;
	extern Ticker displayflashtimer;
#endif

void PrintIDs() {
	byte i;
	Serial.print(F("Preset IDs:  "));
	for(i=0;i<TYRECOUNT;i++) {
		Serial.print(F(" 0x"));
		Serial.print(IDLookup[i],HEX);
		if (i == 4) {
			Serial.println(F(""));
			Serial.print(F("             "));
		} else {
			 if (i < TYRECOUNT-1)
					Serial.print(F(","));      
		}
	}
	Serial.println(F(""));
}

double PSI_To_BAR(double Pressure_PSI) {
	return(Pressure_PSI/PSI2BAR);
}

double PSI_To_KPA(double Pressure_PSI) {
	return(Pressure_PSI * KPA2PSI);
}

double BAR_To_PSI(double Pressure_BAR) {
	return(Pressure_BAR * PSI2BAR);
}

double KPA_To_PSI(double Pressure_KPA) {
	return(Pressure_KPA/KPA2PSI);
}

float DegC_To_DegK(float DegC) {
	return(DegC + 273.15);
}

float DegF_To_DegK(float DegF) {
	return(DegC_To_DegK(DegF_To_DegC(DegF)));
}

float DegC_To_DegF(float DegC) {
	return((DegC * 1.8) + 32.0);
}

float DegF_To_DegC(float DegF) {
	return((DegF-32.0)/1.8);
}


double ConvertPressureForDisplay(double Pressure_PSI) {
	#ifdef DISPLAY_PRESSURE_AS_BAR
		return(PSI_To_BAR(Pressure_PSI));
	#elif DISPLAY_PRESSURE_AS_KPA
		return(PSI_To_KPA(Pressure_PSI));
	#else
		return(Pressure_PSI);
	#endif
}


void ResetSignalRefreshTimer() {
	SignalRefreshTimer.start();
}

#ifdef ESP8266
	void ICACHE_RAM_ATTR EdgeInterrupt() {
#else
	void EdgeInterrupt() {
#endif			
	unsigned long ts = micros();
	unsigned long BitWidth;

	if (TimingsIndex == MAXTIMINGS)   {
		return;
	}

	BitWidth = ts - LastEdgeTime_us;
 
	if (WaitingFirstEdge) {
		if (IsTooShort(BitWidth)) {
			LastEdgeTime_us = ts;  //ignore short pulses at the start of the transmission
			return;
		}
		
		if (digitalRead(RXPin) == LOW)     {
			FirstEdgeIsHighToLow = true;
		} else {
			FirstEdgeIsHighToLow = false;
		}
		WaitingFirstEdge = false;
	}
	 
	if (BitWidth > 0xFFFF) {
		BitWidth = 0xFFFF;
	}

	LastEdgeTime_us = ts;
	Timings[TimingsIndex++] = (unsigned int)BitWidth;

}


bool IsTooShort(unsigned int Width) {
	if (Width < SHORTTIMING_MIN) {
		return (true);
	} else {
		return (false);
	}
}

bool IsTooLong(unsigned int Width) {
	if (Width > LONGTIMING_MAX)   {
		return (true);
	} else {
		return (false);
	}
}




bool IsValidSync(unsigned int Width) {
	if ((Width >= SYNCTIMING_MIN) && (Width <= SYNCTIMING_MAX)) {
		return (true);
	} else {
		return (false);
	}
}

bool IsValidShort(unsigned int Width) {
	if ((Width >= SHORTTIMING_MIN) && (Width <= SHORTTIMING_MAX)) {
		return (true);
	} else {
		return (false);
	}
}


bool IsValidLong(unsigned int Width) {
	if ((Width >= LONGTIMING_MIN) && (Width <= LONGTIMING_MAX)) {
		return (true);
	} else {
		return (false);
	}
}

bool IsEndMarker(unsigned int Width) {
	if ((Width >= ENDTIMING_MIN) && (Width <= ENDTIMING_MAX)) {
		return(true);
	} else {
		return(false);
	}
}


int ValidateBit() {
	unsigned int BitWidth = Timings[CheckIndex];

	if (IsValidLong(BitWidth)) {
		return (BITISLONG);
	}
	if (IsValidShort(BitWidth)) {
		return (BITISSHORT);
	}
	if (IsValidSync(BitWidth)) {
		return (BITISSYNC);
	}
	return (-1);

}

int ValidateBit(int Index) {
	unsigned int BitWidth = Timings[Index];

	if (IsValidLong(BitWidth)) {
		return (BITISLONG);
	}
	if (IsValidShort(BitWidth)) {
		return (BITISSHORT);
	}
	if (IsValidSync(BitWidth)) {
		return (BITISSYNC);
	}
	return (BITISUNDEFINED);
}

byte Compute_CRC8( int bcount, byte Poly, byte crc_init ) {
	byte crc = crc_init;
	int c;
	for (c = 0; c < bcount; c++) {
		byte b = RXBytes[c];
		/* XOR-in next input byte */
		byte data = (byte)(b ^ crc);
		/* get current CRC value = remainder */
		if (Poly == 0x07) {
			crc = (byte)(pgm_read_byte(&CRC8_Poly_07_crctable2[data]));
		} else {
			if (Poly == 0x13) {
				crc = (byte)(pgm_read_byte(&CRC8_Poly_13_crctable2[data]));
			}
		}
	}
	return crc;
}

byte Compute_CRC_XOR(int Start, int bcount, byte crc_init) {
	byte crc = crc_init;
	int c;

	for (c = Start; c < bcount; c++) {
		crc = crc ^ RXBytes[c];
	}
	return(crc);
}

byte Compute_CRC_SUM(int Start, int bcount, byte crc_init) {
	byte crc = crc_init;
	int c;

	for (c = Start; c < bcount; c++) {
		crc = crc + RXBytes[c];
	}
	return(crc);
}

int GetRSSI_dbm() {
	byte RSSI_Read;
	byte RSSI_Offset = 74;
	int ret;
	
	RSSI_Read = readStatusReg(CC1101_RSSI);
	if (RSSI_Read >= 128) {
		ret = (int)((int)(RSSI_Read - 256) /  2) - RSSI_Offset;
	} else {
		ret = (RSSI_Read / 2) - RSSI_Offset;
	}
	return(ret);
}

void ClearRXBuffer() {
	int i;

	for (i = 0; i < sizeof(RXBytes); i++) {
		RXBytes[i] = 0;
	}
}



int ManchesterDecode(int StartIndex) {
	 int i;
	 bool bit1, bit2;
	 byte b = 0;
	 byte n = 0;

	 RXByteCount = 0;
	 for (i = StartIndex; i< BitCount-1;i+=2) {
			bit1 = IncomingBits[i];
			bit2 = IncomingBits[i+1];

			if (bit1 == bit2) {
				 return RXByteCount;
			}

			b = b << 1;
			b = b + (bit2 == true? 1:0);
			n++;
			if (n == 8) {
				RXBytes[RXByteCount] = b;
				RXByteCount++;
				if (RXByteCount >= sizeof(RXBytes)) {
					return(RXByteCount);
				}
				n = 0;
				b = 0;
			}     
	 }
	 return RXByteCount;
}

int DifferentialManchesterDecode(int StartIndex) {
	int i;
	bool bit1, bit2, bit3;
	byte b = 0;
	byte n = 0;

	RXByteCount = 0;
	for (i = StartIndex; i< BitCount-1;i+=2) {
		bit1 = IncomingBits[i];
		bit2 = IncomingBits[i+1];
		bit3 = IncomingBits[i+2];

		if (bit1 != bit2) {
			if (bit2 != bit3) {
				b = b << 1;
				b = b + 0;
				n++;
				if (n == 8) {
					RXBytes[RXByteCount] = b;
					RXByteCount++;
					n = 0;
					b = 0;
				}          
			} else {
				bit2 = bit1;
				i+=1;
				break;
			}
		} else {
			bit2 = 1 - bit1;
			break;
		}
	 }

	 for (; i< BitCount-1;i+=2) {
		bit1 = IncomingBits[i];

		if (bit1 == bit2) 
			 return RXByteCount;
		bit2 = IncomingBits[i+1];

		b = b << 1;
		b = b + (bit1 == bit2? 1:0);
		n++;
		if (n == 8) {
			RXBytes[RXByteCount] = b;
			RXByteCount++;
			n = 0;
			b = 0;
		} 
	 }
	 return RXByteCount;
}

void InvertBitBuffer() {
	 int i;

	 for (i = 0;i < BitCount;i++) {
			IncomingBits[i] = !IncomingBits[i];
	 }
}

static inline uint8_t bit_at(const uint8_t *bytes, unsigned bit) {
	return (uint8_t)(bytes[bit >> 3] >> (7 - (bit & 7)) & 1);
}

int FindManchesterStart(const uint8_t *pattern,int pattern_bits_len ) {
	int i;

	//Renault TMS header pattern
	 //const uint8_t pattern[] = {0xAA, 0xA9};
	// int pattern_bits_len = 16;
	 unsigned int ipos = 0;
	 unsigned int ppos = 0; // cursor on init pattern

	 if (BitCount < pattern_bits_len) 
		return -1;

		while ((ipos < BitCount-3) && (ppos < pattern_bits_len)) {
			if (IncomingBits[ipos] == bit_at(pattern, ppos)) {
				ppos++;
				ipos++;
				if (ppos == pattern_bits_len)
					return ipos;
			} else {
				ipos -= ppos;
				ipos++;
				ppos = 0;
			}
		}
		// Not found
		return -1;
 
}

void InitDataBuffer() {
	BitIndex = 0;
	BitCount = 0;
	ValidBlock = false;
	//WaitingTrailingZeroEdge = false;
	WaitingFirstEdge  = true;
	CheckIndex = 0;
	TimingsIndex = 0;
	SyncFound = false;
}


void ClearTPMSData(int i) {
	if (i > TYRECOUNT)
		return;

	TPMS[i].TPMS_ID = 0;
	TPMS[i].lastupdated = 0;
	#ifdef DISPLAY_PRESSURE_AS_BAR
		TPMS[i].TPMS_LowPressureLimit = BAR_To_PSI(PressureLowLimits[i]);
		TPMS[i].TPMS_HighPressureLimit = BAR_To_PSI(PressureHighLimits[i]);
	#elif DISPLAY_PRESSURE_AS_KPA
		TPMS[i].TPMS_LowPressureLimit = KPA_To_PSI(PressureLowLimits[i]);
		TPMS[i].TPMS_HighPressureLimit = KPA_To_PSI(PressureHighLimits[i]);
	#else
		TPMS[i].TPMS_LowPressureLimit = PressureLowLimits[i];
		TPMS[i].TPMS_HighPressureLimit = PressureHighLimits[i];
	#endif

	#ifdef DISPLAY_TEMP_AS_FAHRENHEIT
		TPMS[i].TPMS_LowTemperatureLimit = DegF_To_DegC(TemperatureLowLimits[i]);
		TPMS[i].TPMS_HighTemperatureLimit = DegF_To_DegC(TemperatureHighLimits[i]);  
	#else
		TPMS[i].TPMS_LowTemperatureLimit = TemperatureLowLimits[i];
		TPMS[i].TPMS_HighTemperatureLimit = TemperatureHighLimits[i];
	#endif
	
	TPMS[i].LowPressure = false;
	TPMS[i].HighPressure = false;
	TPMS[i].LowTemperature = false;
	TPMS[i].HighTemperature = false;
	TPMS[i].AudibleAlarmActive = false;
	#ifdef USE_LCDDISPLAY
		if (i < 4)
			ClearDisplayBlock(i);
	#endif
}

void PulseDebugPin(int width_us) {
	digitalWrite(DEBUGPIN, HIGH);
	delayMicroseconds(width_us);
	digitalWrite(DEBUGPIN, LOW);
}

void UpdateFreqOffset() {
	FreqOffsetAcc = FreqOffsetAcc + readStatusReg(CC1101_FREQEST);
	writeReg(CC1101_FSCTRL0, FreqOffsetAcc);
}



int GetPreferredIndex(unsigned long ID) {
	int i;

	for (i = 0; i  < TYRECOUNT; i++) {
		if (IDLookup[i] == ID) {
			return (i);
		}
	}
	return (-1);
}

void GetPreferredIndexStr(unsigned long ID, char* sptr) {
	int tmp;
	tmp = GetPreferredIndex(ID);

	switch(tmp % 5) {
		case 0:
			 strcpy(sptr, "FL");
			 break;
		case 1:
			 strcpy(sptr, "FR");
			 break;
		case 2:
			 strcpy(sptr, "RL");
			 break; 
		case 3:
			 strcpy(sptr, "RR");
			 break;
		case 4:
			 strcpy(sptr, "SP");
			 break;
		default:
			 sptr[0] = '\0';
			 break;
	}
}

void PrintTimings(byte StartPoint, unsigned int Count) {
	unsigned int i;
	char c[10];
	for (i = 0; i < Count; i++) {
		if ((StartPoint == 0) && (i == StartDataIndex))
				 Serial.println();
		sprintf(c, "%3d,",Timings[StartPoint + i]);
		Serial.print(c);
	}
	Serial.println(F(""));
}

void PrintData(int StartPos, unsigned int Count) {
	unsigned int i, c;
	byte hexdata;
	for (i = StartPos, c = 1; c <= Count; i++, c++) {
		Serial.print(IncomingBits[i]);
		hexdata = (hexdata << 1) + IncomingBits[i];
		if (c % 8 == 0) {
			Serial.print(F(" ["));
			Serial.print(hexdata, HEX);
			Serial.print(F("] "));
			hexdata = 0;
		}
	}
	Serial.println(F(""));

}

void PrintData(unsigned int Count) {
	 PrintData(0,Count);
}

void PrintBytes(unsigned int Count) {
	byte i;

	for (i = 0; i < Count; i++) {
		Serial.print(F(" ["));
		Serial.print(RXBytes[i],HEX);
		Serial.print(F("] "));
	}
	Serial.println(F(""));
}
void InitTPMS() {
	int i;

	for (i = 0; i < TYRECOUNT; i++) {
		ClearTPMSData(i);
	}
	#ifdef USE_LCDDISPLAY 
		UpdateDisplay();
		SignalRefreshNeeded = false;
	#endif

}

double GetTempCompensatedPressureLimit(double LimitsPressure, float LimitsTemperature_DegC, float CurrentTemperature_DegC) {
	return((CurrentTemperature_DegC + DEGC_TO_KELVIN) * (LimitsPressure / (LimitsTemperature_DegC + DEGC_TO_KELVIN)));  // T2 * (P1/T1)
}

bool PressureBelowLowPressureLimit(int TyreIndex) {
	bool ret = false;

	double RoundedPressure = round(TPMS[TyreIndex].TPMS_Pressure * 10)/10.0;
	float LowLimit = TPMS[TyreIndex].TPMS_LowPressureLimit;
	float Temperature = TPMS[TyreIndex].TPMS_Temperature;
	
	#ifdef ENABLE_PRESSURE_ALARM_TEMPERATURE_COMPENSATION
		if (Temperature != NO_VALID_TEMPERATURE) {
			LowLimit = GetTempCompensatedPressureLimit(LowLimit, PressureLimitsTemperature,Temperature);   //adjust the limit based on the current temperatue and the temperature defined for the limits setting T2 * (P1/T1)
			LowLimit = round(LowLimit * 10)/10.0;
		}
	#endif

	if (RoundedPressure < LowLimit) {
		ret = true;
	}
	return(ret);
	
}

bool PressureAboveHighPressureLimit(int TyreIndex) {
	bool ret = false;

	double RoundedPressure = round(TPMS[TyreIndex].TPMS_Pressure * 10)/10.0;
	float HighLimit = TPMS[TyreIndex].TPMS_HighPressureLimit;
	float Temperature = TPMS[TyreIndex].TPMS_Temperature;
	
	#ifdef ENABLE_PRESSURE_ALARM_TEMPERATURE_COMPENSATION
		 if (Temperature != NO_VALID_TEMPERATURE) {     
			HighLimit = GetTempCompensatedPressureLimit(HighLimit, PressureLimitsTemperature,Temperature);   //adjust the limit based on the current temperatue and the temperature defined for the limits setting T2 * (P1/T1)
			HighLimit = round(HighLimit * 10)/10.0;
		 }
	#endif

	if (RoundedPressure > HighLimit) {
		ret = true;
	}
	return(ret);
	
}

bool TemperatureBelowLowTemperatureLimit(int TyreIndex) {
	bool ret = false;

	float LowLimit = TPMS[TyreIndex].TPMS_LowTemperatureLimit;
	float Temperature = round(TPMS[TyreIndex].TPMS_Temperature * 10)/10.0;

	if (Temperature == NO_VALID_TEMPERATURE) {
		ret = false;
	} else {
		if (Temperature < LowLimit) {
			ret = true;
		}    
	}
	return(ret);
	
}

bool TemperatureAboveHighTemperatureLimit(int TyreIndex) {
	bool ret = false;

	float HighLimit = TPMS[TyreIndex].TPMS_HighTemperatureLimit;
	float Temperature = round(TPMS[TyreIndex].TPMS_Temperature * 10)/10.0;

	if (Temperature == NO_VALID_TEMPERATURE) {
		ret = false;
	} else {
		if (Temperature > HighLimit) {
			ret = true;
		}    
	}
	return(ret);
}

void UpdateTPMSData(int index, unsigned long ID, unsigned int status, float Temperature, double Pressure) {

	if (index >= TYRECOUNT)
		return;

	TPMS[index].TPMS_ID = ID;
	TPMS[index].TPMS_Status = status;
	TPMS[index].lastupdated = millis();
	TPMS[index].TPMS_Temperature = Temperature;
	TPMS[index].TPMS_Pressure = Pressure;

	#ifdef ENABLE_PRESSURE_ALARMS
		if (PressureBelowLowPressureLimit(index)) {
			 
			TPMS[index].LowPressure = true;
			#ifdef SHOWDEBUGINFO
				Serial.print(F("  Low Pressure warning."));
				Serial.print(F("  Limit: "));
				Serial.print(ConvertPressureForDisplay(TPMS[index].TPMS_LowPressureLimit));
				#ifdef ENABLE_PRESSURE_ALARM_TEMPERATURE_COMPENSATION
					if (Temperature != NO_VALID_TEMPERATURE) {
						Serial.print(F(" ["));
						Serial.print(ConvertPressureForDisplay(GetTempCompensatedPressureLimit(TPMS[index].TPMS_LowPressureLimit,PressureLimitsTemperature,TPMS[index].TPMS_Temperature)));
						 Serial.print(F("]"));
					}
					#endif
					Serial.print(F("  Measured: "));
					Serial.println(ConvertPressureForDisplay(TPMS[index].TPMS_Pressure));
				#endif
			} else {
				TPMS[index].LowPressure = false;
			}
			
			if (PressureAboveHighPressureLimit(index)) {
				
				TPMS[index].HighPressure = true;
			#ifdef SHOWDEBUGINFO
				Serial.print(F("  High Pressure warning."));
				Serial.print(F("  Limit: "));
				Serial.print(ConvertPressureForDisplay(TPMS[index].TPMS_HighPressureLimit));
				#ifdef ENABLE_PRESSURE_ALARM_TEMPERATURE_COMPENSATION
					if (Temperature != NO_VALID_TEMPERATURE) {
						Serial.print(F(" ["));
						Serial.print(ConvertPressureForDisplay(GetTempCompensatedPressureLimit(TPMS[index].TPMS_HighPressureLimit,PressureLimitsTemperature,TPMS[index].TPMS_Temperature)));
						Serial.print(F("]"));
					}
				#endif
				Serial.print(F("  Measured: "));
				Serial.println(ConvertPressureForDisplay(TPMS[index].TPMS_Pressure));
			#endif
			} else {
				TPMS[index].HighPressure = false;
			}
	#endif

	#ifdef ENABLE_TEMPERATURE_ALARMS
		if (TemperatureBelowLowTemperatureLimit(index)) {      
			TPMS[index].LowTemperature = true;
			#ifdef SHOWDEBUGINFO
				 Serial.print(F("Low Temperature warning."));
				 #ifdef DISPLAY_TEMP_AS_FAHRENHEIT
					 Serial.print(F("  Limit(degF): "));
					 Serial.print(DegC_To_DegF(TPMS[index].TPMS_LowTemperatureLimit));
					 Serial.print(F("  Measured(degF): "));
					 Serial.println(DegC_To_DegF(TPMS[index].TPMS_Temperature));
				 #else
					 Serial.print(F("  Limit(degC): "));
					 Serial.print(TPMS[index].TPMS_LowTemperatureLimit);
					 Serial.print(F("  Measured(degC): "));
					 Serial.println(TPMS[index].TPMS_Temperature);
				 #endif
			#endif
		} else {
			TPMS[index].LowTemperature = false;
		}
		
		if (TemperatureAboveHighTemperatureLimit(index)) {
			
		 TPMS[index].HighTemperature = true;
			#ifdef SHOWDEBUGINFO
				 Serial.print(F("High Temperature warning."));
				 #ifdef DISPLAY_TEMP_AS_FAHRENHEIT
					Serial.print(F("  Limit(degF): "));
					Serial.print(DegC_To_DegF(TPMS[index].TPMS_HighTemperatureLimit));
					Serial.print(F("  Measured(degF): "));
					Serial.println(DegC_To_DegF(TPMS[index].TPMS_Temperature));
				 #else
					Serial.print(F("  Limit(degC): "));
					Serial.print(TPMS[index].TPMS_HighTemperatureLimit);
					Serial.print(F("  Measured(degC): "));
					Serial.println(TPMS[index].TPMS_Temperature);
				 #endif
			#endif
		} else {
			TPMS[index].HighTemperature = false;
		}

	#endif
	
	bitSet(TPMSChangeBits,index);
	TPMS[index].RSSIdBm = RSSIvalue;
}

void DisplayStatusInfo() {
	Serial.print (F("FreqOffset: "));
	Serial.print (FreqOffset);
	Serial.print (F("  DemodLinkQuality: "));
	Serial.print (DemodLinkQuality);
	Serial.print (F("  RSSI: "));
	Serial.println (RSSIvalue);
}

bool OutOfLimitsPressureCheck() {
	int i;
	bool ret = false;
	
	#ifdef ENABLE_PRESSURE_ALARMS
		Pressure_Alarm_Active = false;
		
		for (i = 0; i < TYRECOUNT; i++) {
	
			if ((TPMS[i].LowPressure == true) || (TPMS[i].HighPressure == true)) {
				Pressure_Alarm_Active = true;
				#ifdef ENABLE_AUDIBLE_ALARM
					//initiate alarm (initial trigger)
					if (TPMS[i].AudibleAlarmActive == false) {
						StartAlarm();
						TPMS[i].AudibleAlarmActive = true;
					}
				#endif
			 } else {
				if ((TPMS[i].LowPressure == false) && (TPMS[i].HighPressure == false) && (TPMS[i].LowTemperature == false) && (TPMS[i].HighTemperature == false)) {
					TPMS[i].AudibleAlarmActive = false;
				}
			 }       
		}
		
		#ifdef ENABLE_AUDIBLE_ALARM
			if ((Pressure_Alarm_Active == false) && ( Temperature_Alarm_Active == false)) {
			   if (Audible_Alarm_Running)
				  StopAlarm();
			}
		#endif
		
		return(Pressure_Alarm_Active);

	#else
		Pressure_Alarm_Active = false;
		return(false);   
	#endif

}

bool OutOfLimitsTemperatureCheck() {
	int i;
	bool ret = false;
 
	#ifdef ENABLE_TEMPERATURE_ALARMS
		Temperature_Alarm_Active = false;

		for (i = 0; i < TYRECOUNT; i++) {
			if ((TPMS[i].LowTemperature == true) || (TPMS[i].HighTemperature == true)) {
				Temperature_Alarm_Active = true;
				#ifdef ENABLE_AUDIBLE_ALARM
					//initiate alarm (initial trigger)
					if (TPMS[i].AudibleAlarmActive == false) {
						StartAlarm();
						TPMS[i].AudibleAlarmActive = true;
					}
				#endif
			} else {          
				if ((TPMS[i].LowPressure == false) && (TPMS[i].HighPressure == false) && (TPMS[i].LowTemperature == false) && (TPMS[i].HighTemperature == false)) {
					TPMS[i].AudibleAlarmActive = false;                         
				}
			}       
		}
		
		#ifdef ENABLE_AUDIBLE_ALARM
			if ((Pressure_Alarm_Active == false) && ( Temperature_Alarm_Active == false)) {
				if (Audible_Alarm_Running)  
					StopAlarm();
			}
		#endif
		
		return(Temperature_Alarm_Active);

	#else
		 Temperature_Alarm_Active = false;
		 return(false);   
	#endif

}

void DisplayTimerExpired() {
	 DisplayFlashExpired = true;

	 DisplayFlash = !DisplayFlash; 
	 if (DisplayFlash) {
			displayflashtimer.interval(BLANK_MS);
	 } else {
			displayflashtimer.interval(NOBLANK_MS);
	 }
}

void SignalRefreshRequired() {
	 SignalRefreshNeeded = true;
}

boolean Check_TPMS_Timeouts() {
	 byte i;
	 boolean ret = false;
		
	//clear any data not updated in the last 15 minutes
	for (i = 0; i < TYRECOUNT; i++) {
		if ((TPMS[i].TPMS_ID != 0) && (millis() - TPMS[i].lastupdated > TPMS_TIMEOUT)) {
			ClearTPMSData(i);
			OutOfLimitsPressureCheck();
			OutOfLimitsTemperatureCheck();
			ret = true;
		}
	}

	return(ret);
}


void MatchIDandUpdate(unsigned long id ,unsigned int status, float realtemp,float realpressure) {

	bool IDFound = false;
	int prefindex;
	int i;
	
	//update the array of tyres data
	for (i = 0; i < TYRECOUNT; i++) { //find a matching ID if it already exists
		if (id == TPMS[i].TPMS_ID) {
			UpdateTPMSData(i, id, status, realtemp, realpressure);
			IDFound = true;
			break;
		}

	}

	//no matching IDs in the array, so see if there is an empty slot to add it into, otherwise, ignore it.
	if (IDFound == false) {

		prefindex = GetPreferredIndex(id);
		if (prefindex == -1) { //not found a specified index, so use the next available one..
			#ifndef SPECIFIC_IDS_ONLY 
				for (i = 0; i < TYRECOUNT; i++) {
					if (TPMS[i].TPMS_ID == 0) {
						UpdateTPMSData(i, id, status, realtemp, realpressure);
						break;
					}
				}
			#endif
		} else { //found a match in the known ID list...
			UpdateTPMSData(prefindex, id, status, realtemp, realpressure);
		}

	}
	OutOfLimitsPressureCheck();
	OutOfLimitsTemperatureCheck();
}


int DecodeBitArray(int StartIndex, byte ShiftRightBitCount) {
	//convert 1s and 0s array to byte array
	int i;
	int n = 0;
	byte b = 0;

	ClearRXBuffer();

	n = ShiftRightBitCount;  //pad with this number of 0s to the left
	RXByteCount = 0;
	
	for (i = StartIndex; i < BitCount; i++) {
		b = b << 1;
		b = b + IncomingBits[i];
		n++;
		if (n == 8) {
			RXBytes[RXByteCount] = b;
			//Serial.print(RXBytes[RXByteCount],HEX);
			//Serial.print(" - ");
			RXByteCount++;
			n = 0;
			b = 0;
		}

	}
	//Serial.println("");
	return (RXByteCount);
}

int DecodeBitArray( byte ShiftRightBitCount) {
	//convert 1s and 0s array to byte array
	int i;
	int n = 0;
	byte b = 0;

	ClearRXBuffer();

	n = ShiftRightBitCount;  //pad with this number of 0s to the left
	RXByteCount = 0;
	
	for (i = 0; i < BitCount; i++) {
		b = b << 1;
		b = b + IncomingBits[i];
		n++;
		if (n == 8) {
			RXBytes[RXByteCount] = b;
			//Serial.print(RXBytes[RXByteCount],HEX);
			//Serial.print(" - ");
			RXByteCount++;
			n = 0;
			b = 0;
		}

	}
	return (RXByteCount);
}

bool ReceiveMessage() {
	//Check bytes in FIFO
	int FIFOcount;
	int resp;
	int lRSSI = 0;
	bool StatusUpdated = true;
	byte crcResult;
	int ByteCount = 0;
	bool ValidMessage = false;
	unsigned long t1;


	if(useTestTimings) {
		//test set up....

		CD_Width = CDWIDTH_MIN + ((CDWIDTH_MAX - CDWIDTH_MIN)/2);

		//copy timings to timings array as if they've come from the interrupt

		for (TimingsIndex=0;TimingsIndex<TestTimings_len;TimingsIndex++) {
			Timings[TimingsIndex] = TestTimings[TimingsIndex];
		}

		FirstEdgeIsHighToLow = !FirstTimingIsLow;
	
	} else {
	//set up timing of edges using interrupts...
		LastEdgeTime_us = micros();
		CD_Width = LastEdgeTime_us;

		attachInterrupt(digitalPinToInterrupt(RXPin), EdgeInterrupt, CHANGE);
		#ifdef ARDUINO_SEEED_XIAO_M0
			NVIC_SetPriority(EIC_IRQn, 2);
			//!!!!! this is necessary for the Seeeduino Xiao due external interupts having a higher priority than the micros() timer rollover (by default). 
			// This can cause the micros() value to appear to go 'backwards' in time in the external interrupt handler and end up giving an incorrect 65536 bit width result.
		#endif
		RSSIvalue = -1000;
 	
		while (GetCarrierStatus() == true) {
			//get the maximum RSSI value seen during data receive window
			lRSSI = GetRSSI_dbm();
			if (lRSSI > RSSIvalue) {
				RSSIvalue = lRSSI;
			}
			noInterrupts();
			t1 = micros();
			interrupts();
			if (t1 - CD_Width > CDWIDTH_MAX) {
				break;
			}
		}

		delayMicroseconds(1000);  //there is a delay on the serial data stream so ensure we allow a bit of extra time after CD finishes to ensure all the data is captured
		detachInterrupt(digitalPinToInterrupt(RXPin)); 
		EdgeInterrupt();  //force a final edge change just to be sure
		CD_Width = micros() - CD_Width;
	}

	if ((CD_Width >= CDWIDTH_MIN) && (CD_Width <= CDWIDTH_MAX) && (TimingsIndex > EXPECTEDBITCOUNT )) {
		PulseDebugPin(100);
		#ifdef SHOWDEBUGINFO
			Serial.println(F("******************************************************************"));
			Serial.println(F("Checking...."));
		#endif
		digitalWrite(LED_RX,LED_ON);
		CheckIndex = 0;
		ValidMessage = ValidateTimings();
		
		#ifdef SHOWDEBUGINFO
			Serial.println(F("Timings...."));
			Serial.print(F("CD_Width="));
			Serial.println(CD_Width);
			Serial.print(F("TimingsIndex="));
			Serial.println(TimingsIndex);
			Serial.print(F("Checking complete. Bitcount: "));
			Serial.print(BitCount);
			Serial.print(F("  StartDataIndex: "));
			Serial.println(StartDataIndex);
			Serial.print(F(" RSSI(dBm):"));
			Serial.println(RSSIvalue);
			#ifdef ALWAYSSHOWTIMINGS
				PrintTimings(0,TimingsIndex+1);
				PrintData(BitCount);
				PrintBytes(EXPECTEDBYTECOUNT);
			#else
				if (ValidMessage) {
					PrintTimings(0,TimingsIndex+1);
					PrintData(BitCount);
					PrintBytes(EXPECTEDBYTECOUNT);
				}       
			 #endif

		#endif

		digitalWrite(LED_RX,LED_OFF);
		Flush_RX_FIFO(true);
		return (ValidMessage);
	} else {
		#ifdef SHOWDEBUGINFO
			if (TimingsIndex >= 50) {
				Serial.println(F("******************************************************************"));
				Serial.print(F("CD_Width*="));
				Serial.println(CD_Width);   
				Serial.print(F("TimingsIndex="));
				Serial.println(TimingsIndex); 
				PrintTimings(0,TimingsIndex+1);  
				PrintData(BitCount);
			 }
		#endif
		Flush_RX_FIFO(true);
		return (false);
	}
}
