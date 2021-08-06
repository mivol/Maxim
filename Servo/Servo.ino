#include <Servo.h>
Servo myservo;  
//int pos = 0;    // variable to store the servo position

void setup() {
  myservo.attach(4);  // attaches the servo on pin 13 to the servo object
  Serial.begin(9600);
}

void loop() {
  for (int pos = 0; pos <= 180; pos += 5) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  for (int pos = 180; pos >= 0; pos -= 5) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }

  Serial.print("Hallo");
}
