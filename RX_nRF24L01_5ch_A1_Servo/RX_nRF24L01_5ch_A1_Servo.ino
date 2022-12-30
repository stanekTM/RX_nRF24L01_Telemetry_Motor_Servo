
//************************************************************************************************************************************************************************
//Communication nRF24L01P. Fixed RF channel, fixed address.
//Support for TX Telemetry LCD transmitter https://github.com/stanekTM/TX_4ch_nRF24L01_Telemetry_LCD
//and for the TX Telemetry LED transmitter https://github.com/stanekTM/TX_5ch_nRF24L01_Telemetry_LED
//************************************************************************************************************************************************************************

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

//LED RX battery and RF on/off
#define PIN_LED         A5

//input RX battery
#define PIN_RX_BATTERY  A7

//pins for nRF24L01
#define PIN_CE          A0
#define PIN_CSN         A1

//software SPI https://nrf24.github.io/RF24/md_docs_arduino.html
//----- SCK        16 - A2
//----- MOSI       17 - A3
//----- MISO       18 - A4

//setting of CE and CSN pins
RF24 radio(PIN_CE, PIN_CSN);

//************************************************************************************************************************************************************************
//this structure defines the received data in bytes (structure size max. 32 bytes) ***************************************************************************************
//************************************************************************************************************************************************************************
struct rc_packet_size
{
  unsigned int ch_servo1 = MID_CONTROL_VAL;
  unsigned int ch_servo2 = MID_CONTROL_VAL;
  unsigned int ch_servo3 = MID_CONTROL_VAL;
  unsigned int ch_servo4 = MID_CONTROL_VAL;
  unsigned int ch_servo5 = MID_CONTROL_VAL;
};
rc_packet_size rc_packet; //create a variable with the above structure

//************************************************************************************************************************************************************************
//this struct defines data, which are embedded inside the ACK payload ****************************************************************************************************
//************************************************************************************************************************************************************************
struct telemetry_packet_size
{
  byte rssi;        //not used yet
  float RX_batt_A1;
  float RX_batt_A2; //not used yet
};
telemetry_packet_size telemetry_packet;

//************************************************************************************************************************************************************************
//fail safe, settings 1000-2000 ​​(MIN_CONTROL_VAL = 1000, MID_CONTROL_VAL = 1500, MAX_CONTROL_VAL = 2000) *****************************************************************
//************************************************************************************************************************************************************************
void fail_safe()
{
  rc_packet.ch_servo1 = MID_CONTROL_VAL;
  rc_packet.ch_servo2 = MID_CONTROL_VAL;
  rc_packet.ch_servo3 = MID_CONTROL_VAL;
  rc_packet.ch_servo4 = MID_CONTROL_VAL;
  rc_packet.ch_servo5 = MID_CONTROL_VAL;
}

//************************************************************************************************************************************************************************
//attach servo pins ******************************************************************************************************************************************************
//************************************************************************************************************************************************************************
Servo servo1, servo2, servo3, servo4, servo5;

void attach_servo_pin()
{
  servo1.attach(PIN_SERVO_1);
  servo2.attach(PIN_SERVO_2);
  servo3.attach(PIN_SERVO_3);
  servo4.attach(PIN_SERVO_4);
  servo5.attach(PIN_SERVO_5);
}

//************************************************************************************************************************************************************************
//servo outputs **********************************************************************************************************************************************************
//************************************************************************************************************************************************************************
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
  
  //Serial.println(rc_packet.ch_servo1); //print value ​​on a serial monitor
}

//************************************************************************************************************************************************************************
//initial main settings **************************************************************************************************************************************************
//************************************************************************************************************************************************************************
//const byte invert_address = ~address[5]; //invert bits for writing so that telemetry packets have a different address

