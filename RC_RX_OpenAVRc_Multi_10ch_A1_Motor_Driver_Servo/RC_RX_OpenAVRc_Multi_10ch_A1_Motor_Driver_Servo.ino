
//********************************************************************************************
//Communication nRF24L01P "Stanek". Fixed RF channel, fixed address.                         *
//Support for OpenAVRc transmitter https://github.com/stanekTM/OpenAVRc_Dev                  *
//and for the Multiprotocol TX modul https://github.com/stanekTM/DIY-Multiprotocol-TX-Module *
//********************************************************************************************

#include <RF24.h>         //https://github.com/nRF24/RF24
#include <nRF24L01.h>
//#include <printf.h>       //print the radio debug info
#include <DigitalIO.h>    //https://github.com/greiman/DigitalIO
#include <Servo.h>        //Arduino standard library
#include "PWMFrequency.h" //used locally https://github.com/TheDIYGuy999/PWMFrequency


//settings PWM MotorA (pin D5 or D6)
//1024 = 61Hz, 256 = 244Hz, 64 = 976Hz(default), 8 = 7812Hz 
#define pwm_motorA 256

//settings PWM MotorB (pin D3 or D11)
//1024 = 30Hz, 256 = 122Hz, 128 = 244Hz, 64 = 488Hz(default), 32 = 976Hz, 8 = 3906Hz  
#define pwm_motorB 256

//Brake setting, adjustment (0-255), no brake 0, max brake 255
#define brake_motorA 255
#define brake_motorB 0

//LED alarm battery voltage setting
#define battery_voltage   4.2
#define monitored_voltage 3.49

//setting the dead zone of poor quality joysticks TX for the motor controller
#define dead_zone  10

//PPM settings
#define servoMid   1500
#define servoMin   1000
#define servoMax   2000

//free pins
//pin              0
//pin              1
//pin              A6

//pins for servos
#define pin_servo1 2
#define pin_servo2 4
#define pin_servo3 7
#define pin_servo4 8
#define pin_servo5 9
#define pin_servo6 10
#define pin_servo7 12 //MISO
#define pin_servo8 13 //SCK 
 
//pwm pins for motor
#define pin_pwm1_motorA 5
#define pin_pwm2_motorA 6
#define pin_pwm3_motorB 3
#define pin_pwm4_motorB 11 //MOSI

//LED RX battery and RF on/off
#define pin_LED    A5

//input RX battery
#define pin_RXbatt A7

//pins for nRF24L01
#define pin_CE     A0 
#define pin_CSN    A1 

//software SPI http://tmrh20.github.io/RF24/Arduino.html
//----- SCK   16 - A2
//----- MOSI  17 - A3
//----- MISO  18 - A4

//setting of CE and CSN pins
RF24 radio(pin_CE, pin_CSN);

//RF communication channel settings (0-125, 2.4Ghz + 76 = 2.476Ghz)
#define radio_channel 76

//setting RF channels addresses
const byte tx_rx_address[] = "tx001";
const byte rx_p1_address[] = "rx002";

//************************************************************************************************************************************************************************
//this structure defines the received data in bytes (structure size max. 32 bytes) ***************************************************************************************
//************************************************************************************************************************************************************************
struct packet
{
  unsigned int ch1; //MotorA
  unsigned int ch2; //MotorB
  unsigned int ch3;
  unsigned int ch4;
  unsigned int ch5;
  unsigned int ch6;
  unsigned int ch7;
  unsigned int ch8;
  unsigned int ch9;
  unsigned int ch10;
};
packet rc_data; //create a variable with the above structure

//************************************************************************************************************************************************************************
//this struct defines data, which are embedded inside the ACK payload ****************************************************************************************************
//************************************************************************************************************************************************************************
struct ackPayload
{
  unsigned int RXbatt; //0-255 for OpenAVRc and OpenTX Multiprotocol telemetry
};
ackPayload payload;

//************************************************************************************************************************************************************************
//reset values ​​(servoMin = 1000us, servoMid = 1500us, servoMax = 2000us) *************************************************************************************************
//************************************************************************************************************************************************************************
void resetData()
{
  rc_data.ch1  = servoMid; //MotorA
  rc_data.ch2  = servoMid; //MotorB
  rc_data.ch3  = servoMid;     
  rc_data.ch4  = servoMid;
  rc_data.ch5  = servoMid;     
  rc_data.ch6  = servoMid;
  rc_data.ch7  = servoMid;     
  rc_data.ch8  = servoMid;
  rc_data.ch9  = servoMid;
  rc_data.ch10 = servoMid;
}

//************************************************************************************************************************************************************************
//create servo object ****************************************************************************************************************************************************
//************************************************************************************************************************************************************************
Servo servo1, servo2, servo3, servo4, servo5, servo6, servo7, servo8;

void attachServoPins()
{
  servo1.attach(pin_servo1);
  servo2.attach(pin_servo2);
  servo3.attach(pin_servo3);
  servo4.attach(pin_servo4);
  servo5.attach(pin_servo5);
  servo6.attach(pin_servo6);
  servo7.attach(pin_servo7);
  servo8.attach(pin_servo8);
}

