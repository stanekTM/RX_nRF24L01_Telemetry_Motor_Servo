# RC receiver 5ch (servo driver, telemetry)
Includes nRF24L01+ transceiver and ATmega328P processor.

The firmware will be used for cars, boats, tanks, robots and aircraft.

Works with RC transmitters [**TX_nRF24_2ch_OLED**](https://github.com/stanekTM/TX_nRF24_2ch_OLED) or [**TX_nRF24_5ch_LED**](https://github.com/stanekTM/TX_nRF24_5ch_LED) from my fork.

## Function
* Normal mode = LED RX is lit
* Battery voltage 1S LiPo (4.2V) < monitored voltage = RX LED flash at a interval of 0.5s
* TX transmitter off or signal loss = RX LED flash at a interval of 0.1s
* Fail-safe = servos 1 -> 5 set to neutral or individually set in code

## Arduino pins
```
D9  - servo 1
D10 - servo 2
D11 - servo 3
D12 - servo 4
D13 - servo 5

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
