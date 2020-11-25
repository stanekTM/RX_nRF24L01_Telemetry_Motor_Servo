# RC receiver nRF24L01 
RC receiver nRF24L01 with ATmega328P or Arduino Nano, Pro or Pro Mini.
Telemetry sends the monitored voltage RX to TX. 
The motor driver IC is based on MX1508, MX1208, MX1515, MX1616, TC1508S and others similar, using 4x pwm input control signals.
The possibility of setting the brake is in the code.
Other versions of the RX firmware include servo outputs with 8 and 16 bit timers.
The firmware will be used for racing cars, boats, tanks, robots and aircraft.

This RC receiver works with RC transmitters [RC_TX_nRF24L01_Telemetry_LCD](https://github.com/stanekTM/RC_TX_nRF24L01_Telemetry_LCD), 
[RC_TX_nRF24L01_Telemetry_LED](https://github.com/stanekTM/RC_TX_nRF24L01_Telemetry_LED), 
[OpenAVRc](https://github.com/stanekTM/OpenAVRc_Dev) or [DIY-Multiprotocol-TX-Module](https://github.com/stanekTM/DIY-Multiprotocol-TX-Module) from my fork.
#
### Used libraries:
* <RF24.h>                      https://github.com/nRF24/RF24 
* <DigitalIO.h>                 https://github.com/greiman/DigitalIO
* "PWMFrequency.h" used locally https://github.com/TheDIYGuy999/PWMFrequency
* "ServoTimer2.h"  used locally https://github.com/nabontra/ServoTimer2
* <Servo.h>        Arduino standard library

George StanekTM
