/*
Sound Locator X-axis
based on >> http://www.instructables.com/id/Sound-Locator/?ALLSTEPS
________________________
|Mic3  (Top View)   Mic1|
|       Mic2            |
|_______________________|

 */
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
// called this way, it uses the default address 0x40
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();    
// Depending on your servo make, the pulse width min and max may vary, you 
// want these to be as small/large as possible without hitting the hard stop
// for max range. You'll have to tweak them as necessary to match the servos you
// have!
#define SERVOMIN  170 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  510 // this is the 'maximum' pulse length count (out of 4096)

// our servo # counter
uint8_t servonum = 0;

// Mic iput variables
int mic1 = A1; // Microphone input on A1
int mic2 = A0; // Microphone input on A0
int mic3 = A2; // Microphone input on A2

// Noise threshold before changing position 
int threshold = 60;

// Delay between samples in terms of Milliseconds
int sampledelay = 2;

// servo position:center
int Xpos = 340; //Center
void setServoPulse(uint8_t n, double pulse) {
  double pulselength;
  
  pulselength = 1000000;   // 1,000,000 us per second
  pulselength /= 60;   // 60 Hz
  Serial.print(pulselength); Serial.println(" us per period"); 
  pulselength /= 4096;  // 12 bits of resolution
  Serial.print(pulselength); Serial.println(" us per bit"); 
  pulse *= 1000;
  pulse /= pulselength;
  Serial.println(pulse);
  pwm.setPWM(n, 0, pulse);
}

void setup() {
  // initialize the I/O pins as outputs
pwm.begin();
 
  pwm.setPWMFreq(60);  // Analog servos run at ~60 Hz updates
  yield();
  pwm.setPWM(servonum, 0, 335);
   delay(sampledelay); 
  pwm.setPWM(servonum, 1, 335);
   delay(sampledelay); 
}

void loop() {
   
  // read input:
  readMics();
   // Drive each servo one at a time
 
    pwm.setPWM(servonum, 0, Xpos);
  delay(sampledelay); 
   servonum ++;
  if (servonum > 2) servonum = 0;
}


   
void readMics() {
  
  // read the Mics Values
 
 mic1 = analogRead(A1);
 mic2 = analogRead(A0);
 mic3 = analogRead(A2);
 
 
 //Test if threshold hurdle met before proceeding
 
 if (mic1-mic2>threshold || mic2-mic1>threshold|| mic2-mic3>threshold ||  mic3-mic2>threshold ||  mic3-mic1>threshold ||  mic1-mic3>threshold)
 
 {
   
 // Sound Direction Algorithm
 
 // X-Axis Positive movement
 if (mic1>mic3 && mic1>mic2) {
   Xpos= Xpos + 10;
 }
 
  // X-Axis negative movement
 if (mic3>mic1 && mic3>mic2 ) {
   Xpos= Xpos - 10;
 }

  // X-Axis Center movement
 if (mic2>mic1 && mic2>mic3 ) {
   Xpos= 340;
 }
   // Keep X Axis coordinates within boundaries 0-180
  if (Xpos <= 180) {
  Xpos=180;
  }

  if (Xpos >= 550) {
  Xpos=550;
  }
 
 
 
 }
 
 
}



