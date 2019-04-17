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

void writeAnalog(uint8_t ain1, uint8_t ain2, uint8_t bin1, uint8_t bin2) {
    OCR4A = ain2 << 2;
    OCR4D = (~bin1) << 2; 

    OCR1A = bin2 << 2;
    OCR1B = ain1 << 2;
}

void noMirarSetup() {
   cli();
  
  // Timer 1
  sbi(DDRB, PB5); // Pin 9  OC1A BIN2
  sbi(DDRB, PB6); // Pin 10 OC1B AIN1

  // Clear OC1A on Compare Match, set OC1A at BOTTOM
  cbi(TCCR1A, COM1A0);
  sbi(TCCR1A, COM1A1);

  // Clear OC1B on Compare Match, set OC1B at BOTTOM
  cbi(TCCR1A, COM1B0);
  sbi(TCCR1A, COM1B1);

  // PWM, Fast PWM, 10-bit
  sbi(TCCR1A, WGM10);
  sbi(TCCR1A, WGM11);
  sbi(TCCR1B, WGM12);
  cbi(TCCR1B, WGM13);

  // div 64 prescaler
  sbi(TCCR1B, CS10);
  sbi(TCCR1B, CS11);
  cbi(TCCR1B, CS12);
  
  // Reset timer
  TCNT1 = 0;
  
  // Timer 4
  sbi(DDRC, PC6); // Pin 5  OC4A AIN2
  sbi(DDRD, PD7); // Pin 6  OC4D BIN1

 // Clear OC4A on Compare Match, set OC4A at BOTTOM
  sbi(TCCR4A, COM4A0);
  cbi(TCCR4A, COM4A1);
  
  // Enable PWM on 4A
  sbi(TCCR4A, PWM4A);


 // Clear OC4D on Compare Match, set OC4D at BOTTOM
  cbi(TCCR4C, COM4D0);
  sbi(TCCR4C, COM4D1); 
  
  // Enable PWM on 4D
  sbi(TCCR4C, PWM4D);

  // PWM, Fast PWM, 10-bit
  cbi(TCCR4D, WGM40);
  cbi(TCCR4D, WGM41);
  sbi(TCCR4B, PWM4X);


  // div 64 prescaler
  sbi(TCCR4B, CS40);
  sbi(TCCR4B, CS41);
  sbi(TCCR4B, CS42);
  cbi(TCCR4B, CS43);

  // Reset timer
  TCNT4 = 0;

  sei();
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
