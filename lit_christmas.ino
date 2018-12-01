// pins 15~17 to GND, I2C bus address is 0x20
#include <TinyWireM.h>

#define F_CPU 8000000  // This is used by delay.h library

#include <stdlib.h>
#include <avr/io.h>        // Adds useful constants
#include <util/delay.h>    // Adds delay_ms and delay_us functions

#include <avr/sleep.h>
#include <avr/interrupt.h>

// Routines to set and claer bits (used in the sleep code)
#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif
#define INTERNAL2V56NC (5)  // We use the internal voltage reference

//************ USER PARAMETERS***********************
//**********MODE*************************************
// Variables for the Sleep/power down modes:
volatile boolean f_wdt = 1;
int mode = 8;

// the setup routine runs once when you press reset:
void setup()  {
  // Set up IO pins
  TinyWireM.begin(); // wake up I2C bus
  // set I/O pins to outputs
  TinyWireM.beginTransmission(0x20);
  TinyWireM.write(0x00); // IODIRA register
  TinyWireM.write(0x00); // set all of port A to outputs
  TinyWireM.endTransmission();
  TinyWireM.beginTransmission(0x20);
  TinyWireM.write(0x01); // IODIRB register
  TinyWireM.write(0x00); // set all of port B to outputs
  TinyWireM.endTransmission();
  delay(100);  // Wait a while for this to stabilise.
  setup_watchdog(5);
  // 0=16ms, 1=32ms,2=64ms,3=128ms,4=250ms,5=500ms
  // 6=1 sec,7=2 sec, 8=4 sec, 9= 8sec
}

// the loop routine runs over and over again forever:
void loop()  {
  if      (mode == 0) {setup_watchdog(5);all_on();}
  else if (mode == 1) {setup_watchdog(5);flip_flop();}
  else if (mode == 2) {setup_watchdog(4);tic_toc();}
  else if (mode == 3) {setup_watchdog(3);sweep();}
  else if (mode == 4) {setup_watchdog(2);binary_count();}
  else if (mode == 5) {setup_watchdog(2);knight_rider();}
  else if (mode == 6) {setup_watchdog(4);grey_counter();}
  else if (mode == 7) {setup_watchdog(5);rand_map();}
  else if (mode == 8) {setup_watchdog(3);loop_roof();}
  mode++;
  if(mode == 9){mode = 0;}
}

void loop_roof()
{
  for(int i = 0; i < 50; i++)
  {
   write_b_i2c(128);
   write_no_sleep_b_i2c(0);
   write_a_i2c(1);
   write_a_i2c(2);
   write_a_i2c(4);
   write_a_i2c(8);
   write_a_i2c(32);
   write_a_i2c(16);
   write_no_sleep_a_i2c(0);
   write_b_i2c(1);
   write_b_i2c(2);
   write_b_i2c(4);
   write_b_i2c(8);
   write_b_i2c(16);
   write_b_i2c(32);
   write_b_i2c(64);
  }
}


void rand_map()
{
  for(int i = 0; i < 255; i++)
  {
    int r = random(0,255);
    write2i2c(r);
  }
}

void grey_counter()
{
  int g = 0;
  for(int i = 0; i < 255; i++)
  {
    g = bintogray(i);
    write2i2c(g);
  }
  for(int i = 255; i > 0; i--)
  {
    g = bintogray(i);
    write2i2c(g);
  }
}

void knight_rider()
{
  for (int s = 1; s < 255; s++)
  {
   write_b_i2c(128);
   write_no_sleep_b_i2c(0);
   write_a_i2c(1);
   write_a_i2c(2);
   write_a_i2c(4);
   write_a_i2c(8);
   write_a_i2c(32);
   write_a_i2c(16);
   write_a_i2c(32);
   write_a_i2c(8);
   write_a_i2c(4);
   write_a_i2c(2);
   write_a_i2c(1);
   write_no_sleep_a_i2c(0);
  }
}

void all_on()
{
  for (int a = 0; a < 50; a++)
  {
    write2i2c(255);
  }
}

void binary_count()
{
  for (int a = 0; a < 256; a++)
  {
     write2i2c(a);
     write2i2c(0);
  }
}

void flip_flop()
{
    for (int s = 1; s < 255; s = (s << 1) | 1 ) {
      flash2i2c(101,85);   // 01100101 , 01010101
      flash2i2c(154,170);  // 10011010 , 10101010
    }
}

void sweep()
{
  for (int s = 3; s < 256; s = s << 1)
  {
      write2i2c(s);
      write2i2c(0);
  }
  for (int r = 192; r > 1; r = r >> 1)
  {
      write2i2c(r);
      write2i2c(0);
  }
}

