
//*********************************************************************************************************************
//Communication nRF24L01P. Fixed RF channel, fixed address.
//Support transmitter with OLED screen and telemetry https://github.com/stanekTM/TX_nRF24_2ch_OLED
//and transmitter with LED indication and telemetry https://github.com/stanekTM/TX_nRF24_5ch_LED
//*********************************************************************************************************************

#include <RF24.h>         //https://github.com/nRF24/RF24
//#include <printf.h>       //print the radio debug info
#include <DigitalIO.h>    //https://github.com/greiman/DigitalIO
#include <Servo.h>        //Arduino standard library


//setting RF channels address (5 bytes number or character)
const byte address[] = "jirka";

//RF communication channel settings (0-125, 2.4Ghz + 76 = 2.476Ghz)
#define RADIO_CHANNEL  76

//LED alarm battery voltage setting
#define BATTERY_VOLTAGE    4.2
#define MONITORED_VOLTAGE  3.35

//setting the control range value
#define MIN_CONTROL_VAL  1000
#define MID_CONTROL_VAL  1500
#define MAX_CONTROL_VAL  2000

//free pins
//pin                   0
//pin                   1
//pin                   2
//pin                   3
//pin                   4
//pin                   5
//pin                   6
//pin                   7
//pin                   8
//pin                   A6

//pins for servos
#define PIN_SERVO_1     9
#define PIN_SERVO_2     10
#define PIN_SERVO_3     11 //MOSI
#define PIN_SERVO_4     12 //MISO
#define PIN_SERVO_5     13 //SCK

//LED battery and RF on/off
#define PIN_LED         A5

//input battery
#define PIN_BATTERY     A7

//pins for nRF24L01
#define PIN_CE          A0
#define PIN_CSN         A1

//software SPI https://nrf24.github.io/RF24/md_docs_arduino.html
//----- SCK        16 - A2
//----- MOSI       17 - A3
//----- MISO       18 - A4

//setting of CE and CSN pins
RF24 radio(PIN_CE, PIN_CSN);

//*********************************************************************************************************************
//this structure defines the received data in bytes (structure size max. 32 bytes) ************************************
//*********************************************************************************************************************
struct rc_packet_size
{
  unsigned int ch_servo1 = MID_CONTROL_VAL;
  unsigned int ch_servo2 = MID_CONTROL_VAL;
  unsigned int ch_servo3 = MID_CONTROL_VAL;
  unsigned int ch_servo4 = MID_CONTROL_VAL;
  unsigned int ch_servo5 = MID_CONTROL_VAL;
};
rc_packet_size rc_packet;

//*********************************************************************************************************************
//this struct defines data, which are embedded inside the ACK payload *************************************************
//*********************************************************************************************************************
struct telemetry_packet_size
{
  byte rssi;     //not used yet
  float batt_A1;
  float batt_A2; //not used yet
};
telemetry_packet_size telemetry_packet;

//*********************************************************************************************************************
//fail safe, settings 1000-2000 (MIN_CONTROL_VAL = 1000, MID_CONTROL_VAL = 1500, MAX_CONTROL_VAL = 2000) **************
//*********************************************************************************************************************
void fail_safe()
{
  rc_packet.ch_servo1 = MID_CONTROL_VAL;
  rc_packet.ch_servo2 = MID_CONTROL_VAL;
  rc_packet.ch_servo3 = MID_CONTROL_VAL;
  rc_packet.ch_servo4 = MID_CONTROL_VAL;
  rc_packet.ch_servo5 = MID_CONTROL_VAL;
}

//*********************************************************************************************************************
//attach servo pins ***************************************************************************************************
//*********************************************************************************************************************
Servo servo1, servo2, servo3, servo4, servo5;

void attach_servo_pin()
{
  servo1.attach(PIN_SERVO_1);
  servo2.attach(PIN_SERVO_2);
  servo3.attach(PIN_SERVO_3);
  servo4.attach(PIN_SERVO_4);
  servo5.attach(PIN_SERVO_5);
}

//*********************************************************************************************************************
//servo outputs *******************************************************************************************************
//*********************************************************************************************************************
int value_servo1 = 0, value_servo2 = 0, value_servo3 = 0, value_servo4 = 0, value_servo5 = 0;