void setup()
{
  //Serial.begin(9600); //print value ​​on a serial monitor
  //printf_begin();     //print the radio debug info
  
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_RX_BATTERY, INPUT);
  
  fail_safe();
  attach_servo_pin();
  
  //define the radio communication
  radio.begin();  
  radio.setAutoAck(true);          //ensure autoACK is enabled (default true)
  radio.enableAckPayload();        //enable Ack dynamic payloads. This only works on pipes 0&1 by default
  radio.enableDynamicPayloads();   //enable dynamic payloads on all pipes
  radio.setRetries(5, 5);          //set the number and delay of retries on failed submit (max. 15 x 250us delay (blocking !), max. 15 retries)
  
  radio.setChannel(RADIO_CHANNEL); //which RF channel to communicate on (0-125, 2.4Ghz + 76 = 2.476Ghz)
  radio.setDataRate(RF24_250KBPS); //RF24_250KBPS (fails for units without +), RF24_1MBPS, RF24_2MBPS
  radio.setPALevel(RF24_PA_MIN);   //RF24_PA_MIN (-18dBm), RF24_PA_LOW (-12dBm), RF24_PA_HIGH (-6dbm), RF24_PA_MAX (0dBm)
  
  //radio.openWritingPipe(invert_address); //open the writing pipe0 (RX_ADDR_P0 + TX_ADDR)
  radio.openReadingPipe(1, address);     //open the reading pipe1 (RX_ADDR_P1) and then call "startListening"
  
  radio.startListening(); //set the module as receiver. Start listening on the pipes opened for reading
}

//************************************************************************************************************************************************************************
//program loop ***********************************************************************************************************************************************************
//************************************************************************************************************************************************************************
void loop()
{
  last_rx_time();
  send_and_receive_data();
  output_servo();
  
  //Serial.println("Radio details *****************");
  //radio.printDetails(); //print the radio debug info
}

//************************************************************************************************************************************************************************
//get time after losing RF data or turning off the TX, reset data and the LED flashing ***********************************************************************************
//************************************************************************************************************************************************************************
unsigned long rx_time = 0;

void last_rx_time()
{
  if (millis() - rx_time > 1000) //1s
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
  if (radio.available())
  {
    radio.writeAckPayload(1, &telemetry_packet, sizeof(telemetry_packet_size));
   
    radio.read(&rc_packet, sizeof(rc_packet_size));
    
    RX_batt_check();
    rx_time = millis(); //at this moment we have received the data
  }
}

//************************************************************************************************************************************************************************
//reading adc RX battery. After receiving RF data, the monitored RX battery is activated *********************************************************************************
//when RX BATTERY_VOLTAGE < MONITORED_VOLTAGE = LED alarm RX flash at a interval of 0.5s. Battery OK = LED RX is lit *****************************************************
//************************************************************************************************************************************************************************
unsigned long adc_time = 0, led_time = 0;
bool batt_detect, led_state;

void RX_batt_check()
{
  if (millis() - adc_time > 1000) //delay adc reading RX battery
  {
    adc_time = millis();
    
    telemetry_packet.RX_batt_A1 = analogRead(PIN_RX_BATTERY) * (BATTERY_VOLTAGE / 1023);
  }
  
  batt_detect = telemetry_packet.RX_batt_A1 <= MONITORED_VOLTAGE;
  
  if (millis() - led_time > 500)
  {
    led_time = millis();
    
    if (led_state >= !batt_detect + HIGH)
    {
      led_state = LOW;
    }
    else
    {
      led_state = HIGH;
    }
    digitalWrite(PIN_LED, led_state);
  }
  //Serial.println(telemetry_packet.RX_batt_A1); //print value ​​on a serial monitor
}

//************************************************************************************************************************************************************************
//when RX is switched on and TX is switched off, or after the loss of RF data = LED RX flash at a interval of 0.1s. Normal mode = LED RX is lit **************************
//************************************************************************************************************************************************************************
void RF_off_check()
{
  if (millis() - led_time > 100)
  {
    led_time = millis();
    
    if (led_state)
    {
      led_state = LOW;
    }
    else
    {
      led_state = HIGH;
    }
    digitalWrite(PIN_LED, led_state);
  }
}
 
