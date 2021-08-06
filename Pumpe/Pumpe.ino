int GSM1 = 19;
int in1 = 18;
int in2 = 5;
const int freq = 30000;
const int pwmChannel = 0;
const int resolution = 8;
int dutyCycle = 200;

void forward(){
  Serial.println("Moving Forward");
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH); 
  delay(2000);
}
void forward_smooth(){
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    while (dutyCycle <= 255){
    ledcWrite(pwmChannel, dutyCycle);   
    Serial.print("Forward with duty cycle: ");
    Serial.println(dutyCycle);
    dutyCycle = dutyCycle + 5;
    delay(500);
  }
  dutyCycle = 200;
  }
void back(){
  Serial.println("Moving Backwards");
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW); 
  delay(2000);
}
void off(){
  Serial.println("Motor stopped");
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  delay(1000);
}


void setup() {
  Serial.begin(9600);
  
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(GSM1, OUTPUT);

  ledcSetup(pwmChannel, freq, resolution);  
  ledcAttachPin(GSM1, pwmChannel);

}

void loop() {
  forward();
  off();
}
