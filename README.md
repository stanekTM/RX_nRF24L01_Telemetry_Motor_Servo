# RC receiver nRF24L01 
RC receiver nRF24L01 with ATmega328P processor for smaller dimensions.
It is possible to use Arduino Nano, Pro or Pro Mini.
The firmware will be used for racing micro cars and boats.
After editing the code, it can control tanks and aircraft.
It includes a MX1508 motor driver and a CMT2150A transponder for laps timing.
Other versions of the RX firmware include servo outputs with 8 and 16 bit timers.
Telemetry sends the monitored voltage RX to TX using LED states. 
The motor driver IC is based on MX1508, MX1208, MX1515, MX1616, TC1508S and others similar, using 4x pwm input control signals.
The possibility of setting the brake is in the code.

This RC receiver works with a simple RC transmitter from my repository [RC_TX_nRF24L01_Telemetry_LED](https://github.com/stanekTM/RC_TX_nRF24L01_Telemetry_LED)

## RC receiver with motor drivers (no servos)
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
[RC_RX_nRF24L01_Telemetry_Motor_Driver](https://github.com/stanekTM/RC_RX_nRF24L01_Telemetry_Motor_Driver_Servo/blob/master/RC_RX_nRF24L01_Telemetry_Motor_Driver/RC_RX_nRF24L01_Telemetry_Motor_Driver.ino)

### Used libraries:
* <RF24.h>                      https://github.com/nRF24/RF24 
* <DigitalIO.h>                 https://github.com/greiman/DigitalIO
* "PWMFrequency.h" used locally https://github.com/TheDIYGuy999/PWMFrequency

### Function:
* Motor A (steering) = 3.9kHz (pwm 1, 2)
* Motor B (throttle) = 3.9kHz (pwm 3, 4)
* Brake = on, off or adjustable effect 
* Normal mode = LED RX is lit
* Battery voltage 1S LiPo (4.2V) < 3.3V = RX, TX LEDs flash at a interval of 500ms
* TX transmitter off or signal loss = RX LED flash at a interval of 100ms 
* Fail-safe = Motor A and B stopped

### Schema:
![Schema_Motor_Driver](https://raw.githubusercontent.com/stanekTM/RC_RX_nRF24L01_Telemetry_Motor_Driver_Servo/master/RC_RX_nRF24L01_Telemetry_Motor_Driver/Schema_Motor_Driver.PNG)

### Gerber files: 
in processing ...
![Layout_Motor_Driver](https://raw.githubusercontent.com/stanekTM/RC_RX_nRF24L01_Telemetry_Motor_Driver_Servo/master/RC_RX_nRF24L01_Telemetry_Motor_Driver/Layout_Motor_Driver.PNG)

George StanekTM
