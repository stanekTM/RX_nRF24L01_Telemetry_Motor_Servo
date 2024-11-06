# RC receiver (motor-servo driver, telemetry)
RC receiver with fixed RF channel and fixed address.
Telemetry sends measured voltage and "fake" RSSI (the nRF24L01+ transceiver does not contain real RSSI and is only a rough measurement of lost packets).

The hardware includes a low-cost nRF24L01+ transceiver and an ATmega328P processor.
The motor driver IC is based on MX1208, MX1508, MX1515, MX1616, MX1919 and others similar, which uses 4x PWM input control signals.

The option to adjust the brake is included in the code.

Other versions of the RC receiver firmware include servo outputs.
The firmware will be used for cars, boats, tanks, robots and aircraft.

**This RC receiver works with RC transmitters**
* [**TX_nRF24_2ch_OLED**](https://github.com/stanekTM/TX_nRF24_2ch_OLED)
* [**TX_nRF24_5ch_LED**](https://github.com/stanekTM/TX_nRF24_5ch_LED)
* [**OpenAVRc**](https://github.com/Ingwie/OpenAVRc_Dev)
* [**Multiprotocol**](https://github.com/stanekTM/TX_FW_Multi_Stanek) from my fork.

All versions of the code are based on this [scheme](https://raw.githubusercontent.com/stanekTM/RX_nRF24_Motor_Servo/master/documents/micro_rx_schema.PNG), but each version uses different pins.

## Used libraries
* <RF24.h>      https://github.com/nRF24/RF24
* <DigitalIO.h> https://github.com/greiman/DigitalIO
* <Servo.h>     Arduino standard library

## Example micro RC receiver
<a href="https://youtu.be/E0pgMNPuYU4"><img src="documents/micro_rx_youtube.PNG" width="640"></a>

The PCB is created by a custom version of the open source [PCB Elegance](https://github.com/stanekTM/PCB_Elegance) and manufactured by [JLCPCB](https://jlcpcb.com)

<img src="https://raw.githubusercontent.com/stanekTM/RX_nRF24_Motor_Servo/master/documents/micro_rx_1.jpg" width="415"><img src="https://raw.githubusercontent.com/stanekTM/RX_nRF24_Motor_Servo/master/documents/micro_rx_2.jpg" width="415">
<img src="https://raw.githubusercontent.com/stanekTM/RX_nRF24_Motor_Servo/master/documents/micro_rx_3.jpg" width="415"><img src="https://raw.githubusercontent.com/stanekTM/RX_nRF24_Motor_Servo/master/documents/micro_rx_4.jpg" width="415">
<img src="https://raw.githubusercontent.com/stanekTM/RX_nRF24_Motor_Servo/master/documents/micro_rx_5.jpg" width="415"><img src="https://raw.githubusercontent.com/stanekTM/RX_nRF24_Motor_Servo/master/documents/micro_rx_6.jpg" width="415">
<img src="https://raw.githubusercontent.com/stanekTM/RX_nRF24_Motor_Servo/master/documents/micro_rx_7.jpg" width="415">

## RC receiver prototype (motor-servo driver, telemetry)
<img src="https://raw.githubusercontent.com/stanekTM/RX_nRF24_Motor_Servo/master/documents/rx_prototype_motor_servo_1.jpg" width="415"><img src="https://raw.githubusercontent.com/stanekTM/RX_nRF24_Motor_Servo/master/documents/rx_prototype_motor_servo_2.jpg" width="415">

## RC receiver prototype [(mix tank-arcade motor driver, telemetry)](https://github.com/stanekTM/RX_nRF24_Motor_Servo/tree/master/RX_nRF24_2ch_Motor_Mix_Tank)
<img src="https://raw.githubusercontent.com/stanekTM/RX_nRF24_Motor_Servo/master/documents/rx_prototype_mix_tank_arcade_1.jpg" width="415"><img src="https://raw.githubusercontent.com/stanekTM/RX_nRF24_Motor_Servo/master/documents/rx_prototype_mix_tank_arcade_2.jpg" width="415"><a href="https://youtu.be/AYgY5UkVnUM"><img src="https://github.com/stanekTM/RX_nRF24_Motor_Servo/blob/master/documents/rx_prototype_mix_tank_arcade_youtube.PNG" width="415"></a>
