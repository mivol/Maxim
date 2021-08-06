////////////////////////////////////////////////////////////////////

#include <Servo.h>
#include <Adafruit_NeoPixel.h>

//#include <Wire.h>
//#include <BH1750.h>

//BH1750 lightMeter;

#define NUMPIXELS 30 // Popular NeoPixel ring size
int pos = 0;    // variable to store the servo position
int led_1 = 2;

int Bewegungssensor = 21;

////////////////////////////////////////////////////////////////////

int in1 = 18;
int in2 = 5;
const int freq = 30000;
const int pwmChannel = 1;
const int resolution = 8;
int dutyCycle = 200;



Servo myservo;
Adafruit_NeoPixel pixels(NUMPIXELS, led_1, NEO_GRB  + NEO_KHZ800);


void whiteLight() {
  //pixels.clear(); // Set all pixel colors to 'off'
  for (int i = 0; i < NUMPIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(255, 255, 255));
    pixels.show();   // Send the updated pixel colors to the hardware.
    delay(50); // Pause before next pass through loop
  }
}

void Lighteffect() {
  //pixels.clear(); // Set all pixel colors to 'off'
  for (int i = 0; i < NUMPIXELS; i++) {
     int color1 = random(0,255);
     int color2 = random(0,255);
    int color3 = random(0,255);
    pixels.setPixelColor(i, pixels.Color(color1, color2, color3));
    pixels.show();   // Send the updated pixel colors to the hardware.
    delay(50); // Pause before next pass through loop
//    color1 -= 8;
//  color2 += 8;
//color3 -= 8;
  }
}

////////////////////////////////////////////////////////////////////
void forward() {
  Serial.println("Moving Forward");
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  delay(1000);
}

void forward_smooth() {
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  while (dutyCycle <= 255) {
    ledcWrite(pwmChannel, dutyCycle);
    Serial.print("Forward with duty cycle: ");
    Serial.println(dutyCycle);
    dutyCycle = dutyCycle + 5;
    delay(500);
  }
  dutyCycle = 200;
}
void back() {
  Serial.println("Moving Backwards");
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  delay(2000);
}
void off() {
  Serial.println("Motor stopped");
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  delay(1000);
}

void servo_open(){
  for (pos = 0; pos <= 90; pos += 1) { // goes from 0 degrees to 180 degrees
      myservo.write(pos);              // tell servo to go to position in variable 'pos'
      delay(15);                       // waits 15ms for the servo to reach the position
    }
}

void servo_close(){
  for (pos = 90; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
      myservo.write(pos);              // tell servo to go to position in variable 'pos'
      delay(15);                       // waits 15ms for the servo to reach the position
    }
}

////////////////////////////////////////////////////////////////////

int taster = 33;

////////////////////////////////////////////////////////////////////

int Feuchtigkeitssensor = 13;
int liquid_level;


void setup() {

  
  ////////////////////////////////////////////////////////////////////

  myservo.attach(4);  // attaches the servo on pin 13 to the servo object
  Serial.begin(9600);

  Serial.print("Start");
  ////////////////////////////////////////////////////////////////////

  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)

  ////////////////////////////////////////////////////////////////////

  pinMode(Bewegungssensor, INPUT);

  ////////////////////////////////////////////////////////////////////

  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
 // pinMode(GSM1, OUTPUT);
  //ledcSetup(pwmChannel, freq, resolution);
  //ledcAttachPin(GSM1, pwmChannel);
 pinMode(32, INPUT);
 pinMode(15, INPUT);

  ////////////////////////////////////////////////////////////////////

  pinMode(taster, INPUT);

  ////////////////////////////////////////////////////////////////////

  pinMode(Feuchtigkeitssensor, INPUT);

  ////////////////////////////////////////////////////////////////////

 // Wire.begin();
 // lightMeter.begin();

  ////////////////////////////////////////////////////////////////////
}


void loop() {

  ////////////////////////////////////////////////////////////////////

  Bewegungssensor = digitalRead(15);
  if (Bewegungssensor == HIGH) {
    Serial.println("Bewegung erkannt");
    for (pos = 0; pos <= 90; pos += 1) { // goes from 0 degrees to 180 degrees
      myservo.write(pos);              // tell servo to go to position in variable 'pos'
      delay(15);                       // waits 15ms for the servo to reach the position
    }
                                                                                                                  //Toröffner
    Lighteffect();
    delay(300);
    pixels.clear();
    pixels.show();

    for (pos = 90; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
      myservo.write(pos);              // tell servo to go to position in variable 'pos'
      delay(15);                       // waits 15ms for the servo to reach the position
    }
  }

  ////////////////////////////////////////////////////////////////////

  liquid_level = analogRead(Feuchtigkeitssensor);
  if (liquid_level < 1500) {
    Serial.println("Der Boden ist zu trocken!");
    forward();                                                                           //Pumpe
  }
  else {
    Serial.println("Der Boden feucht genug!");
    back();
  }

  ////////////////////////////////////////////////////////////////////
/*
  //float lux = lightMeter.readLightLevel();
  float lux = 100;
  if (lux < 70) {
    whiteLight();
    Serial.println("Dunkel");
  }
  else {                                                                              //Licht bei Dunkelheit
    pixels.clear();
    pixels.show();
    delay(1000);
    Serial.println("Hell");
  }
*/

  int Helligkeit = analogRead(32);
  Serial.println(Helligkeit);
  if(Helligkeit<2500){
    whiteLight();
    Serial.println("Dunkel");
    delay(3000);
  }
  else {                                                                              //Licht bei Dunkelheit
    pixels.clear();
    pixels.show();
    delay(1000);
    Serial.println("Hell");
  }

  ////////////////////////////////////////////////////////////////////

}
