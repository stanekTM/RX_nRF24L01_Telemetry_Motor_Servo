# RC receiver nRF24L01 
RC receiver nRF24L01 with ATmega328P processor for smaller dimensions. It is possible to use Arduino Nano, Pro or Pro Mini.
It also includes a MX1508 motor driver with brake and a CMT2150A transponder for laps timing.
Other versions of the RX firmware include servo outputs with 8 and 16 bit timers.
The firmware will be used for racing micro cars and boats.
After editing the code, it can control tanks and aircraft.
The possibility of setting the brake is in the code.
Telemetry sends the monitored voltage RX to TX using LED states. 

This RC receiver works with a simple RC transmitter from my repository https://github.com/stanekTM/RC_TX_nRF24L01_Lite_Telemetry_LED

## RC receiver with motor drivers (no servos)
The motor driver IC is based on MX1508, MX1208, MX1515, MX1616, TC1508S and others similar, using 4x pwm input control signals.

### Function:
1. Motor A (steering) = 3.9kHz (pwm 1, 2)
2. Motor B (throttle) = 3.9kHz (pwm 3, 4)
3. Brake = on, off or adjustable effect 
4. Normal mode = LED RX is lit
5. Battery voltage 1S LiPo (4.2V) < 3.3V = RX, TX LEDs flash at a interval of 500ms
6. TX transmitter off or signal loss = RX LED flash at a interval of 100ms 
7. Fail-safe = Motor A and B stopped

### Schema:
![screen1](https://raw.githubusercontent.com/stanekTM/RC_RX_nRF24L01_Lite_Telemetry_Motor_Driver_Servo/master/RC_RX_nRF24L01_Lite_Telemetry_Motor_Driver/RC_RX_nRF24L01_Lite_Telemetry_Motor_Driver.PNG)

### Gerber files: in processing ...

George StanekTM