int ch3_value = 0, ch4_value = 0, ch5_value = 0, ch6_value = 0, ch7_value = 0, ch8_value = 0, ch9_value = 0, ch10_value = 0;

void outputServo()
{
  servo1.writeMicroseconds(ch3_value);   
  servo2.writeMicroseconds(ch4_value); 
  servo3.writeMicroseconds(ch5_value);   
  servo4.writeMicroseconds(ch6_value); 
  servo5.writeMicroseconds(ch7_value);   
  servo6.writeMicroseconds(ch8_value); 
  servo7.writeMicroseconds(ch9_value);
  servo8.writeMicroseconds(ch10_value);

  ch3_value  = map(rc_data.ch3, servoMin, servoMax, servoMin, servoMax); 
  ch4_value  = map(rc_data.ch4, servoMin, servoMax, servoMin, servoMax);
  ch5_value  = map(rc_data.ch5, servoMin, servoMax, servoMin, servoMax); 
  ch6_value  = map(rc_data.ch6, servoMin, servoMax, servoMin, servoMax);
  ch7_value  = map(rc_data.ch7, servoMin, servoMax, servoMin, servoMax); 
  ch8_value  = map(rc_data.ch8, servoMin, servoMax, servoMin, servoMax);
  ch9_value  = map(rc_data.ch9, servoMin, servoMax, servoMin, servoMax);
  ch10_value = map(rc_data.ch10, servoMin, servoMax, servoMin, servoMax); 

//  Serial.println(rc_data.ch3); //print value ​​on a serial monitor 
}

//************************************************************************************************************************************************************************
//setup frequencies and motors control ***********************************************************************************************************************************
//************************************************************************************************************************************************************************
int value_motorA = 0, value_motorB = 0; 

void outputPWM()
{  
/*
 * The base frequency for pins 3, 9, 10, 11 is 31250Hz.
 * The base frequency for pins 5, 6         is 62500Hz.
 * 
 * The divisors available on pins 5, 6, 9, 10 are: 1, 8, 64, 256, and 1024.
 * The divisors available on pins 3, 11       are: 1, 8, 32, 64, 128, 256, and 1024.
 *    
 * Pins 5, 6  are paired on timer0, functions delay(), millis() and micros()
 * D5   pwm 976Hz(default), timer0, 8-bit 
 * D6   pwm 976Hz(default), timer0, 8-bit
 * 
 * Pins 9, 10 are paired on timer1, Servo library
 * D9   pwm 488Hz(default), timer1, 16-bit
 * D10  pwm 488Hz(default), timer1, 16-bit    
 * 
 * Pins 3, 11 are paired on timer2, ServoTimer2 library
 * D3   pwm 488Hz(default), timer2, 8-bit
 * D11  pwm 488Hz(default), timer2, 8-bit, SPI MOSI hardware
*/
 
//MotorA PWM frequency pin D5 or pin D6
//1024 = 61Hz, 256 = 244Hz, 64 = 976Hz(default), 8 = 7812Hz 
  setPWMPrescaler(pin_pwm1_motorA, pwm_motorA);

//MotorB PWM frequency pin D3 or pin D11
//1024 = 30Hz, 256 = 122Hz, 128 = 244Hz, 64 = 488Hz(default), 32 = 976Hz, 8 = 3906Hz  
  setPWMPrescaler(pin_pwm3_motorB, pwm_motorB);  

//MotorA --------------------------------------------------------------------------------------

  if (rc_data.ch1 < servoMid - dead_zone)
  {
    value_motorA = map(rc_data.ch1, servoMid - dead_zone, servoMin, 0, 255);
    analogWrite(pin_pwm1_motorA, value_motorA); 
    digitalWrite(pin_pwm2_motorA, LOW);
  }
  else if (rc_data.ch1 > servoMid + dead_zone)
  {
    value_motorA = map(rc_data.ch1, servoMid + dead_zone, servoMax, 0, 255);
    analogWrite(pin_pwm2_motorA, value_motorA); 
    digitalWrite(pin_pwm1_motorA, LOW);
  }
  else
  {
    analogWrite(pin_pwm1_motorA, brake_motorA);
    analogWrite(pin_pwm2_motorA, brake_motorA);
  }

//  Serial.println(rc_data.ch1); //print value ​​on a serial monitor
  
//MotorB --------------------------------------------------------------------------------------

  if (rc_data.ch2 < servoMid - dead_zone)
  {
    value_motorB = map(rc_data.ch2, servoMid - dead_zone, servoMin, 0, 255); 
    analogWrite(pin_pwm3_motorB, value_motorB); 
    digitalWrite(pin_pwm4_motorB, LOW);
  }
  else if (rc_data.ch2 > servoMid + dead_zone)
  {
    value_motorB = map(rc_data.ch2, servoMid + dead_zone, servoMax, 0, 255); 
    analogWrite(pin_pwm4_motorB, value_motorB); 
    digitalWrite(pin_pwm3_motorB, LOW);
  }
  else
  {
    analogWrite(pin_pwm3_motorB, brake_motorB);
    analogWrite(pin_pwm4_motorB, brake_motorB);
  }
}

