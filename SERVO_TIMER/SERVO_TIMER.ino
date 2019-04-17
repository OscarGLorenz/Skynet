#include "FlySkyIBus.h"

#define sbi(port,pin) (port |= (1<<pin))
#define cbi(port,pin) (port &= ~(1<<pin))

static volatile uint16_t counter;  // Overflows needed to width
static volatile uint16_t lowTime;  // The period must be 20ms
static volatile bool state;  // High or low

ISR(TIMER3_COMPA_vect) {
  if (state) {
    sbi(PORTF,PF7);
    OCR3A = counter;
  } else {
    cbi(PORTF,PF7);
    OCR3A = lowTime;
  }
  
  state = !state;
  TCNT3 = 0;
}

void servoInit() {
  cli();
  
  // Set PF7 as outrut
  sbi(DDRF,PF7);
  
  IBus.begin(Serial1);

  // Normal Port Operation
  cbi(TCCR3B, WGM33);
  cbi(TCCR3B, WGM32);
  cbi(TCCR3A, WGM31);
  cbi(TCCR3A, WGM30);

  // COMPA Normal operation
  cbi(TCCR3A, COM3A1);
  cbi(TCCR3A, COM3A0);
  
  // div 8 prescaler
  cbi(TCCR3B, CS30);
  sbi(TCCR3B, CS31);
  cbi(TCCR3B, CS32);

  // Clear timer3 counter 
  TCNT3 = 0; 

  // Enable the compare A interrupt    
  sbi(TIMSK3,OCIE3A) ;

  sei();
}

void servoWrite(int pulsewidth) {
    // Timer add one each 0.5us. Then if pulsewidth in us, multiply by 2
    counter = pulsewidth * 2;  
    // Period should be 20ms = 20000us = 40000 * 0.5us 
    lowTime = 40000;  
}

void setup() {
  delay(1000);
  servoInit();
}
int pos;
void loop() {
    IBus.loop();

    servoWrite(IBus.readChannel(5));     
    delay(10);
}
