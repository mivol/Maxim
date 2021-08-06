int Feuchtigkeitssensor = 13;
int liquid_level;

void setup() {
  Serial.begin(9600); 
  pinMode(Feuchtigkeitssensor, INPUT); 
}

void loop(){
  liquid_level= analogRead(Feuchtigkeitssensor);
  Serial.println(liquid_level);
  delay(1000);
}
