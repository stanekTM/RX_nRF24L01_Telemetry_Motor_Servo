## RC receiver nRF24L01 12 channels servo outputs for OpenAVRc and Multiprotocol/OpenTX
RC receiver nRF24L01 with ATmega328P or Arduino Nano, Pro or Pro Mini.
Telemetry sends the monitored voltage RX to TX. 
It contains 12 channels servo outputs with 16 bit timers.
The firmware will be used for racing cars, boats, tanks, robots and aircraft.

This RC receiver works with RC transmitters [OpenAVRc](https://github.com/stanekTM/OpenAVRc_Dev) or 
[DIY-Multiprotocol-TX-Module](https://github.com/stanekTM/DIY-Multiprotocol-TX-Module) from my fork.

### Function:
* Normal mode = LED RX is lit
* Battery voltage 1S LiPo (4.2V) < 3.3V = RX LED flash at a interval of 500ms
* TX transmitter off or signal loss = RX LED flash at a interval of 100ms
* Fail-safe = servos 1 -> 12 set to neutral or individually set in code

### Arduino pins:
* D2  - servo 1
* D3  - servo 2
* D4  - servo 3
* D5  - servo 4
* D6  - servo 5
* D7  - servo 6
* D8  - servo 7
* D9  - servo 8
* D10 - servo 9
* D11 - servo 10
* D12 - servo 11
* D13 - servo 12
#
* A5  - LED
* A7  - telemetry analog input RX battery
#
nRF24L01:
* A0  - CE
* A1  - CSN
* A2  - SCK
* A3  - MOSI
* A4  - MISO

### Code:
[RC_RX_OpenAVRc_Multi_12ch_A1_Servo_16bit](https://github.com/stanekTM/RC_RX_nRF24L01_Telemetry_Motor_Driver_Servo/blob/master/RC_RX_OpenAVRc_Multi_12ch_A1_Servo_16bit/RC_RX_OpenAVRc_Multi_12ch_A1_Servo_16bit.ino)

### Used libraries:
* <RF24.h>                      https://github.com/nRF24/RF24 
* <DigitalIO.h>                 https://github.com/greiman/DigitalIO
* "PWMFrequency.h" used locally https://github.com/TheDIYGuy999/PWMFrequency
* <Servo.h>        Arduino standard library
#
George StanekTM
