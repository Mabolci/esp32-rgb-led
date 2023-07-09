# Smarthome - RGB Led Strip Controller

ESP32 microcontroller code for Domoticz control of RGB LED strip.

## Hardware 

* ESP32 (ESP8266 can also be used)
* 3x Mosfet modules
* RGB LED strip
* 12V power supply

## Setup and Usage

### Software

You can choose which GPIO ports are used for the RGB led strip control. You can assign them as follows.

```cpp
const int redPin = GPIO1;
const int greenPin = GPIO2;   
const int bluePin = GPIO3;  
```

### Hardware

Connect the GND pin on the MOSFET modules to the GND of ESP32. Next connect the PWN to the chosen GPIO pins. The + on the module should be connected to 12V power, and the - should be connected to the ground of the 12V power supply. The load on the MOSFET module should be connected to the right cable on the RGB led strip.

