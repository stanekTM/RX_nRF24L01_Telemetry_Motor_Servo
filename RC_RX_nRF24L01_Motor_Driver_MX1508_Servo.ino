
#include <SPI.h>          //https://github.com/arduino/ArduinoCore-avr/tree/master/libraries/SPI
#include <nRF24L01.h>     //https://github.com/nRF24/RF24
#include <RF24.h>         //https://github.com/nRF24/RF24
#include "PWMFrequency.h" //https://github.com/TheDIYGuy999/PWMFrequency
#include "ServoTimer2.h"  //https://github.com/nabontra/ServoTimer2 

RF24 radio(8, A0); //set CE and CSN pins

unsigned long lastReceiveTime = 0;

const byte addresses[][6] = {"tx001", "rx002"};

boolean buttonState = 0; //test telemetry

//**************************************************************************************************************************
//structure size max 32 bytes **********************************************************************************************
//**************************************************************************************************************************
struct rx_data
{
  byte ch1;
  byte ch2;
  byte ch3;
  byte ch4;
  byte ch5;
  byte ch6;
  byte ch7;
  byte ch8;
};

rx_data rc_data; //create a variable with the above structure

//**************************************************************************************************************************
//we will create variables with an initial integer *************************************************************************
//**************************************************************************************************************************
int ch1_value = 0;
int ch2_value = 0;
int ch3_value = 0;
int ch4_value = 0;
int ch5_value = 0;
int ch6_value = 0;
int motA_value = 0;
int motB_value = 0;

//**************************************************************************************************************************
//reset values ​​(min = 0, mid = 127, max = 255) *****************************************************************************
//**************************************************************************************************************************
void resetData()
{
  rc_data.ch1 = 127;     
  rc_data.ch2 = 127;
  rc_data.ch3 = 127;
  rc_data.ch4 = 127;
  rc_data.ch5 = 0;
  rc_data.ch6 = 0;
  rc_data.ch7 = 127;
  rc_data.ch8 = 127;
}

//**************************************************************************************************************************
//create servo object ******************************************************************************************************
//**************************************************************************************************************************
ServoTimer2 servo1;
ServoTimer2 servo2;
ServoTimer2 servo3;
ServoTimer2 servo4;
ServoTimer2 servo5;
ServoTimer2 servo6;

void attachServoPins()
{
  servo1.attach(2);
  servo2.attach(3);
  servo3.attach(4);
  servo4.attach(7);
  servo5.attach(A4);
  servo6.attach(A5);
}

void outputServo()
{
  servo1.write(ch1_value);   
  servo2.write(ch2_value); 
  servo3.write(ch3_value);
  servo4.write(ch4_value);
  servo5.write(ch5_value);
  servo6.write(ch6_value);

  ch1_value = map(rc_data.ch1,0,255,1000,2000); //linear
  ch2_value = map(rc_data.ch2,0,255,1000,2000);
  ch3_value = map(rc_data.ch3,0,255,1000,2000);
  ch4_value = map(rc_data.ch4,0,255,1000,2000);
  ch5_value = map(rc_data.ch5,0,1,1000,2000);
  ch6_value = map(rc_data.ch6,0,1,1000,2000);
}

