# RC receiver 12ch (servo driver, telemetry)
Includes nRF24L01+ transceiver and ATmega328P processor.

The firmware will be used for cars, boats, tanks, robots and aircraft.

Works with RC transmitters [**OpenAVRc**](https://github.com/Ingwie/OpenAVRc_Dev) or [**Multiprotocol**](https://github.com/stanekTM/TX_FW_Multi_Stanek) from my fork.

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

nRF24L01+:
A0  - CE
A1  - CSN
software SPI:
A2  - SCK
A3  - MOSI
A4  - MISO
```

## Used libraries
* <RF24.h>      https://github.com/nRF24/RF24
* <DigitalIO.h> https://github.com/greiman/DigitalIO
* <Servo.h>     Arduino standard library
