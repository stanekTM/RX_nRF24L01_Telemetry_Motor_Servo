
//************************************************************************************************
//Communication nRF24L01P. Fixed RF channel, fixed address.                                      *
//Support for TX Telemetry LCD transmitter https://github.com/stanekTM/TX_nRF24L01_Telemetry_LCD *
//and for the TX Telemetry LED transmitter https://github.com/stanekTM/TX_nRF24L01_Telemetry_LED *
//************************************************************************************************

#include <RF24.h>         //https://github.com/nRF24/RF24 v1.3.9
//#include <printf.h>       //print the radio debug info
#include <DigitalIO.h>    //https://github.com/greiman/DigitalIO
#include <Servo.h>        //Arduino standard library
#include "PWMFrequency.h" //used locally https://github.com/TheDIYGuy999/PWMFrequency


//setting RF channels address (5 bytes number or character)
const byte address[] = "jirka";

//RF communication channel settings (0-125, 2.4Ghz + 76 = 2.476Ghz)
#define RADIO_CHANNEL  76

//settings PWM motorA (pin D5 or D6 are paired on timer0, functions delay(), millis(), micros() and delayMicroseconds())
//1024 = 61Hz, 256 = 244Hz, 64 = 976Hz(default), 8 = 7812Hz
#define PWM_MOTOR_A  64

//settings PWM motorB (pin D3 or D11)
//1024 = 30Hz, 256 = 122Hz, 128 = 244Hz, 64 = 488Hz(default), 32 = 976Hz, 8 = 3906Hz
#define PWM_MOTOR_B  256

//setting the reaction of the motor to be rotated after the lever has been moved (0-255)
#define ACCELERATE_MOTOR_A  90
#define ACCELERATE_MOTOR_B  40

//setting the maximum engine power. Suitable for TX transmitters without endpoint setting (0-255)
#define MAXIMUM_MOTOR_A  255
#define MAXIMUM_MOTOR_B  255

//brake setting, adjustment (0-255), no brake 0, max brake 255
#define BRAKE_MOTOR_A  255
#define BRAKE_MOTOR_B  0

//LED alarm battery voltage setting
#define BATTERY_VOLTAGE    4.2
#define MONITORED_VOLTAGE  3.35

//setting the dead zone of poor quality joysticks TX for the motor controller
#define DEAD_ZONE  15

//setting the control range value
#define min_control_val 1000
#define mid_control_val 1500
#define max_control_val 2000

//free pins
//pin                   0
//pin                   1
//pin                   2
//pin                   4
//pin                   7
//pin                   8
//pin                   9
//pin                   A6

//pins for servos
#define pin_servo1      10
#define pin_servo2      12 //MISO
#define pin_servo3      13 //SCK
 
//pwm pins for motor
#define pin_pwm1_motorA 5
#define pin_pwm2_motorA 6
#define pin_pwm3_motorB 3
#define pin_pwm4_motorB 11 //MOSI

//LED RX battery and RF on/off
#define pin_LED         A5

//input RX battery
#define pin_RX_battery  A7

//pins for nRF24L01
#define pin_CE          A0
#define pin_CSN         A1

//software SPI http://tmrh20.github.io/RF24/Arduino.html
//----- SCK        16 - A2
//----- MOSI       17 - A3
//----- MISO       18 - A4

//setting of CE and CSN pins
RF24 radio(pin_CE, pin_CSN);

//************************************************************************************************************************************************************************
//this structure defines the received data in bytes (structure size max. 32 bytes) ***************************************************************************************
//************************************************************************************************************************************************************************
struct rc_packet_size
{
  unsigned int ch1; //motorA
  unsigned int ch2; //motorB
  unsigned int ch3;
  unsigned int ch4;
  unsigned int ch5;
};
rc_packet_size rc_packet; //create a variable with the above structure

//************************************************************************************************************************************************************************
//this struct defines data, which are embedded inside the ACK payload ****************************************************************************************************
//************************************************************************************************************************************************************************
struct telemetry_packet_size
{
  uint8_t rssi;     // not used yet
  float RX_batt_A1;
  float RX_batt_A2; // not used yet
};
telemetry_packet_size telemetry_packet;

