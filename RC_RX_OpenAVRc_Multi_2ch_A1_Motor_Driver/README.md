# RC receiver nRF24L01 for OpenAVRc and Multiprotocol/OpenTX
RC receiver nRF24L01 with ATmega328P processor for smaller dimensions.
It is possible to use Arduino Nano, Pro or Pro Mini.
Telemetry sends the monitored voltage RX to TX. 
It includes CMT2150A transponder for laps timing and motor driver.
The motor driver IC is based on MX1508, MX1208, MX1515, MX1616, TC1508S and others similar, using 4x pwm input control signals.
The possibility of setting the brake is in the code.
The firmware will be used for racing micro cars and boats.
After editing the code, it can control tanks and aircraft.

This RC receiver works with RC transmitters [OpenAVRc](https://github.com/stanekTM/OpenAVRc_Dev) from my fork or [DIY-Multiprotocol-TX-Module](https://github.com/stanekTM/DIY-Multiprotocol-TX-Module) from my fork.

## RC receiver with motor drivers (no servos)
### Function:
* Motor A (steering, ch1) = 3.9kHz (pwm 1, 2)
* Motor B (throttle, ch2) = 3.9kHz (pwm 3, 4)
* Brake = on, off or adjustable effect 
* Normal mode = LED RX is lit
* Battery voltage 1S LiPo (4.2V) < 3.3V = RX LED flash at a interval of 500ms
* TX transmitter off or signal loss = RX LED flash at a interval of 100ms 
* Fail-safe = Motor A and B stopped

### Arduino pins:
* D9  - pwm1/Motor A
* D10 - pwm2/Motor A
* D3  - pwm3/Motor B
* D11 - pwm4/Motor B
#
* D2  - LED
* A7  - telemetry analog input RX battery
#
nRF24L01:
* A0  - CE
* A1  - CSN
* A2  - SCK
* A3  - MOSI
* A4  - MISO

### Code:
[RC_RX_OpenAVRc_Multi_2ch_A1_Motor_Driver](https://github.com/stanekTM/RC_RX_nRF24L01_Telemetry_Motor_Driver_Servo/blob/master/RC_RX_OpenAVRc_Multi_2ch_A1_Motor_Driver/RC_RX_OpenAVRc_Multi_2ch_A1_Motor_Driver.ino)

### Used libraries:
* <RF24.h>                      https://github.com/nRF24/RF24 
* <DigitalIO.h>                 https://github.com/greiman/DigitalIO
* "PWMFrequency.h" used locally https://github.com/TheDIYGuy999/PWMFrequency

### Schema:
![Schema_Motor_Driver](https://raw.githubusercontent.com/stanekTM/RC_RX_nRF24L01_Telemetry_Motor_Driver_Servo/master/RC_RX_OpenAVRc_Multi_2ch_A1_Motor_Driver/Schema_Motor_Driver.PNG)

### Gerber files: 
in processing ...
![Layout_Motor_Driver](https://raw.githubusercontent.com/stanekTM/RC_RX_nRF24L01_Telemetry_Motor_Driver_Servo/master/RC_RX_OpenAVRc_Multi_2ch_A1_Motor_Driver/Layout_Motor_Driver.PNG)
#
George StanekTM
