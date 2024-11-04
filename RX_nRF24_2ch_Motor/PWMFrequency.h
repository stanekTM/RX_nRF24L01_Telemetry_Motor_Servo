
//
// -----------------------
// PWM FREQUENCY PRESCALER
// -----------------------
// Written by Kiwisincebirth 2014
// Revised by MacTester57 (aka TheDIYGuy999) to allow correct PWM frequencies (confirmed with oscilloscope). January 2015
// - prescale variable: replaced uint8_t with uint16_t data type (fixes bug, which did not allow frequencies < 492Hz)
// - mode variable: replaced hex format with binary to make it more readable, if compared with bit tables in the 32U4 manual
// - added comments
//

#ifndef PWMFrequency
#define PWMFrequency

#if ARDUINO >= 100
#include <Arduino.h> // Arduino 1.0
#else
#include <WProgram.h> // Arduino 0022
#endif

#if defined __AVR_ATmega32U4__

/*
 * Divides a given PWM pin frequency by a divisor.
 *
 * Arduino Leonardo AtMega 32u4 specific
 *
 * Sets the Prescaler (Divisor) for a given PWM pin. The resulting frequency
 * is equal to the base frequency divided by the given divisor:
 *   - Base frequencies:
 *      o The base frequency for pins 3 and 11    is 64,500 Hz.
 *      o The base frequency for pins 5,6,9,10,13 is 31,250 Hz.
 *   - Divisors:
 *      o The divisors available on pins 3, 5, 9, 10 and 11 are: 1, 8, 64, 256, and 1024.
 *      o The divisors available on pins 6 and 13 are all powers of two between 1 and 16384
 * 
 * PWM frequencies are tied together in pairs of pins. If one in a
 * pair is changed, the other is also changed to match:
 *   - Pins 3 and 11 are paired on timer0  8bit (Default prescale=64, Freq=977Hz)
 *   - Pins 9 and 10 are paired on timer1 16bit (Default prescale=64, Freq=490Hz)
 *   - Pins 5 is exclusivly     on timer3 16bit (Default prescale=64, Freq=490Hz)
 *   - Pins 6 and 13 are paired on timer4 10bit (default prescale=64, Freq=490Hz)
 * 
 * Note: Pins 3 and 11 operate on Timer 0 changes this pins will
 * affect the user of the main time millis() functions
 * 
 * Thanks to MacTester of the TonyMacX86 forums for his work in defining 
 * the timings and testing this library
 */

void setPWMPrescaler(uint8_t pin, uint16_t prescale)
{
  byte mode;
  
  if (pin == 3 || pin == 5 || pin == 9 || pin == 10 || pin == 11)
  {
    switch (prescale)
    {
      case    1: mode = 0b001; break;
      case    8: mode = 0b010; break;
      case   64: mode = 0b011; break;
      case  256: mode = 0b100; break;
      case 1024: mode = 0b101; break;
      default: return;
    }
  }
  else if (pin == 6 || pin == 13)
  {
    switch (prescale)
    {
      case     1: mode = 0b0001; break;
      case     2: mode = 0b0010; break;
      case     4: mode = 0b0011; break;
      case     8: mode = 0b0100; break;
      case    16: mode = 0b0101; break;
      case    32: mode = 0b0110; break;
      case    64: mode = 0b0111; break;
      case   128: mode = 0b1000; break;
      case   256: mode = 0b1001; break;
      case   512: mode = 0b1010; break;
      case  1024: mode = 0b1011; break;
      case  2048: mode = 0b1100; break;
      case  4096: mode = 0b1101; break;
      case  8192: mode = 0b1110; break;
      case 16384: mode = 0b1111; break;
      default: return;
    }
  }
  
  if (pin == 3 || pin == 11)
  {
    TCCR0B = TCCR1B & 0b11111000 | mode;
  }
  else if (pin == 9 || pin == 10)
  {
    TCCR1B = TCCR1B & 0b11111000 | mode;
  }
  else if (pin == 5)
  {
    TCCR3B = TCCR3B & 0b11111000 | mode;
  }
  else if (pin == 6 || pin == 13)
  {
    TCCR4B = TCCR4B & 0b11110000 | mode;
  }
}

#else

/*
 * Divides a given PWM pin frequency by a divisor.
 * 
 * Sets the Prescaler (Divisor) for a given PWM pin. The resulting frequency 
 * is equal to the base frequency divided by the given divisor:
 *   - Base frequencies:
 *      o The base frequency for pins 5 and 6          is 62500 Hz.
 *      o The base frequency for pins 3, 9, 10, and 11 is 31250 Hz.
 *   - Divisors:
 *      o The divisors available on pins 5, 6, 9, 10 are: 1,8,64,256,1024.
 *      o The divisors available on pins 3, 11 are: 1,8,32,64,128,256,1024.
 * 
 * PWM frequencies are tied together in pairs of pins. If one in a
 * pair is changed, the other is also changed to match:
 *   - Pins 5 and 6  are paired on timer0 (Default prescale=64, Freq=977Hz)
 *   - Pins 9 and 10 are paired on timer1 (Default prescale=64, Freq=490Hz)
 *   - Pins 3 and 11 are paired on timer2 (Default prescale=64, Freq=490Hz)
 * 
 * Note that this function will have side effects on anything else
 * that uses timers:
 *   - Changes on pins 5, or 6 may cause the delay() and
 *     millis() functions to stop working. Other timing-related
 *     functions may also be affected.
 *   - Changes on pins 9 or 10 will cause the Servo library to function
 *     incorrectly.
 * 
 * Thanks to macegr of the Arduino forums for his documentation of the
 * PWM frequency divisors. His post can be viewed at:
 *
 *   http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1235060559/0#4
 */
 
void setPWMPrescaler(uint8_t pin, uint16_t prescale)
{
  byte mode;
  
  if (pin == 5 || pin == 6 || pin == 9 || pin == 10)
  {
    switch (prescale)
    {
      case    1: mode = 0b001; break;
      case    8: mode = 0b010; break;
      case   64: mode = 0b011; break;
      case  256: mode = 0b100; break;
      case 1024: mode = 0b101; break;
      default: return;
    }
  }
  else if (pin == 3 || pin == 11)
  {
    switch (prescale)
    {
      case    1: mode = 0b001; break;
      case    8: mode = 0b010; break;
      case   32: mode = 0b011; break;
      case   64: mode = 0b100; break;
      case  128: mode = 0b101; break;
      case  256: mode = 0b110; break;
      case 1024: mode = 0b111; break;
      default: return;
    }
  }
  
  if (pin == 5 || pin == 6)
  {
    TCCR0B = (TCCR0B & 0b11111000) | mode;
  }
  else if (pin == 9 || pin == 10)
  {
    TCCR1B = (TCCR1B & 0b11111000) | mode;
  }
  else if (pin == 3 || pin == 11)
  {
    TCCR2B = (TCCR2B & 0b11111000) | mode;
  }
}

#endif

#endif
 
