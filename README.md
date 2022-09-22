# RX nRF24L01 (motor-servo driver, telemetry)
RC receiver nRF24L01 with ATmega328P/16Mhz, Arduino Nano or Pro Mini.
Telemetry sends the monitored voltage A1 and RSSI to TX.

The motor driver IC is based on MX1208, MX1508, MX1515, MX1616, MX1919 and others similar, which uses 4x PWM input control signals.
The option to adjust the brake is included in the code.

Other versions of the RX firmware include servo outputs.
The firmware will be used for cars, boats, tanks, robots and aircraft.

**This RC receiver works with RC transmitters**
* [**TX_nRF24L01_4ch_Telemetry_LCD**](https://github.com/stanekTM/TX_nRF24L01_4ch_Telemetry_LCD)
* [**TX_nRF24L01_5ch_Telemetry_LED**](https://github.com/stanekTM/TX_nRF24L01_5ch_Telemetry_LED)
* [**OpenAVRc**](https://github.com/stanekTM/OpenAVRc_Dev)
* [**Multiprotocol**](https://github.com/stanekTM/DIY-Multiprotocol-TX-Module) from my fork.

Note: I use (Arduino) ATmega328P 5V/16Mhz and supply VCC only with 3.3V voltage.
If you supply the VCC directly with a LiPo 1S cell, except for the nRF24L01, the analog voltage measurement will not work due to the VREF.
All versions of the code are based on this [scheme](https://raw.githubusercontent.com/stanekTM/RX_nRF24L01_Telemetry_Motor_Servo/master/documents/micro_rx_schema.PNG), but each version uses different pins.

## Used libraries
* <RF24.h>                      https://github.com/nRF24/RF24 v1.3.9
* <DigitalIO.h>                 https://github.com/greiman/DigitalIO
* "PWMFrequency.h" used locally https://github.com/TheDIYGuy999/PWMFrequency
* <Servo.h>        Arduino standard library

## Example micro RX
<a href="https://youtu.be/E0pgMNPuYU4"><img src="documents/micro_rx_youtube.PNG" width="640"></a>

The PCB is created by a custom version of the open source [PCB Elegance](https://github.com/stanekTM/PCB_Elegance) and manufactured by [JLCPCB](https://jlcpcb.com)

<img src="https://raw.githubusercontent.com/stanekTM/RX_nRF24L01_Telemetry_Motor_Servo/master/documents/micro_rx_1.jpg" width="415"><img src="https://raw.githubusercontent.com/stanekTM/RX_nRF24L01_Telemetry_Motor_Servo/master/documents/micro_rx_2.jpg" width="415">
<img src="https://raw.githubusercontent.com/stanekTM/RX_nRF24L01_Telemetry_Motor_Servo/master/documents/micro_rx_3.jpg" width="415"><img src="https://raw.githubusercontent.com/stanekTM/RX_nRF24L01_Telemetry_Motor_Servo/master/documents/micro_rx_4.jpg" width="415">
<img src="https://raw.githubusercontent.com/stanekTM/RX_nRF24L01_Telemetry_Motor_Servo/master/documents/micro_rx_5.jpg" width="415"><img src="https://raw.githubusercontent.com/stanekTM/RX_nRF24L01_Telemetry_Motor_Servo/master/documents/micro_rx_6.jpg" width="415">
<img src="https://raw.githubusercontent.com/stanekTM/RX_nRF24L01_Telemetry_Motor_Servo/master/documents/micro_rx_7.jpg" width="415">

## RX prototype (motor-servo driver, telemetry)
<img src="https://raw.githubusercontent.com/stanekTM/RX_nRF24L01_Telemetry_Motor_Servo/master/documents/rx_prototype_motor_servo_1.jpg" width="415"><img src="https://raw.githubusercontent.com/stanekTM/RX_nRF24L01_Telemetry_Motor_Servo/master/documents/rx_prototype_motor_servo_2.jpg" width="415">

## RX prototype [(mix tank-arcade motor driver, telemetry)](https://github.com/stanekTM/RX_nRF24L01_Telemetry_Motor_Servo/tree/master/RX_nRF24L01_2ch_A1_Motor_Mix_Tank)
<img src="https://raw.githubusercontent.com/stanekTM/RX_nRF24L01_Telemetry_Motor_Servo/master/documents/rx_prototype_mix_tank_arcade_1.jpg" width="415"><img src="https://raw.githubusercontent.com/stanekTM/RX_nRF24L01_Telemetry_Motor_Servo/master/documents/rx_prototype_mix_tank_arcade_2.jpg" width="415">
