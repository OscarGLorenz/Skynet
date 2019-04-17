#define ESC A0
#define AIN1 5
#define AIN2 10
#define BIN1 6
#define BIN2 9

#define RIGHT_LEFTRIGHT 0
#define RIGHT_UPDOWN 1
#define LEFT_UPDOWN 2
#define LEFT_LEFTRIGHT 3
#define CHANNEL5 4
#define MUERTE 5

#include "FlySkyIBus.h"
//#include "Servo.h"

//Servo esc;

void movete(uint8_t a, uint8_t b, uint8_t c, uint8_t d) {
  analogWrite(AIN1, a);
  analogWrite(AIN2, b);
  analogWrite(BIN1, c);
  analogWrite(BIN2, d);

}
void setup() {
  IBus.begin(Serial1);

  //esc.attach(ESC);
  //esc.writeMicroseconds(1000);

  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  IBus.loop();

  //esc.writeMicroseconds(IBus.readChannel(MUERTE));
  //int r = (IBus.readChannel(RIGHT_UPDOWN) - 1500)*51/100;
  //int l = (IBus.readChannel(LEFT_UPDOWN) - 1500)*51/100;
  float gira;
  float gira2;
  int alante = (int)(((int) IBus.readChannel(RIGHT_UPDOWN)) - 1500) / 500.0 * 255.0 ;   //Adelante o atrás rango -255,255
  float lado   = (float)(((int) IBus.readChannel(RIGHT_LEFTRIGHT)) - 1500) / 500.0;      //Girar hacia un lado u otro, rango -1,1
  float potensa  = (((int)IBus.readChannel(LEFT_UPDOWN)) - 1000) / 1000.0;    //Potencia, rango 0,1
  float rotate = (((int)IBus.readChannel(LEFT_LEFTRIGHT)) - 1500) / 500.0;            //Rotar, rango -1,1

  gira2 = lado;
  /*if(lado>0){
    gira2 = lado;
    }
    else {
    gira = 1.0+lado;
    gira2 = fabs(lado);
    }*/
  int vel = potensa * alante;
  Serial.println("Potencia: " + String(potensa) + " Alante: " + String(alante) + " Vel: " + String(vel) + " Lado: " + String(lado)+ "Rotate:" + String(rotate));
  if (vel > 0) {
    movete(0, vel * (1 - gira2), 0, vel * (1 + gira2));
    //Serial.println(String(0),String(vel*gira2),String(0),String(vel*gira));
  }
  else {
    movete(abs(vel) * (1 - gira2), 0, abs(vel) * (1 + gira2), 0);
    //Serial.println(String(vel*gira),String(0),String(vel*gira2),String(0));
  }
  if (lado == 0&&vel==0) {
    if (rotate > 0) {
      vel = 255*rotate;
      movete(0, 255 * rotate, 255 * rotate, 0);
    } else {
      vel = -255*rotate;
      movete(vel, 0, 0, vel);
    }
  }

}