//************************************************************************************************************************************************************************
//initial main settings **************************************************************************************************************************************************
//************************************************************************************************************************************************************************
void setup()
{
//  Serial.begin(9600); //print value ​​on a serial monitor
//  printf_begin();     //print the radio debug info

  pinMode(pin_pwm1_motorA, OUTPUT);
  pinMode(pin_pwm2_motorA, OUTPUT);
  pinMode(pin_pwm3_motorB, OUTPUT);
  pinMode(pin_pwm4_motorB, OUTPUT);
  
  pinMode(pin_LED, OUTPUT);
  pinMode(pin_RXbatt, INPUT);
  
  resetData();
  attachServoPins();

  //define the radio communication
  radio.begin();  
  radio.setAutoAck(true);          //ensure autoACK is enabled (default true)
  radio.enableAckPayload();        //enable Ack dynamic payloads. This only works on pipes 0&1 by default
  radio.enableDynamicPayloads();   //enable dynamic payloads on all pipes

//  radio.enableDynamicAck();
//  radio.setPayloadSize(10);        //set static payload size. Default max. 32 bytes
//  radio.setCRCLength(RF24_CRC_16); //RF24_CRC_8, RF24_CRC_16
//  radio.setAddressWidth(5);        //the address width in bytes 3, 4 or 5 (24, 32 or 40 bit)

  radio.setRetries(5, 5);          //set the number and delay of retries on failed submit (max. 15 x 250us delay (blocking !), max. 15 retries)
  
  radio.setChannel(radio_channel); //which RF channel to communicate on (0-125, 2.4Ghz + 76 = 2.476Ghz)
  radio.setDataRate(RF24_250KBPS); //RF24_250KBPS (fails for units without +), RF24_1MBPS, RF24_2MBPS
  radio.setPALevel(RF24_PA_MIN);   //RF24_PA_MIN (-18dBm), RF24_PA_LOW (-12dBm), RF24_PA_HIGH (-6dbm), RF24_PA_MAX (0dBm) 

  radio.openWritingPipe(tx_rx_address);    //open a pipe for writing via byte array
  radio.openReadingPipe(1, rx_p1_address); //open all the required reading pipes, and then call "startListening"
                                          
  radio.startListening(); //set the module as receiver. Start listening on the pipes opened for reading
}

//************************************************************************************************************************************************************************
//program loop ***********************************************************************************************************************************************************
//************************************************************************************************************************************************************************
void loop()
{
  receive_time();
  send_and_receive_data();

  outputServo();
  outputPWM();

//  Serial.println("Radio details *****************");
//  radio.printDetails(); //print the radio debug info
 
} //end program loop

//************************************************************************************************************************************************************************
//get time after losing RF data or turning off the TX, reset data and the LED flashing ***********************************************************************************
//************************************************************************************************************************************************************************
unsigned long lastRxTime = 0;

void receive_time()
{
  if(millis() >= lastRxTime + 400) //400 = 3.3VCC, 1000 = 5VCC
  {
    resetData();       
    RFoff_check(); 
  }
}

//************************************************************************************************************************************************************************
//send and receive data **************************************************************************************************************************************************
//************************************************************************************************************************************************************************
void send_and_receive_data()
{
  byte pipeNo;
  
  if (radio.available(&pipeNo))
  {
    radio.writeAckPayload(pipeNo, &payload, sizeof(ackPayload));
   
    radio.read(&rc_data, sizeof(packet));
    
    lastRxTime = millis(); //at this moment we have received the data
    RX_batt_check();                     
  } 
}

//************************************************************************************************************************************************************************
//measuring the input of the RX battery. After receiving RF data, the monitored RX battery is activated ******************************************************************
//when RX battery_voltage < monitored_voltage = LED alarm RX flash at a interval of 0.5s. Battery OK = LED RX is lit *****************************************************
//************************************************************************************************************************************************************************
unsigned long ledTime = 0;
int ledState, detect;

void RX_batt_check()
{
  payload.RXbatt = map(analogRead(pin_RXbatt), 0, 1023, 0, 255);

  detect = payload.RXbatt <= (255 / battery_voltage) * monitored_voltage;
  
  if (millis() >= ledTime + 200) //200 = 3.3VCC, 500 = 5VCC
  {
    ledTime = millis();
    
    if (ledState >= !detect + HIGH)
    {
      ledState = LOW;
    }
    else
    {
      ledState = HIGH;
    }   
    digitalWrite(pin_LED, ledState);
  }
//  Serial.println(payload.RXbatt); //print value ​​on a serial monitor
}

//************************************************************************************************************************************************************************
//when RX is switched on and TX is switched off, or after the loss of RF data = LED RX flash at a interval of 0.1s. Normal mode = LED RX is lit **************************
//************************************************************************************************************************************************************************
void RFoff_check()
{
  if (millis() >= ledTime + 30) //30 = 3.3VCC, 100 = 5VCC
  {
    ledTime = millis();
    
    if (ledState)
    {
      ledState = LOW;
    }
    else
    {
      ledState = HIGH;
    }   
    digitalWrite(pin_LED, ledState);
  }
}
   