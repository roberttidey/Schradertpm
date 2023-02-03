# Schrader tyre pressure monitor for ESP8266
This is a work in progress. It is not functional yet.

It is effectively a port of the excellent Hackster project by JSMSolns

https://www.hackster.io/jsmsolns/arduino-tpms-tyre-pressure-display-b6e544

See this article for full details and background


## Modifications so far
- Usea BaseSupport library for basic ESP8266 support https://github.com/roberttidey/BaseSupport
- Uses TickTwo library for compatibility
- WifiManager for initial set up
- OTA for updates
- File browser for maintenance
- State machine added to avoid while loops in main loop which would cause watchdog problems on ESP8266
- useTestTimings is now a dynamic variable rather than a compile option
- added getData webURL to get basic data http://ip/getData
- added setMode webURL to control usetestTimings http://setMode?mode=0 or http://setMode?mode=1
	
## Extra Libraries
- BaseSupport
- WiFiManager
- TickTwo
- SSD1306Ascii

## Basic set up
- Edit passwords etc in BaseConfig.h
- Uncomment FASTCONNECT in BaseConfig as required
- Normal arduino esp8266 compile and upload
- A simple built in file uploader (/upload) should then be used to upload the base data files to SPIFF
  edit.htm.gz
  index.html
  favicon*.png
  graphs.js.gz
- The /edit operation is then supported to update further
	
