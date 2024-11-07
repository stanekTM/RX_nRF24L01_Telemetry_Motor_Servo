# RC receiver 2ch (mix tank-arcade motor driver, telemetry)
Includes nRF24L01+ transceiver and ATmega328P processor.
The motor driver IC is based on MX1208, MX1508, MX1515, MX1616, MX1919 and others similar, which uses 4x PWM input control signals.
The option to adjust the brake is included in the code.

The firmware will be used for boats, tanks and robots.

Works with RC transmitters [**TX_nRF24_2ch_OLED**](https://github.com/stanekTM/TX_nRF24_2ch_OLED) or [**TX_nRF24_5ch_LED**](https://github.com/stanekTM/TX_nRF24_5ch_LED) from my fork.

## Function
* MotorA = adjustable pwm/ch1 - mix tank-arcade
* MotorB = adjustable pwm/ch2 - mix tank-arcade
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

## RC receiver prototype
<img src="https://raw.githubusercontent.com/stanekTM/RX_nRF24_Motor_Servo/master/documents/rx_prototype_mix_tank_arcade_1.jpg" width="415"><img src="https://raw.githubusercontent.com/stanekTM/RX_nRF24_Motor_Servo/master/documents/rx_prototype_mix_tank_arcade_2.jpg" width="415"><a href="https://youtu.be/AYgY5UkVnUM"><img src="https://github.com/stanekTM/RX_nRF24_Motor_Servo/blob/master/documents/rx_prototype_mix_tank_arcade_youtube.PNG" width="415"></a>
