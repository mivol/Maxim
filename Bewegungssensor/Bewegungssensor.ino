int Bewegungssensor = 15;

void setup() {
  Serial.begin(9600);
  pinMode(Bewegungssensor, INPUT);
}

void loop() {
  Bewegungssensor = digitalRead(15);
   if (Bewegungssensor == HIGH) {
    Serial.println("Bewegung erkannt");
  }
  else {
    Serial.println("keine Bewegung erkannt");
  }
  delay(500);
}
