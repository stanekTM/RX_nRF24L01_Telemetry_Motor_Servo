
//*********************************************************************************************************************
//Communication nRF24L01P "Stanek". Fixed RF channel, fixed address.
//Support for OpenAVRc transmitter https://github.com/Ingwie/OpenAVRc_Dev
//and for the Multiprotocol TX modul https://github.com/stanekTM/DIY-Multiprotocol-TX-Module
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
//pin                   A6

//pins for servos
#define PIN_SERVO_1     2
#define PIN_SERVO_2     3
#define PIN_SERVO_3     4
#define PIN_SERVO_4     5
#define PIN_SERVO_5     6
#define PIN_SERVO_6     7
#define PIN_SERVO_7     8
#define PIN_SERVO_8     9
#define PIN_SERVO_9     10
#define PIN_SERVO_10    11 //MOSI
#define PIN_SERVO_11    12 //MISO
#define PIN_SERVO_12    13 //SCK

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
  unsigned int ch_servo1  = MID_CONTROL_VAL;
  unsigned int ch_servo2  = MID_CONTROL_VAL;
  unsigned int ch_servo3  = MID_CONTROL_VAL;
  unsigned int ch_servo4  = MID_CONTROL_VAL;
  unsigned int ch_servo5  = MID_CONTROL_VAL;
  unsigned int ch_servo6  = MID_CONTROL_VAL;
  unsigned int ch_servo7  = MID_CONTROL_VAL;
  unsigned int ch_servo8  = MID_CONTROL_VAL;
  unsigned int ch_servo9  = MID_CONTROL_VAL;
  unsigned int ch_servo10 = MID_CONTROL_VAL;
  unsigned int ch_servo11 = MID_CONTROL_VAL;
  unsigned int ch_servo12 = MID_CONTROL_VAL;
};
rc_packet_size rc_packet;

//*********************************************************************************************************************
//this struct defines data, which are embedded inside the ACK payload *************************************************
//*********************************************************************************************************************
struct telemetry_packet_size
{
  byte rssi;    //0-255 for OpenAVRc and OpenTX Multiprotocol telemetry
  byte batt_A1; //0-255 for OpenAVRc and OpenTX Multiprotocol telemetry
  byte batt_A2; //0-255 for OpenAVRc and OpenTX Multiprotocol telemetry (not used yet)
};
telemetry_packet_size telemetry_packet;

//*********************************************************************************************************************
//fail safe, settings 1000-2000 (MIN_CONTROL_VAL = 1000, MID_CONTROL_VAL = 1500, MAX_CONTROL_VAL = 2000) **************
//*********************************************************************************************************************
void fail_safe()
{
  rc_packet.ch_servo1  = MID_CONTROL_VAL;
  rc_packet.ch_servo2  = MID_CONTROL_VAL;
  rc_packet.ch_servo3  = MID_CONTROL_VAL;
  rc_packet.ch_servo4  = MID_CONTROL_VAL;
  rc_packet.ch_servo5  = MID_CONTROL_VAL;
  rc_packet.ch_servo6  = MID_CONTROL_VAL;
  rc_packet.ch_servo7  = MID_CONTROL_VAL;
  rc_packet.ch_servo8  = MID_CONTROL_VAL;
  rc_packet.ch_servo9  = MID_CONTROL_VAL;
  rc_packet.ch_servo10 = MID_CONTROL_VAL;
  rc_packet.ch_servo11 = MID_CONTROL_VAL;
  rc_packet.ch_servo12 = MID_CONTROL_VAL;
}

//*********************************************************************************************************************
//attach servo pins ***************************************************************************************************
//*********************************************************************************************************************
Servo servo1, servo2, servo3, servo4, servo5, servo6, servo7, servo8, servo9, servo10, servo11, servo12;

void attach_servo_pins()
{
  servo1.attach(PIN_SERVO_1);
  servo2.attach(PIN_SERVO_2);
  servo3.attach(PIN_SERVO_3);
  servo4.attach(PIN_SERVO_4);
  servo5.attach(PIN_SERVO_5);
  servo6.attach(PIN_SERVO_6);
  servo7.attach(PIN_SERVO_7);
  servo8.attach(PIN_SERVO_8);
  servo9.attach(PIN_SERVO_9);
  servo10.attach(PIN_SERVO_10);
  servo11.attach(PIN_SERVO_11);
  servo12.attach(PIN_SERVO_12);
}

