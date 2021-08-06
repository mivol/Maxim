////////////////////////////////////////////////////////////////////
#include <WiFi.h>
#include <PubSubClient.h>

#include <Servo.h>
#include <Adafruit_NeoPixel.h>

#define NUMPIXELS 30 // Popular NeoPixel ring size
int pos = 0;    // variable to store the servo position
int led_1 = 2;

////////////////////////////////////////////////////////////////////

//int GSM1 = 35;
int in1 = 18;
int in2 = 5;

const int freq = 30000;
const int pwmChannel = 1;
const int resolution = 8;
int dutyCycle = 200;

#define DELAYVAL 500 // Time (in milliseconds) to pause between pixels

// Replace the next variables with your SSID/Password combination
const char* ssid = "SICK Summer University";
const char* password = "SSU2021!";

// Add your MQTT Broker IP address, example:
const char* mqtt_server = "192.168.8.21";


Servo myservo;
Adafruit_NeoPixel pixels(NUMPIXELS, led_1, NEO_GRB  + NEO_KHZ800);


WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;
bool manuellOn = false;

void servo_open() {
  for (pos = 0; pos <= 90; pos += 1) { // goes from 0 degrees to 180 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
}

void servo_close() {
  for (pos = 90; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
}

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
    int color1 = random(0, 255);
    int color2 = random(0, 255);
    int color3 = random(0, 255);
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


void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte * message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;

  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();

  // Feel free to add more if statements to control more GPIOs with MQTT

  // If a message is received on the topic esp32/output, you check if the message is either "on" or "off".
  // Changes the output state according to the message
  if (String(topic) == "Garten/LED") {
    Serial.print("Changing output to ");
    if (messageTemp == "true") {
      Serial.println("true");
      manuellOn = true;
      //Stripe on
      whiteLight();

    }
    else if (messageTemp == "false") {
      Serial.println("false");
      manuellOn = false;
      //Stripe off
      pixels.clear();
      pixels.show();
    }
  }
  if (String(topic) == "Garten/Bewässerung") {
    Serial.print("Changing output to ");
    if (messageTemp == "true") {
      Serial.println("true");
      manuellOn = true;
      //Bewässerung on
      forward();

    }
    else if (messageTemp == "false") {
      Serial.println("false");
      manuellOn = false;
      //Bewässerung off
      off();

    }
  }
  if (String(topic) == "Garten/Toröffner") {
    Serial.print("Changing output to ");
    if (messageTemp == "true") {
      Serial.println("true");
      manuellOn = true;
      //Toröffner on
      servo_open();
    }
    else if (messageTemp == "false") {
      Serial.println("false");
      manuellOn = false;
      //Toröffner off
      servo_close();
    }
  }


}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
      // Subscribe
      client.subscribe("Garten/LED");
      client.subscribe("Garten/Bewässerung");
      client.subscribe("Garten/Türöffner");

    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

////////////////////////////////////////////////////////////////////

int taster = 33;

////////////////////////////////////////////////////////////////////

int Feuchtigkeitssensor = 34;
int liquid_level;


void setup() {


  ////////////////////////////////////////////////////////////////////

  myservo.attach(4);  // attaches the servo on pin 13 to the servo object
  Serial.begin(9600);

  Serial.print("Start");
  ////////////////////////////////////////////////////////////////////

  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)

  ////////////////////////////////////////////////////////////////////


  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  //pinMode(GSM1, OUTPUT);
  //ledcSetup(pwmChannel, freq, resolution);
  //ledcAttachPin(GSM1, pwmChannel);

  ////////////////////////////////////////////////////////////////////

  pinMode(taster, INPUT);
  pinMode(32, INPUT);
  pinMode(15, INPUT); //Bewegung

  ////////////////////////////////////////////////////////////////////

  pinMode(Feuchtigkeitssensor, INPUT);

  ////////////////////////////////////////////////////////////////////

  // Wire.begin();
  // lightMeter.begin();

  ////////////////////////////////////////////////////////////////////


  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}


void loop() {


  Serial.println("Start");
  if (!client.connected()) {
    Serial.println("Reconnect");
    reconnect();
  }
  Serial.println("Client loop");
  client.loop();
  Serial.println("Go");

  //if (manuellOn == false) {
  int Bewegungssensor = digitalRead(15);
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
  if (Helligkeit < 2000) {
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
  //}

  Serial.println("End loop");

}