//************************************************************************************************************************************************************************
//fail safe, settings 1000-2000 ​​(min_control_val = 1000, mid_control_val = 1500, max_control_val = 2000) *****************************************************************
//************************************************************************************************************************************************************************
void fail_safe()
{
  rc_packet.ch1  = mid_control_val; //motorA
  rc_packet.ch2  = mid_control_val; //motorB
  rc_packet.ch3  = mid_control_val;
  rc_packet.ch4  = mid_control_val;
  rc_packet.ch5  = mid_control_val;
}

//************************************************************************************************************************************************************************
//create servo object ****************************************************************************************************************************************************
//************************************************************************************************************************************************************************
Servo servo1, servo2, servo3;

void attachServoPins()
{
  servo1.attach(pin_servo1);
  servo2.attach(pin_servo2);
  servo3.attach(pin_servo3);
}

int value_servo1 = 0, value_servo2 = 0, value_servo3 = 0;

void outputServo()
{
  value_servo1 = map(rc_packet.ch3, min_control_val, max_control_val, min_control_val, max_control_val);
  value_servo2 = map(rc_packet.ch4, min_control_val, max_control_val, min_control_val, max_control_val);
  value_servo3 = map(rc_packet.ch5, min_control_val, max_control_val, min_control_val, max_control_val);
  
  servo1.writeMicroseconds(value_servo1);
  servo2.writeMicroseconds(value_servo2);
  servo3.writeMicroseconds(value_servo3);

//  Serial.println(rc_packet.ch3); //print value ​​on a serial monitor 
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
 * Pins 5, 6  are paired on timer0, functions delay(), millis(), micros() and delayMicroseconds()
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
 
//motorA PWM frequency pin D5 or pin D6
//1024 = 61Hz, 256 = 244Hz, 64 = 976Hz(default), 8 = 7812Hz 
  setPWMPrescaler(pin_pwm1_motorA, PWM_MOTOR_A);

//motorB PWM frequency pin D3 or pin D11
//1024 = 30Hz, 256 = 122Hz, 128 = 244Hz, 64 = 488Hz(default), 32 = 976Hz, 8 = 3906Hz  
  setPWMPrescaler(pin_pwm3_motorB, PWM_MOTOR_B); 

//motorA --------------------------------------------------------------------------------------

  if (rc_packet.ch1 < mid_control_val - DEAD_ZONE)
  {
    value_motorA = map(rc_packet.ch1, mid_control_val - DEAD_ZONE, min_control_val, ACCELERATE_MOTOR_A, MAXIMUM_MOTOR_A);
    analogWrite(pin_pwm1_motorA, value_motorA); 
    digitalWrite(pin_pwm2_motorA, LOW);
  }
  else if (rc_packet.ch1 > mid_control_val + DEAD_ZONE)
  {
    value_motorA = map(rc_packet.ch1, mid_control_val + DEAD_ZONE, max_control_val, ACCELERATE_MOTOR_A, MAXIMUM_MOTOR_A);
    analogWrite(pin_pwm2_motorA, value_motorA); 
    digitalWrite(pin_pwm1_motorA, LOW);
  }
  else
  {
    analogWrite(pin_pwm1_motorA, BRAKE_MOTOR_A);
    analogWrite(pin_pwm2_motorA, BRAKE_MOTOR_A);
  }

//  Serial.println(rc_packet.ch1); //print value ​​on a serial monitor
  
//motorB --------------------------------------------------------------------------------------

  if (rc_packet.ch2 < mid_control_val - DEAD_ZONE)
  {
    value_motorB = map(rc_packet.ch2, mid_control_val - DEAD_ZONE, min_control_val, ACCELERATE_MOTOR_B, MAXIMUM_MOTOR_B); 
    analogWrite(pin_pwm3_motorB, value_motorB); 
    digitalWrite(pin_pwm4_motorB, LOW);
  }
  else if (rc_packet.ch2 > mid_control_val + DEAD_ZONE)
  {
    value_motorB = map(rc_packet.ch2, mid_control_val + DEAD_ZONE, max_control_val, ACCELERATE_MOTOR_B, MAXIMUM_MOTOR_B); 
    analogWrite(pin_pwm4_motorB, value_motorB); 
    digitalWrite(pin_pwm3_motorB, LOW);
  }
  else
  {
    analogWrite(pin_pwm3_motorB, BRAKE_MOTOR_B);
    analogWrite(pin_pwm4_motorB, BRAKE_MOTOR_B);
  }
}

