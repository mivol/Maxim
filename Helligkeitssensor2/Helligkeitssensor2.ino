void setup() {
  Serial.begin(9600);
  pinMode(35, INPUT);
}

void loop() {
  int Helligkeit = analogRead(35);
  Serial.println(Helligkeit);
  delay(500);
}
