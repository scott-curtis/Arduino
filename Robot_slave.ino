// Control Arduino (Slave)

#include <Wire.h>

int bkwdRightPin=A0; 
int fwdRightPin=A1;
int fwdLeftPin=A2; 
int bkwdLeftPin=A3;

int speedPinA=5;
int speedPinB=6;

int fan=11;
int fanSpeed = 0;
int speed = 0;

int inVar = 0;
int address = 1;

void setup(){
  Serial.begin(9600); 
  
  pinMode(bkwdRightPin,OUTPUT);
  pinMode(fwdRightPin,OUTPUT);
  pinMode(bkwdLeftPin,OUTPUT);
  pinMode(fwdLeftPin,OUTPUT); 
  pinMode(speedPinA,OUTPUT);
  pinMode(speedPinB,OUTPUT);
  pinMode(fan,OUTPUT);
  
  Wire.begin(address);
  Wire.onReceive(onRecv);
}

void loop(){
  switch(inVar){
  case (1):
    speed = 150;
    digitalWrite(bkwdRightPin,LOW);
    digitalWrite(fwdRightPin,HIGH);
    digitalWrite(bkwdLeftPin,LOW);
    digitalWrite(fwdLeftPin,HIGH); 
    break;
    
  case (2):
    speed = 150;
    digitalWrite(bkwdRightPin,HIGH);
    digitalWrite(fwdRightPin,LOW);
    digitalWrite(bkwdLeftPin,HIGH); 
    digitalWrite(fwdLeftPin,LOW);
    break;
    
  case (3):
    speed = 150;
    digitalWrite(bkwdRightPin,HIGH);
    digitalWrite(fwdRightPin,LOW); 
    digitalWrite(bkwdLeftPin,LOW);
    digitalWrite(fwdLeftPin,HIGH); 
    break;
    
  case (4):
    speed = 150;
    digitalWrite(bkwdRightPin,LOW);
    digitalWrite(fwdRightPin,HIGH);
    digitalWrite(bkwdLeftPin,HIGH);
    digitalWrite(fwdLeftPin,LOW);
    break;
      
  case (0):
    speed = 0;
    digitalWrite(speedPinA,0);
    digitalWrite(speedPinB,0);
    break;
    
  case (10):
    fanSpeed = 255 * .1;
    break;
    
  case (20):
    fanSpeed = 255 * .2;
    break;
    
  case (30):
    fanSpeed = 255 * .3;
    break;
  
  case (40):
    fanSpeed = 255 * .4;
    break;
    
  case (50):
    fanSpeed = 255 * .5;
    break;
    
  case (60):
    fanSpeed = 255 * .6;
    break;
    
  case (70): 
    fanSpeed = 255 * .7;
    break;
    
  case (80):  
    fanSpeed = 255 * .8;
    break;
    
  case (90): 
    fanSpeed = 255 * .9;
    break; 
  
  case (100):  
    fanSpeed = 255;
    break;
  
  case (11):
    fanSpeed = 0; 
    break;
  
  case (12):
    break; 
}
  analogWrite(speedPinA,speed);
  analogWrite(speedPinB,speed);

  analogWrite(fan,fanSpeed);
}



void onRecv(int howMany) {
  inVar = Wire.read();    // receive byte as an integer
}