//*********************************************************************************************************************
//servo outputs *******************************************************************************************************
//*********************************************************************************************************************
void output_servo()
{
  servo1.writeMicroseconds(rc_packet.ch_servo1);
  servo2.writeMicroseconds(rc_packet.ch_servo2);
  servo3.writeMicroseconds(rc_packet.ch_servo3);
  servo4.writeMicroseconds(rc_packet.ch_servo4);
  servo5.writeMicroseconds(rc_packet.ch_servo5);
  servo6.writeMicroseconds(rc_packet.ch_servo6);
  servo7.writeMicroseconds(rc_packet.ch_servo7);
  servo8.writeMicroseconds(rc_packet.ch_servo8);
  servo9.writeMicroseconds(rc_packet.ch_servo9);
  servo10.writeMicroseconds(rc_packet.ch_servo10);
  servo11.writeMicroseconds(rc_packet.ch_servo11);
  servo12.writeMicroseconds(rc_packet.ch_servo12);
  
  //Serial.println(rc_packet.ch_servo1);
}

//*********************************************************************************************************************
//initial main settings ***********************************************************************************************
//*********************************************************************************************************************
void setup()
{
  //Serial.begin(9600); //print value on a serial monitor
  //printf_begin();     //print the radio debug info
  
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_BATTERY, INPUT);
  
  fail_safe();
  attach_servo_pins();
  
  //define the radio communication
  radio.begin();
  radio.setAutoAck(true);
  radio.enableAckPayload();
  radio.enableDynamicPayloads();
  radio.setRetries(5, 5);
  radio.setChannel(RADIO_CHANNEL);
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_MIN); //RF24_PA_MIN (-18dBm), RF24_PA_LOW (-12dBm), RF24_PA_HIGH (-6dbm), RF24_PA_MAX (0dBm)
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
byte telemetry_counter = 0;
unsigned int packet_state;
unsigned int val_packet_state;

void send_and_receive_data()
{
  switch (sizeof(rc_packet_size))
  {
    case 4:  val_packet_state = 8130; //2ch
    break;
    case 6:  val_packet_state = 7360; //3ch
    break;
    case 8:  val_packet_state = 6720; //4ch
    break;
    case 10: val_packet_state = 6180; //5ch
    break;
    case 12: val_packet_state = 5200; //6ch
    break;
    case 14: val_packet_state = 4970; //7ch
    break;
    case 16: val_packet_state = 4780; //8ch
    break;
    case 18: val_packet_state = 4610; //9ch
    break;
    case 20: val_packet_state = 4080; //10ch
    break;
    case 22: val_packet_state = 3960; //11ch
    break;
    case 24: val_packet_state = 3510; //12ch
    break;
  }
  
  if (radio.available())
  {
    radio.writeAckPayload(1, &telemetry_packet, sizeof(telemetry_packet_size));
    
    radio.read(&rc_packet, sizeof(rc_packet_size));
    
    telemetry_counter++;
    
    battery_check();
    fs_time = millis();
  }
  
  if (packet_state++ > val_packet_state)
  {
    if (telemetry_counter < 10)                            telemetry_packet.rssi = 0;
    if (telemetry_counter > 10 && telemetry_counter < 30)  telemetry_packet.rssi = 10;
    if (telemetry_counter > 30 && telemetry_counter < 60)  telemetry_packet.rssi = 50;
    if (telemetry_counter > 60 && telemetry_counter < 80)  telemetry_packet.rssi = 70;
    if (telemetry_counter > 80 && telemetry_counter < 100) telemetry_packet.rssi = 90;
    if (telemetry_counter > 100)                           telemetry_packet.rssi = 100;
    
    telemetry_counter = 0;
    packet_state = 0;
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
  if (millis() - adc_time > 1000) //delay adc reading battery
  {
    adc_time = millis();
    
    telemetry_packet.batt_A1 = map(analogRead(PIN_BATTERY), 0, 1023, 0, 255);
    
    low_batt_detect = telemetry_packet.batt_A1 <= (255 / BATTERY_VOLTAGE) * MONITORED_VOLTAGE;
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
 