void output_servo()
{
  value_servo1 = map(rc_packet.ch_servo1, MIN_CONTROL_VAL, MAX_CONTROL_VAL, MIN_CONTROL_VAL, MAX_CONTROL_VAL);
  value_servo2 = map(rc_packet.ch_servo2, MIN_CONTROL_VAL, MAX_CONTROL_VAL, MIN_CONTROL_VAL, MAX_CONTROL_VAL);
  value_servo3 = map(rc_packet.ch_servo3, MIN_CONTROL_VAL, MAX_CONTROL_VAL, MIN_CONTROL_VAL, MAX_CONTROL_VAL);
  value_servo4 = map(rc_packet.ch_servo4, MIN_CONTROL_VAL, MAX_CONTROL_VAL, MIN_CONTROL_VAL, MAX_CONTROL_VAL);
  value_servo5 = map(rc_packet.ch_servo5, MIN_CONTROL_VAL, MAX_CONTROL_VAL, MIN_CONTROL_VAL, MAX_CONTROL_VAL);
  
  servo1.writeMicroseconds(value_servo1);
  servo2.writeMicroseconds(value_servo2);
  servo3.writeMicroseconds(value_servo3);
  servo4.writeMicroseconds(value_servo4);
  servo5.writeMicroseconds(value_servo5);
  
  //Serial.println(rc_packet.ch_servo1);
}

//*********************************************************************************************************************
//initial main settings ***********************************************************************************************
//*********************************************************************************************************************
//const byte invert_address = ~address[5]; //invert bits for writing so that telemetry packets have a different address

void setup()
{
  //Serial.begin(9600); //print value ​​on a serial monitor
  //printf_begin();     //print the radio debug info
  
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_BATTERY, INPUT);
  
  fail_safe();
  attach_servo_pin();
  
  //define the radio communication
  radio.begin();
  radio.setAutoAck(true);
  radio.enableAckPayload();
  radio.enableDynamicPayloads();
  radio.setRetries(5, 5);
  radio.setChannel(RADIO_CHANNEL);
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_MIN); //RF24_PA_MIN (-18dBm), RF24_PA_LOW (-12dBm), RF24_PA_HIGH (-6dbm), RF24_PA_MAX (0dBm)
  //radio.openWritingPipe(invert_address);
  radio.openReadingPipe(1, address);
  radio.startListening();
}

//*********************************************************************************************************************
//program loop ********************************************************************************************************
//*********************************************************************************************************************
void loop()
{
  output_servo();
  fail_safe_time();
  send_and_receive_data();
  
  //Serial.println("Radio details *****************");
  //radio.printDetails(); //print the radio debug info
}

//*********************************************************************************************************************
//get time after losing RF data or turning off the TX, reset data and the LED flashing ********************************
//*********************************************************************************************************************
unsigned long fs_time = 0;

void fail_safe_time()
{
  if (millis() - fs_time > 1000) //after 1 second
  {
    fail_safe(); 
    RF_off_check();
  }
}

//*********************************************************************************************************************
//send and receive data ***********************************************************************************************
//*********************************************************************************************************************
void send_and_receive_data()
{
  if (radio.available())
  {
    radio.writeAckPayload(1, &telemetry_packet, sizeof(telemetry_packet_size));
   
    radio.read(&rc_packet, sizeof(rc_packet_size));
    
    battery_check();
    fs_time = millis();
  }
}

//*********************************************************************************************************************
//reading adc battery. After receiving RF data, the monitored battery is activated. Battery OK = LED is lit ***********
//When BATTERY_VOLTAGE < MONITORED_VOLTAGE = LED alarm flash at a interval of 0.5s ************************************
//*********************************************************************************************************************
unsigned long adc_time = 0, led_time = 0;
bool low_batt_detect = 0, previous_state_batt, batt_led_state = 1, RF_led_state;

void battery_check()
{
  if (millis() - adc_time > 1000) //delay adc reading RX battery
  {
    adc_time = millis();
    
    telemetry_packet.batt_A1 = analogRead(PIN_BATTERY) * (BATTERY_VOLTAGE / 1023);
    
    low_batt_detect = telemetry_packet.batt_A1 <= MONITORED_VOLTAGE;
  }
  
  digitalWrite(PIN_LED, batt_led_state);
  
  if (low_batt_detect)
  {
    previous_state_batt = 1;
    
    if (millis() - led_time > 500)
    {
      led_time = millis();
      
      batt_led_state = !batt_led_state;
    }
  }
  low_batt_detect = previous_state_batt;
  
  //Serial.println(low_batt_detect);
}

//*********************************************************************************************************************
//when RX is switched on and TX is switched off, or after the loss of RF data = LED flash at a interval of 0.1s *******
//Normal mode = LED is lit ********************************************************************************************
//*********************************************************************************************************************
void RF_off_check()
{
  digitalWrite(PIN_LED, RF_led_state);
  
  if (millis() - led_time > 100)
  {
    led_time = millis();
    
    RF_led_state = !RF_led_state;
  }
}
 
