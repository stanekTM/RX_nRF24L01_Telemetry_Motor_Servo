# RC receiver 2ch (motor driver, telemetry)
Includes nRF24L01+ transceiver and ATmega328P processor.
The motor driver IC is based on MX1208, MX1508, MX1515, MX1616, MX1919 and others similar, which uses 4x PWM input control signals.
The option to adjust the brake is included in the code.
It includes CMT2150A transponder for laps timing and motor driver.

The firmware will be used for micro cars, boats and tanks.

Works with RC transmitters [**OpenAVRc**](https://github.com/Ingwie/OpenAVRc_Dev) or [**Multiprotocol**](https://github.com/stanekTM/TX_FW_Multi_Stanek) from my fork.

## Function
* MotorA = adjustable pwm/ch1
* MotorB = adjustable pwm/ch2
* Brake = on, off or adjustable effect 
* Normal mode = LED RX is lit
* Battery voltage 1S LiPo (4.2V) < monitored voltage = RX LED flash at a interval of 0.5s
* TX transmitter off or signal loss = RX LED flash at a interval of 0.1s 
* Fail-safe = MotorA and MotorB stopped

## Arduino pins
```
D9  - pwm1/MotorA
D10 - pwm2/MotorA
D3  - pwm3/MotorB
D11 - pwm4/MotorB

D2  - LED
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

## Example micro RX
<a href="https://youtu.be/E0pgMNPuYU4"><img src="https://github.com/stanekTM/RX_nRF24_Motor_Servo/blob/master/documents/micro_rx_youtube.PNG" width="640"></a>

The PCB is created by a custom version of the open source [PCB Elegance](https://github.com/stanekTM/PCB_Elegance) and manufactured by [JLCPCB](https://jlcpcb.com)

<img src="https://raw.githubusercontent.com/stanekTM/RX_nRF24_Motor_Servo/master/documents/micro_rx_1.jpg" width="415"><img src="https://raw.githubusercontent.com/stanekTM/RX_nRF24_Motor_Servo/master/documents/micro_rx_2.jpg" width="415">
<img src="https://raw.githubusercontent.com/stanekTM/RX_nRF24_Motor_Servo/master/documents/micro_rx_3.jpg" width="415"><img src="https://raw.githubusercontent.com/stanekTM/RX_nRF24_Motor_Servo/master/documents/micro_rx_4.jpg" width="415">
<img src="https://raw.githubusercontent.com/stanekTM/RX_nRF24_Motor_Servo/master/documents/micro_rx_5.jpg" width="415"><img src="https://raw.githubusercontent.com/stanekTM/RX_nRF24_Motor_Servo/master/documents/micro_rx_6.jpg" width="415">
<img src="https://raw.githubusercontent.com/stanekTM/RX_nRF24_Motor_Servo/master/documents/micro_rx_7.jpg" width="415">

## An example of a tank-arcade mixer for OpenAVRc and OpenTX
<img src="https://raw.githubusercontent.com/stanekTM/RX_nRF24_Motor_Servo/master/documents/example_mixer_1.bmp" width="415"><img src="https://raw.githubusercontent.com/stanekTM/RX_nRF24_Motor_Servo/master/documents/example_mixer_2.bmp" width="415"><a href="https://youtu.be/AYgY5UkVnUM"><img src="https://github.com/stanekTM/RX_nRF24_Motor_Servo/blob/master/documents/rx_prototype_mix_tank_arcade_youtube.PNG" width="415"></a>

## Schema
The schematic is created by a custom version of the open source [PCB Elegance](https://github.com/stanekTM/PCB_Elegance)

![schema](https://raw.githubusercontent.com/stanekTM/RX_nRF24_Motor_Servo/master/documents/micro_rx_schema.PNG)

## Layout
The layout is created by a custom version of the open source [PCB Elegance](https://github.com/stanekTM/PCB_Elegance)

![layout](https://raw.githubusercontent.com/stanekTM/RX_nRF24_Motor_Servo/master/documents/micro_rx_layout.PNG)
