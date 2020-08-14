# RC receiver nRF24L01 
RC receiver nRF24L01 with ATmega328P processor for smaller dimensions. It is possible to use Arduino Nano, Pro or Pro Mini.
It also includes a MX1508 motor driver with brake and a CMT2150A transponder for laps timing.
Other versions of the RX firmware include servo outputs with 8 and 16 bit timers.
The firmware will be used for racing micro cars and boats.
After editing the code, it can control tanks and aircraft.
The possibility of setting the brake is in the code.
Telemetry sends the monitored voltage RX to TX. When the minimum voltage is exceeded, both RX and TX LEDs start flashing at a frequency of 500ms. 

## RC receiver with motor drivers (no servos)
1. Motor A (pwm 1, 2) = 3.9 kHz (steering) 
2. Motor B (pwm 3, 4) = 3.9 kHz (throttle)
3. Battery voltage 1S LiPo (4.2V) < 3.3V = RX, TX LEDs flash at a frequency of 500ms
4. TX transmitter off or signal loss = RX LED flash at a frequency of 100ms

![screen1](https://github.com/stanekTM/RC_RX_nRF24L01_Lite_Telemetry_Motor_Driver_Servo/blob/master/RC_RX_nRF24L01_Lite_Telemetry_Motor_Driver/RC_RX_nRF24L01_Lite_Telemetry_Motor_Driver.PNG)

George StanekTM