void tic_toc()
{
  for (int s = 1; s < 255; s = (s << 1) | 1)
  {
      write2i2c(s);
      write2i2c(0);
  }
  for (int r = 255; r > 1; r = r >> 1)
  {
      write2i2c(r);
      write2i2c(0);
  }
}

int bintogray(int bin)
{
    return bin ^ (bin >> 1);
}

void flash2i2c(int r, int s)
{
  TinyWireM.beginTransmission(0x20);
  TinyWireM.write(0x12); // GPIOA
  TinyWireM.write(r); // port A
  TinyWireM.endTransmission();
  TinyWireM.beginTransmission(0x20);
  TinyWireM.write(0x13); // GPIOB
  TinyWireM.write(s); // port B
  TinyWireM.endTransmission();
  TinyWireM.beginTransmission(0x20);
  TinyWireM.write(0x12); // GPIOA
  TinyWireM.write(0); // port A
  TinyWireM.endTransmission();
  TinyWireM.beginTransmission(0x20);
  TinyWireM.write(0x13); // GPIOB
  TinyWireM.write(0); // port B
  TinyWireM.endTransmission();
  if (f_wdt == 1) { // wait for timed out watchdog / flag is set when a watchdog timeout occurs
    f_wdt = 0;     // reset flag
    system_sleep();  // Send the unit to sleep
  }
}

void write2i2c(int r)
{
  TinyWireM.beginTransmission(0x20);
  TinyWireM.write(0x12); // GPIOA
  TinyWireM.write(r); // port A
  TinyWireM.endTransmission();
  TinyWireM.beginTransmission(0x20);
  TinyWireM.write(0x13); // GPIOB
  TinyWireM.write(r); // port B
  TinyWireM.endTransmission();
  if (f_wdt == 1) { // wait for timed out watchdog / flag is set when a watchdog timeout occurs
    f_wdt = 0;     // reset flag
    system_sleep();  // Send the unit to sleep
  }
}

void write_a_i2c(int r)
{
  TinyWireM.beginTransmission(0x20);
  TinyWireM.write(0x12); // GPIOA
  TinyWireM.write(r); // port A
  TinyWireM.endTransmission();
  if (f_wdt == 1) { // wait for timed out watchdog / flag is set when a watchdog timeout occurs
    f_wdt = 0;     // reset flag
    system_sleep();  // Send the unit to sleep
  }
}

void write_no_sleep_a_i2c(int r)
{
  TinyWireM.beginTransmission(0x20);
  TinyWireM.write(0x12); // GPIOA
  TinyWireM.write(r); // port A
  TinyWireM.endTransmission();
}

void write_b_i2c(int r)
{
  TinyWireM.beginTransmission(0x20);
  TinyWireM.write(0x13); // GPIOA
  TinyWireM.write(r); // port A
  TinyWireM.endTransmission();
  if (f_wdt == 1) { // wait for timed out watchdog / flag is set when a watchdog timeout occurs
    f_wdt = 0;     // reset flag
    system_sleep();  // Send the unit to sleep
  }
}

void write_no_sleep_b_i2c(int r)
{
  TinyWireM.beginTransmission(0x20);
  TinyWireM.write(0x13); // GPIOA
  TinyWireM.write(r); // port A
  TinyWireM.endTransmission();
}

// set system into the sleep state
// system wakes up when wtchdog is timed out
void system_sleep() {
  cbi(ADCSRA, ADEN);                   // switch Analog to Digitalconverter OFF
  set_sleep_mode(SLEEP_MODE_PWR_DOWN); // sleep mode is set here
  sleep_enable();
  sleep_mode();                        // System actually sleeps here
  sleep_disable();                     // System continues execution here when watchdog timed out
  sbi(ADCSRA, ADEN);                   // switch Analog to Digitalconverter ON
}
// 0=16ms, 1=32ms,2=64ms,3=128ms,4=250ms,5=500ms
// 6=1 sec,7=2 sec, 8=4 sec, 9= 8sec
void setup_watchdog(int ii) {
  byte bb;
  int ww;
  if (ii > 9 ) ii = 9;
  bb = ii & 7;
  if (ii > 7) bb |= (1 << 5);
  bb |= (1 << WDCE);
  ww = bb;

  MCUSR &= ~(1 << WDRF);
  // start timed sequence
  WDTCR |= (1 << WDCE) | (1 << WDE);
  // set new watchdog timeout value
  WDTCR = bb;
  WDTCR |= _BV(WDIE);
}

// Watchdog Interrupt Service / is executed when watchdog timed out
ISR(WDT_vect) {
  f_wdt = 1; // set global flag
}
