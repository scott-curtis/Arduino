// A_ pins are analog, _ pins are digital. Speed pins must be on a PWM specific, digital pin. 

// Ultrasonic Sensor Pins
int trigPin = 13;
int echoPin = A1;

// Motor pins
int bkwdRightPin=A2; 
int fwdRightPin=A3;
int speedPinA=3;
int fwdLeftPin=A4; 
int bkwdLeftPin=A5;
int speedPinB=11;

// IR receiver pin
int RECV_PIN = 12;

int speed=255;


#include <IRremote.h>
#include <LiquidCrystal.h>


IRrecv irrecv(RECV_PIN);

String nextChar;

//LCD pins cannot be changed, they are hard-coded. Also uses pin 10 for backlighting
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

decode_results results;


void setup(){
  // Initialise all our pins to be ready for I/O
  
  pinMode(bkwdRightPin,OUTPUT);
  pinMode(fwdRightPin,OUTPUT);
  pinMode(speedPinA,OUTPUT);
  pinMode(fwdLeftPin,OUTPUT);
  pinMode(bkwdLeftPin,OUTPUT);
  pinMode(speedPinB,OUTPUT);
  
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  
  
  // Initialize the IR and the LCD options (location of screen to start/autoscroll)
  irrecv.enableIRIn();
  
  lcd.begin(16,2);
  lcd.autoscroll();
  lcd.setCursor(16,1);
}


void loop() {
  
  // Do internal pull-ups for the trig pin, and wait duration (microseconds) to calculate distance
  long duration, distance;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);      
  digitalWrite(trigPin, HIGH);
                           
  delayMicroseconds(10); 
  digitalWrite(trigPin, LOW);
  
  // Calculate the distance in CM
  duration = pulseIn(echoPin, HIGH);
  distance = (duration/2) / 29.1;
  
  // If we are less than 10cm away from crashing, STOP!
  if (distance < 10) {  
    analogWrite(speedPinA,0);
    analogWrite(speedPinB,0);
  }
  
  
  // Listen for input from IR. Hex codes are random as far as I can tell
  if (irrecv.decode(&results)) {    
    switch(results.value) {
      
      // nextChar is the next char to be printed to the LCD screen
      // LOW and HIGH are the only allowed variables on Analog inputs
      // Directions are on or off only. Forwards and backwards for a pin must be opposite (Or both off)
      
      case 0xFF629D:
        digitalWrite(bkwdRightPin,LOW);
        digitalWrite(fwdRightPin,HIGH);
        digitalWrite(bkwdLeftPin,LOW);
        digitalWrite(fwdLeftPin,HIGH);
        nextChar = "^ ";
        break;

      case 0xFFA857:
        digitalWrite(bkwdRightPin,HIGH);
        digitalWrite(fwdRightPin,LOW);
        digitalWrite(bkwdLeftPin,HIGH); 
        digitalWrite(fwdLeftPin,LOW); 
        nextChar = "v ";
        break;

      case 0xFF22DD:
        digitalWrite(bkwdRightPin,LOW);
        digitalWrite(fwdRightPin,HIGH);
        digitalWrite(bkwdLeftPin,HIGH);
        digitalWrite(fwdLeftPin,LOW); 
        nextChar = "< ";
        break;

      case 0xFFC23D:
        digitalWrite(bkwdRightPin,HIGH);
        digitalWrite(fwdRightPin,LOW); 
        digitalWrite(bkwdLeftPin,LOW);
        digitalWrite(fwdLeftPin,HIGH); 
        nextChar = "> ";
        break;

      case 0xFF02FD:
        speed=0;
        nextChar = "OK ";
        break;

      case 0xFF6897:
        nextChar = "1 ";
        
        // Calculate a percentage of the speed, and set speed to that percent
        speed = 255 * (1 * .1);
        break;

      case 0xFF9867:
        nextChar = "2 ";
        speed = 255 * (2 * .1);
        break;

      case 0xFFB04F:
        nextChar = "3 ";
        speed = 255 * (3 * .1);
        break;

      case 0xFF30CF:
        nextChar = "4 ";
        speed = 255 * (4 * .1);
        break;

      case 0xFF18E7:
        nextChar = "5 ";
        speed = 255 * (5 * .1);
        break;

      case 0xFF7A85:
        nextChar = "6 ";
        speed = 255 * (6 * .1);
        break;

      case 0xFF10EF:
        nextChar = "7 ";
        speed = 255 * (7 * .1);
        break;

      case 0xFF38C7:
        nextChar = "8 ";
        speed = 255 * (8 * .1);
        break;

      case 0xFF5AA5:
        nextChar = "9 ";
        speed = 255 * (9 * .1);
        break;

      case 0xFF4AB5:
        nextChar = "0 ";
        speed = 255 * (10 * .1);
        break;

      case 0xFF42BD:
        nextChar = "* ";
        break;

      case 0xFF52AD:
        nextChar = "# ";
        break;
      
      case 0xFFFFFFFF:
      // catch "bad" signals from the remote. Option to handle that in this case.
        //lcd.print("Come Closer! ");
        break;
    }
    
    // Commit our changes to the speed of the motor
    analogWrite(speedPinA,speed);
    analogWrite(speedPinB,speed);
    lcd.print(nextChar);
    lcd.print(" ");
    
    // Continue looking for more IR
    irrecv.resume();
  }
}
