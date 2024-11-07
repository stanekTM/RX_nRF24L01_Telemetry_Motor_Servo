# RC receiver 5ch (servo driver, telemetry)
Includes nRF24L01+ transceiver and ATmega328P processor.

## Function
* Normal mode = LED RX is lit
* Battery voltage 1S LiPo (4.2V) < monitored voltage = RX LED flash at a interval of 0.5s
* TX transmitter off or signal loss = RX LED flash at a interval of 0.1s
* Fail-safe = servos 1 -> 12 set to neutral or individually set in code

## Arduino pins
```
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
* <RF24.h>      https://github.com/nRF24/RF24
* <DigitalIO.h> https://github.com/greiman/DigitalIO
* <Servo.h>     Arduino standard library
