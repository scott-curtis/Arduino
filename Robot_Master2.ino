// I/O Arduino (Master)
#include <IRremote.h>
#include <LiquidCrystal.h>
#include <Wire.h>

int trigPin = 13;
int echoPin = 12;
long duration;
long distance;

int firePin1 = 1;
int firePin2 = 2;
int firePin3 = 3;

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

int RECV_PIN = 11;
decode_results results;
IRrecv irrecv(RECV_PIN);
String nextChar;

int nextCmd;
int prevCmd;
unsigned long prevInput;

boolean fanOnAuto = false;
boolean canInput = true;

void setup(){
 Serial.begin(9600); 
 Wire.begin();
 
 pinMode(trigPin, OUTPUT);
 pinMode(echoPin, INPUT);
 
 irrecv.enableIRIn();
  
 lcd.begin(16,2);
 lcd.autoscroll();
 lcd.setCursor(16,1);
 lcd.print("Manual");
 
}

void loop(){
  
  // Shared code in both modes
  // 
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);      
  digitalWrite(trigPin, HIGH);
                           
  delayMicroseconds(10); 
  digitalWrite(trigPin, LOW);
  
  // Calculate the distance in CM
  duration = pulseIn(echoPin, HIGH);
  distance = (duration/2) / 29.1;
  Serial.println(analogRead(firePin1));
  Serial.println(analogRead(firePin2));
  Serial.println(analogRead(firePin3));
 
 // Check if the user entered "#", to change modes between automatic and manual
 if (irrecv.decode(&results)) {
    if (results.value == 0xFF52AD) {
      canInput = !canInput;
      
      // Show user which mode they are in. Hard to tell if remote signal broke otherwise
      if (canInput == true) {
        lcd.print("Manual"); 
      }
      else {
        lcd.print("Automatic");
      }
      // Stop all current commands before changing modes (fan and motor stop signals.)
      Wire.beginTransmission(1);
      Wire.write(0);
      Wire.endTransmission();
      // Must send both fan and motor kill signals seperately, or the second command will be ignored and slave will crash.
      Wire.beginTransmission(1);
      Wire.write(11);
      Wire.endTransmission();
    } 
    irrecv.resume();
 }
 
     // If they set it to manual mode
 if (canInput == true){
   // And there is a new command
   if (results.value != prevInput){
     // Do what they input 
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
   }
    Wire.beginTransmission(1);
    Wire.write(nextCmd);
    Wire.endTransmission();
    
    lcd.print(nextChar);
    lcd.print(" ");
    
    prevInput = results.value;
   }
 }
 
 // If the robot is in automatic mode
 else {
   Wire.beginTransmission(1);
   Wire.write(1);
   Wire.endTransmission();
   
   // Check if the fire sensors are above the threshold
   // If they are,
   
   if (analogRead(firePin1) > 200 || analogRead(firePin2) > 200 || analogRead(firePin3) > 200){
   nextChar = "^ ";
   nextCmd = 100;
   fanOnAuto = true;
   
   // Turn on the fan
   Wire.beginTransmission(1);
   Wire.write(nextCmd);
   Wire.endTransmission();
   // Stop the motors.
   Wire.beginTransmission(1);
   Wire.write(0);
   Wire.endTransmission();
 }
 
 // If the fan was just on, and the readings dropped far below the threshold,
 if (fanOnAuto == true && analogRead(firePin2) < 150){
   nextChar = "OK ";
   nextCmd = 11;
   fanOnAuto = false;
   
   // Turn off the fan.
   Wire.beginTransmission(1);
   Wire.write(nextCmd);
   Wire.endTransmission();
 }
 }
}
