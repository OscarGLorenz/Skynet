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

#include "Nomires.h"

void setup() {
  noMirarSetup();
  IBus.begin(Serial1);
  servoInit();
  //Serial.begin(9600);
}

void loop() {
  IBus.loop();
  servoWrite(IBus.readChannel(5));   
  int vel;
  int alante = (int)(((int) IBus.readChannel(RIGHT_UPDOWN)) - 1500) / 500.0 * 255.0 ;   //Adelante o atrás rango -255,255
  float lado   = (float)(((int) IBus.readChannel(RIGHT_LEFTRIGHT)) - 1500) / 500.0;      //Girar hacia un lado u otro, rango -1,1
  float potensa  = (((int)IBus.readChannel(LEFT_UPDOWN)) - 1000) / 1000.0;    //Potencia, rango 0,1
  float rotate = (((int)IBus.readChannel(LEFT_LEFTRIGHT)) - 1500) / 500.0;            //Rotar, rango -1,1
  vel = potensa * alante;
  lado = 1.5 * lado;
  if (lado > 1) lado = 1;
  if (lado < -1) lado = -1;
  //Serial.println("Potencia: " + String(potensa) + " Alante: " + String(alante) + " Vel: " + String(vel) + " Lado: " + String(lado) + " Rotate:" + String(rotate));
  if (vel > 0) { //Ir para alante
    if (lado > 0) {
      writeAnalog(vel * (1 - lado), 0, 0, vel);
    }
    else {
      writeAnalog(vel, 0, 0, vel * (1 + lado));
    }
    //Serial.println(String(0),String(vel*gira2),String(0),String(vel*gira));
  } else if (vel < 0) { //Ir pa tras
    if (lado > 0) {
      writeAnalog(0, -vel * (1 - lado), -vel, 0);
    }
    else {
      writeAnalog(0, -vel, -vel * (1 + lado), 0);
    }      //Serial.println(String(vel*gira),String(0),String(vel*gira2),String(0));
  }
  else { //rotar
    if (rotate > 0) {
      writeAnalog(255 * rotate, 0, 255 * rotate, 0);
    }
    else {
      writeAnalog(0, 255 * rotate, 0, 255 * rotate);
    }
  }
}
