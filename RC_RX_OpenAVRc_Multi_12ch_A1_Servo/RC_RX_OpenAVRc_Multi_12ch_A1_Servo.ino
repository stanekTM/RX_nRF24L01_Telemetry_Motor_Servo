
//********************************************************************************************
//Communication nRF24L01P "Stanek". Fixed RF channel, fixed address.                         *
//Support for OpenAVRc transmitter https://github.com/stanekTM/OpenAVRc_Dev                  *
//and for the Multiprotocol TX modul https://github.com/stanekTM/DIY-Multiprotocol-TX-Module *
//********************************************************************************************

#include <RF24.h>         //https://github.com/nRF24/RF24 v1.3.9
//#include <printf.h>       //print the radio debug info
#include <DigitalIO.h>    //https://github.com/greiman/DigitalIO
#include <Servo.h>        //Arduino standard library


//setting RF channels address (5 bytes number or character)
const byte address[] = "jirka";

//RF communication channel settings (0-125, 2.4Ghz + 76 = 2.476Ghz)
#define radio_channel 76

//LED alarm battery voltage setting
#define battery_voltage   4.2
#define monitored_voltage 3.35

//PPM settings
#define servoMid     1500
#define servoMin     1000
#define servoMax     2000

//free pins
//pin                0
//pin                1
//pin                A6

//pins for servos
#define pin_servo1   2
#define pin_servo2   3
#define pin_servo3   4
#define pin_servo4   5
#define pin_servo5   6
#define pin_servo6   7
#define pin_servo7   8
#define pin_servo8   9
#define pin_servo9   10
#define pin_servo10  11 //MOSI
#define pin_servo11  12 //MISO
#define pin_servo12  13 //SCK

//LED RX battery and RF on/off
#define pin_LED      A5 

//input RX battery
#define pin_RXbatt   A7

//pins for nRF24L01
#define pin_CE       A0 
#define pin_CSN      A1 

//software SPI http://tmrh20.github.io/RF24/Arduino.html
//----- SCK     16 - A2
//----- MOSI    17 - A3
//----- MISO    18 - A4

//setting of CE and CSN pins
RF24 radio(pin_CE, pin_CSN);

//************************************************************************************************************************************************************************
//this structure defines the received data in bytes (structure size max. 32 bytes) ***************************************************************************************
//************************************************************************************************************************************************************************
struct packet
{
  unsigned int ch1;
  unsigned int ch2;
  unsigned int ch3;
  unsigned int ch4;
  unsigned int ch5;
  unsigned int ch6;
  unsigned int ch7;
  unsigned int ch8;
  unsigned int ch9;
  unsigned int ch10;
  unsigned int ch11;
  unsigned int ch12;
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
  rc_data.ch1  = servoMid;
  rc_data.ch2  = servoMid;
  rc_data.ch3  = servoMid;     
  rc_data.ch4  = servoMid;
  rc_data.ch5  = servoMid;
  rc_data.ch6  = servoMid;
  rc_data.ch7  = servoMid;
  rc_data.ch8  = servoMid;
  rc_data.ch9  = servoMid;
  rc_data.ch10 = servoMid;
  rc_data.ch11 = servoMid;
  rc_data.ch12 = servoMid;
}

//************************************************************************************************************************************************************************
//create servo object ****************************************************************************************************************************************************
//************************************************************************************************************************************************************************
Servo servo1, servo2, servo3, servo4, servo5, servo6, servo7, servo8, servo9, servo10, servo11, servo12;

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
  servo9.attach(pin_servo9);
  servo10.attach(pin_servo10);
  servo11.attach(pin_servo11);
  servo12.attach(pin_servo12);
}

int ch1_value = 0, ch2_value = 0, ch3_value = 0, ch4_value = 0, ch5_value = 0, ch6_value = 0,
    ch7_value = 0, ch8_value = 0, ch9_value = 0, ch10_value = 0, ch11_value = 0, ch12_value = 0;

void outputServo()
{
  ch1_value  = map(rc_data.ch1, servoMin, servoMax, servoMin, servoMax);
  ch2_value  = map(rc_data.ch2, servoMin, servoMax, servoMin, servoMax);
  ch3_value  = map(rc_data.ch3, servoMin, servoMax, servoMin, servoMax); 
  ch4_value  = map(rc_data.ch4, servoMin, servoMax, servoMin, servoMax);
  ch5_value  = map(rc_data.ch5, servoMin, servoMax, servoMin, servoMax);
  ch6_value  = map(rc_data.ch6, servoMin, servoMax, servoMin, servoMax);
  ch7_value  = map(rc_data.ch7, servoMin, servoMax, servoMin, servoMax);
  ch8_value  = map(rc_data.ch8, servoMin, servoMax, servoMin, servoMax);
  ch9_value  = map(rc_data.ch9, servoMin, servoMax, servoMin, servoMax);
  ch10_value = map(rc_data.ch10, servoMin, servoMax, servoMin, servoMax);
  ch11_value = map(rc_data.ch11, servoMin, servoMax, servoMin, servoMax);
  ch12_value = map(rc_data.ch12, servoMin, servoMax, servoMin, servoMax);
  
  servo1.writeMicroseconds(ch1_value);   
  servo2.writeMicroseconds(ch2_value);
  servo3.writeMicroseconds(ch3_value);   
  servo4.writeMicroseconds(ch4_value);
  servo5.writeMicroseconds(ch5_value);
  servo6.writeMicroseconds(ch6_value);
  servo7.writeMicroseconds(ch7_value);
  servo8.writeMicroseconds(ch8_value);
  servo9.writeMicroseconds(ch9_value);
  servo10.writeMicroseconds(ch10_value);
  servo11.writeMicroseconds(ch11_value);
  servo12.writeMicroseconds(ch12_value);

//  Serial.println(rc_data.ch1); //print value ​​on a serial monitor 
}

//************************************************************************************************************************************************************************
//initial main settings **************************************************************************************************************************************************
//************************************************************************************************************************************************************************
uint8_t invert_address = ~address[5]; //Invert bits for writing so that telemetry packets have a different address

void setup()
{
//  Serial.begin(9600); //print value ​​on a serial monitor
//  printf_begin();     //print the radio debug info

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
//reading adc RX battery. After receiving RF data, the monitored RX battery is activated *********************************************************************************
//when RX battery_voltage < monitored_voltage = LED alarm RX flash at a interval of 0.5s. Battery OK = LED RX is lit *****************************************************
//************************************************************************************************************************************************************************
unsigned long ledTime = 0, adcTime = 0;
int ledState, detect;

void RX_batt_check()
{
  if (millis() >= adcTime + 1000) //delay adc reading RX battery
  {
    adcTime = millis();
    
    payload.RXbatt = map(analogRead(pin_RXbatt), 0, 1023, 0, 255);
  }
  
  detect = payload.RXbatt <= (255 / battery_voltage) * monitored_voltage;
    
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
//  Serial.println(payload.RXbatt); //print value ​​on a serial monitor
}

//************************************************************************************************************************************************************************
//when RX is switched on and TX is switched off, or after the loss of RF data = LED RX flash at a interval of 0.1s. Normal mode = LED RX is lit **************************
//************************************************************************************************************************************************************************
void RFoff_check()
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
   
