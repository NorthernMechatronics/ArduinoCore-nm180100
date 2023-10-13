# Adafruit Capacative Touchscreen Guide

This guide will supplement the official guide for using the <a href="https://learn.adafruit.com/adafruit-2-8-and-3-2-color-tft-touchscreen-breakout-v2" target="_blank">Adafruit 2.8" and 3.2" Color TFT Touchscreen Breakout v2</a> with your NM180100 Evaluation Board.

## Requirements

### Hardware
* NM180100EVB
* A USB to micro-USB cable to connect the board
* A Linux or Windows machine
* Hardware to connect the touchscreen to your EVB 

### Software
* Arduino IDE with NM180100EVB Arduino Core set up. <a href="./doc/NM180100EVB_User_Guide.md" target="_blank">Setup Instructions</a>

For this tutorial, wire the display up in SPI mode.

## SPI Wiring

|Touchscreen pin|EVB pin    |
|---------------|-----------|
|3-5V Vin       |5V         |
|GND            |GND (any)  |
|CLK            |5          |
|MISO           |6          |
|MOSI           |7          |
|CS             |31         |
|D/C            |32         |

#### The code will need to be adjusted for CD and D/C

Find the section that has 
```
#define TFT_CS 9
#define TFT_DC 10
```
![Original CS and DC mapping](./res/touchscreen_cs_dc_old.png)

Change the code to:
```
#define TFT_CS 31
#define TFT_DC 32
```
![Corrected CS and DC mapping](./res/touchscreen_cs_dc_new.png)

<b>Note that in some cases these entries may be swapped, so be careful when changing the values, or just copy the snippet above if in doubt.</b>

## Capacative Touchscreen I2C Wiring

|Touchscreen pin|EVB pin|
|---------------|-------|
|SCL            |8      |
|SDA            |9      |


</br>
</br>
</br>
</br>


# Adafruit Motor Shield V2 Guide

This guide will supplement the official guide for using the <a href="https://learn.adafruit.com/adafruit-motor-shield-v2-for-arduino/overview" target="_blank">Adafruit Motor Shield V2</a> with your NM180100 Evaluation Board.

## Requirements

### Hardware
* NM180100EVB
* A USB to micro-USB cable to connect the board
* A Linux or Windows machine
* Some motors to drive!