//************************************************************************************************************************************************************************
//initial main settings **************************************************************************************************************************************************
//************************************************************************************************************************************************************************
uint8_t invert_address = ~address[5]; //invert bits for writing so that telemetry packets have a different address

void setup()
{
//  Serial.begin(9600); //print value ​​on a serial monitor
//  printf_begin();     //print the radio debug info

  pinMode(pin_pwm1_motorA, OUTPUT);
  pinMode(pin_pwm2_motorA, OUTPUT);
  pinMode(pin_pwm3_motorB, OUTPUT);
  pinMode(pin_pwm4_motorB, OUTPUT);
  
  pinMode(pin_LED, OUTPUT);
  pinMode(pin_RX_battery, INPUT);
  
  fail_safe();
  attachServoPins();

  //define the radio communication
  radio.begin();  
  radio.setAutoAck(true);          //ensure autoACK is enabled (default true)
  radio.enableAckPayload();        //enable Ack dynamic payloads. This only works on pipes 0&1 by default
  radio.enableDynamicPayloads();   //enable dynamic payloads on all pipes
  
  radio.setRetries(5, 5);          //set the number and delay of retries on failed submit (max. 15 x 250us delay (blocking !), max. 15 retries)
  
  radio.setChannel(RADIO_CHANNEL); //which RF channel to communicate on (0-125, 2.4Ghz + 76 = 2.476Ghz)
  radio.setDataRate(RF24_250KBPS); //RF24_250KBPS (fails for units without +), RF24_1MBPS, RF24_2MBPS
  radio.setPALevel(RF24_PA_MIN);   //RF24_PA_MIN (-18dBm), RF24_PA_LOW (-12dBm), RF24_PA_HIGH (-6dbm), RF24_PA_MAX (0dBm) 

  radio.openWritingPipe(invert_address); //open the writing pipe0 (RX_ADDR_P0 + TX_ADDR)
  radio.openReadingPipe(1, address);     //open the reading pipe1 (RX_ADDR_P1) and then call "startListening"
  
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
 
}

//************************************************************************************************************************************************************************
//get time after losing RF data or turning off the TX, reset data and the LED flashing ***********************************************************************************
//************************************************************************************************************************************************************************
unsigned long lastRxTime = 0;

void receive_time()
{
  if(millis() >= lastRxTime + 1000) //1s
  {
    fail_safe();
    RF_off_check();
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
    radio.writeAckPayload(pipeNo, &telemetry_packet, sizeof(telemetry_packet_size));
   
    radio.read(&rc_packet, sizeof(rc_packet_size));
    
    lastRxTime = millis(); //at this moment we have received the data
    RX_batt_check();
  } 
}

//************************************************************************************************************************************************************************
//reading adc RX battery. After receiving RF data, the monitored RX battery is activated *********************************************************************************
//when RX BATTERY_VOLTAGE < MONITORED_VOLTAGE = LED alarm RX flash at a interval of 0.5s. Battery OK = LED RX is lit *****************************************************
//************************************************************************************************************************************************************************
unsigned long ledTime = 0, adcTime = 0;
int ledState, detect;

void RX_batt_check()
{
  if (millis() >= adcTime + 1000) //delay adc reading RX battery
  {
    adcTime = millis();

    telemetry_packet.RX_batt_A1 = analogRead(pin_RX_battery) * (BATTERY_VOLTAGE / 1023);
  }

  detect = telemetry_packet.RX_batt_A1 <= MONITORED_VOLTAGE;
  
  if (millis() >= ledTime + 500)
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
//  Serial.println(telemetry_packet.RX_batt_A1); //print value ​​on a serial monitor
}

//************************************************************************************************************************************************************************
//when RX is switched on and TX is switched off, or after the loss of RF data = LED RX flash at a interval of 0.1s. Normal mode = LED RX is lit **************************
//************************************************************************************************************************************************************************
void RF_off_check()
{
  if (millis() >= ledTime + 100)
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
 
