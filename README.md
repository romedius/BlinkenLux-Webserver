This is the firmware for our first Version of the BlinkenLux ESP8266 project developed for the Temporary Space Invasion at Freies Theater Innsbruck

WARNING: This is a documentation stub!

# Parts 

This boards is populated with three important components:
* ESP8266 WiFi module and microcntroller 
* CH340G USB to Serial converter
* WS2812 LEDs, also known as NeoPixels

# Software and drivers

To program this board you need to get the Arduino IDE from https://www.arduino.cc/en/Main/Software
and add the ESP8266 plugin (TODO).

On Windows and mac you need drivers for the CH340G chip, on Linux they should be included in a reasonably new distribution.

# Programming 

To program the board please hold downthe reset button and press the program button at the same time. You can release the reset button now and release the program button, as soon as the Arduino IDE starts to write the firmware.
