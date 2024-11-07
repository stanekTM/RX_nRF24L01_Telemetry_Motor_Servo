# RC receiver 10ch (motor-servo driver, telemetry)
Includes nRF24L01+ transceiver and ATmega328P processor.
The motor driver IC is based on MX1208, MX1508, MX1515, MX1616, MX1919 and others similar, which uses 4x PWM input control signals.
The option to adjust the brake is included in the code.

The firmware will be used for cars, boats, tanks, robots and aircraft.

Works with RC transmitters [**OpenAVRc**](https://github.com/Ingwie/OpenAVRc_Dev) or [**Multiprotocol**](https://github.com/stanekTM/TX_FW_Multi_Stanek) from my fork.

## Function
* MotorA = adjustable pwm/ch1
* MotorB = adjustable pwm/ch2
* Brake = on, off or adjustable effect 
* Normal mode = LED RX is lit
* Battery voltage 1S LiPo (4.2V) < monitored voltage = RX, TX LEDs flash at a interval of 0.5s
* TX transmitter off or signal loss = RX LED flash at a interval of 0.1s 
* Fail-safe = MotorA and MotorB stopped, servos 1 -> 8 set to neutral or individually set in code

## Arduino pins
```
D2  - servo 1
D4  - servo 2
D7  - servo 3
D8  - servo 4
D9  - servo 5
D10 - servo 6
D12 - servo 7
D13 - servo 8

D5  - pwm1/MotorA
D6  - pwm2/MotorA
D3  - pwm3/MotorB
D11 - pwm4/MotorB

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
* <Servo.h> Arduino standard library
