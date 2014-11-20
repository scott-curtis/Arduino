// I/O Arduino (Master)
#include <IRremote.h>
#include <LiquidCrystal.h>
#include <Wire.h>

int trigPin = 13;
int echoPin = A1;

int RECV_PIN = 12;

int firePin1 = 1;
int firePin2 = 2;
int firePin3 = 3;

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);


decode_results results;
IRrecv irrecv(RECV_PIN);
String nextChar;
int nextCmd;

boolean flag = false;

void setup(){
 Serial.begin(9600); 
 Wire.begin();
 
 irrecv.enableIRIn();
  
 lcd.begin(16,2);
 lcd.autoscroll();
 lcd.setCursor(16,1);
 
}

void loop(){
  
 if (analogRead(firePin1) > 400 || analogRead(firePin2) > 400 || analogRead(firePin3) > 400){
   nextChar = "^ ";
   nextCmd = 100;
   flag = true;
   
   Wire.beginTransmission(1);
   Wire.write(nextCmd);
   Wire.endTransmission();
 }
 
 if (flag == true && analogRead(firePin2) < 250){
   nextChar = "OK ";
   nextCmd = 11;
   flag = false;
   
   Wire.beginTransmission(1);
   Wire.write(nextCmd);
   Wire.endTransmission();
 }
 
 long duration, distance;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);      
  digitalWrite(trigPin, HIGH);
                           
  delayMicroseconds(10); 
  digitalWrite(trigPin, LOW);
  
  // Calculate the distance in CM
  duration = pulseIn(echoPin, HIGH);
  distance = (duration/2) / 29.1;
 
 
 if (irrecv.decode(&results)) {    
    switch(results.value) {
      case 0xFFFFFFFF:
        break;
      
      case 0xFF629D:
        nextChar = "^ ";
        nextCmd = 1;
        break;

      case 0xFFA857:
        nextChar = "v ";
        nextCmd = 2;
        break;

      case 0xFF22DD:
        nextChar = "< ";
        nextCmd = 3;
        break;

      case 0xFFC23D:
        nextChar = "> ";
        nextCmd = 4;
        break;

      case 0xFF02FD:
        nextChar = "OK ";
        nextCmd = 0;
        break;

      case 0xFF6897:
        nextChar = "1 ";
        nextCmd = 10;
        break;

      case 0xFF9867:
        nextChar = "2 ";
        nextCmd = 20;
        break;

      case 0xFFB04F:
        nextChar = "3 ";
        nextCmd = 30;
        break;

      case 0xFF30CF:
        nextChar = "4 ";
        nextCmd = 40;
        break;

      case 0xFF18E7:
        nextChar = "5 ";
        nextCmd = 50;
        break;

      case 0xFF7A85:
        nextChar = "6 ";
        nextCmd = 60;
        break;

      case 0xFF10EF:
        nextChar = "7 ";
        nextCmd = 70;
        break;

      case 0xFF38C7:
        nextChar = "8 ";
        nextCmd = 80;
        break;

      case 0xFF5AA5:
        nextChar = "9 ";
        nextCmd = 90;
        break;

      case 0xFF4AB5:
        nextChar = "0 ";
        nextCmd = 100;
        break;

      case 0xFF42BD:
        nextChar = "* ";
        nextCmd = 11;
        break;

      case 0xFF52AD:
        nextChar = "# ";
        nextCmd = 12;
        break;
    }
    
    Wire.beginTransmission(1);
    Wire.write(nextCmd);
    Wire.endTransmission();
    
    lcd.print(nextChar);
    lcd.print(" ");
   
    irrecv.resume();
 }
}