//**************************************************************************************************************************
//frequencies and motors control *******************************************************************************************
//**************************************************************************************************************************
void outputPWM()
{  
/*
 * The base frequency for pins 3, 9, 10, 11 is 31250Hz.
 * The base frequency for pins 5, 6 is 62500Hz.
 * 
 * The divisors available on pins 5, 6, 9, 10 are: 1, 8, 64, 256, and 1024.
 * The divisors available on pins 3, 11       are: 1, 8, 32, 64, 128, 256, and 1024.
 * 
 * Pins 5, 6  are paired on timer0
 * Pins 9, 10 are paired on timer1
 * Pins 3, 11 are paired on timer2
 *  
 * PWM frequency (default)
 * D3  //pwm 488Hz, timer2, 8-bit
 * D11 //pwm 488Hz, timer2, 8-bit, SPI MOSI
   
 * D5  //pwm 976Hz, timer0, 8-bit
 * D6  //pwm 976Hz, timer0, 8-bit
 *
 * D9  //pwm 488Hz, timer1, 16-bit
 * D10 //pwm 488Hz, timer1, 16-bit     
*/
 
//PWM frequency pin D5 or pin D6:  1024 = 61Hz, 256 = 244Hz, 64 = 976Hz(default)
//MotorA (pin D5 or pin D6, prescaler 64)  
  setPWMPrescaler(5, 64);
  
//PWM frequency pin D9 or pin D10: 1024 = 30Hz, 256 = 122Hz, 64 = 488Hz(default), 8 = 3906Hz
//MotorB (pin D9 or pin D10, prescaler 8)  
  setPWMPrescaler(9, 8);

//MotorA ------------------------------------------------------------------------------------ 

  if (rc_data.ch7 < 125)
  {
    motA_value = map(rc_data.ch7, 125, 0, 0, 255);
    analogWrite(5, motA_value); 
    digitalWrite(6, LOW);
  }
  else if (rc_data.ch7 > 129)
  {
    motA_value = map(rc_data.ch7, 129, 255, 0, 255);
    analogWrite(6, motA_value); 
    digitalWrite(5, LOW);
  }
  else
  {
    digitalWrite(5, LOW); //"HIGH" brake, "LOW" no brake
    digitalWrite(6, LOW); //"HIGH" brake, "LOW" no brake
//    analogWrite(5, motA_value = 127); //adjustable brake (0-255)
//    analogWrite(6, motA_value = 127); //adjustable brake (0-255)
  }
  
//MotorB ------------------------------------------------------------------------------------

  if (rc_data.ch8 < 125)
  {
    motB_value = map(rc_data.ch8, 125, 0, 0, 255); 
    analogWrite(9, motB_value); 
    digitalWrite(10, LOW);
  }
  else if (rc_data.ch8 > 129)
  {
    motB_value = map(rc_data.ch8, 129, 255, 0, 255); 
    analogWrite(10, motB_value); 
    digitalWrite(9, LOW);
  }
  else
  {
//    digitalWrite(9, HIGH);  //"HIGH" brake, "LOW" no brake
//    digitalWrite(10, HIGH); //"HIGH" brake, "LOW" no brake
    analogWrite(9,  motB_value = 127);  //adjustable brake (0-255)
    analogWrite(10, motB_value = 127);  //adjustable brake (0-255)
  }
}

//**************************************************************************************************************************
//initial main settings ****************************************************************************************************
//**************************************************************************************************************************
void setup()
{
  Serial.begin(9600);

  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(A1, OUTPUT); //led RF
  pinMode(A3, INPUT_PULLUP); //test telemetry
  
  resetData(); //reset each channel value

  //define the radio communication
  radio.begin();
  
  radio.setAutoAck(1);           //ensure autoACK is enabled
  radio.enableAckPayload();      //allow optional ack payloads
  radio.enableDynamicPayloads();
  radio.setRetries(5, 5);                  // 5x250us delay (blocking!!), max. 5 retries
  
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_LOW); //set power amplifier(PA): RF24_PA_MIN, RF24_PA_LOW, RF24_PA_HIGH and RF24_PA_MAX  

  radio.openWritingPipe(addresses[0]);    //tx001
  radio.openReadingPipe(1, addresses[1]); //rx002
  
  radio.startListening(); //set the module as receiver

  attachServoPins();
}

//**************************************************************************************************************************
//program loop *************************************************************************************************************
//**************************************************************************************************************************
void loop()
{
  receive_the_data();

  //check whether we keep receving data, or we have a connection between the two modules
  unsigned long now = millis();
  if (now - lastReceiveTime > 1000) //if the signal is lost, reset the data after 1 second
  {
    resetData();            //if connection is lost, reset the data
    digitalWrite(A1, HIGH); //led RF off signal
  }

  outputServo();
  outputPWM();

//  Serial.println(motB_value); //print value ​​on a serial monitor  
} //end program loop

//**************************************************************************************************************************
//reading data at a specific time ******************************************************************************************
//**************************************************************************************************************************
void receive_the_data()
{
  byte pipeNo;
  
  if (radio.available(&pipeNo))  //check whether there is data to be received //while
  {
    radio.writeAckPayload(pipeNo, &buttonState, sizeof(buttonState)); // prepare the ACK payload
    buttonState = digitalRead(A3); //test telemetry
    
    radio.read(&rc_data, sizeof(rx_data)); //read the whole data and store it into the 'data' structure
    lastReceiveTime = millis();            //at this moment we have received the data
    digitalWrite(A1, LOW);                 //led RF on signal
  } 
}  
 
