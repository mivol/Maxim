int taster = 33;

void setup() {
  Serial.begin(9600);
  pinMode(taster, INPUT); 
}

void loop() {
  taster = digitalRead(33);
   if (taster == HIGH) {
    Serial.println("An");
  }
  else {
    Serial.println("Aus");
  }
  delay(500);
}
