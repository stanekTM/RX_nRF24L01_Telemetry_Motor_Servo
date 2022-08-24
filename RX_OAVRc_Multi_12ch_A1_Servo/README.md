# RX 12ch (servo driver, telemetry)
RC receiver nRF24L01 with ATmega328P/16Mhz, Arduino Nano or Pro Mini.
Telemetry sends the monitored voltage RX to TX. 
It contains 12 channels servo outputs.
This is the maximum of the Arduino RX receiver of the "Servo" library on the Atmega328P processor.
The firmware will be used for cars, boats, tanks, robots and aircraft.

This RC receiver works with RC transmitters [OpenAVRc](https://github.com/stanekTM/OpenAVRc_Dev) or 
[Multiprotocol](https://github.com/stanekTM/DIY-Multiprotocol-TX-Module) from my fork.

Note: I use (Arduino) ATmega328P 5V/16Mhz and supply VCC only with 3.3V voltage. 
If you supply the VCC directly with a LiPo 1S cell, except for the nRF24L01, the analog voltage measurement will not work due to the VREF.

## Function
* Normal mode = LED RX is lit
* Battery voltage 1S LiPo (4.2V) < monitored voltage = RX LED flash at a interval of 0.5s
* TX transmitter off or signal loss = RX LED flash at a interval of 0.1s
* Fail-safe = servos 1 -> 12 set to neutral or individually set in code

## Arduino pins
```
D2  - servo 1
D3  - servo 2
D4  - servo 3
D5  - servo 4
D6  - servo 5
D7  - servo 6
D8  - servo 7
D9  - servo 8
D10 - servo 9
D11 - servo 10
D12 - servo 11
D13 - servo 12

A5  - LED
A7  - telemetry analog input RX battery

nRF24L01:
A0  - CE
A1  - CSN
A2  - SCK
A3  - MOSI
A4  - MISO
```

## Used libraries
* <RF24.h>      https://github.com/nRF24/RF24 v1.3.9 
* <DigitalIO.h> https://github.com/greiman/DigitalIO
* <Servo.h>     Arduino standard library
