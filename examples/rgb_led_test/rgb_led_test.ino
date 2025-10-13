int RGBLED_RedPin = 3;
int RGBLED_GreenPin = 5;
int RGBLED_BluePin = 6;


void setup() {
  Serial.begin(9600); // initialize serial communications at 9600 bps
  pinMode(RGBLED_RedPin, OUTPUT);
  pinMode(RGBLED_GreenPin, OUTPUT);
  pinMode(RGBLED_BluePin, OUTPUT);
}

void loop() {
  //RED
  digitalWrite(RGBLED_RedPin, HIGH);
  digitalWrite(RGBLED_GreenPin, LOW);
  digitalWrite(RGBLED_BluePin, LOW);
  delay(1000);
  // GREEN
  digitalWrite(RGBLED_GreenPin, HIGH);
  digitalWrite(RGBLED_BluePin, LOW);
  digitalWrite(RGBLED_RedPin, LOW);
  delay(1000);
  // BLUE
  digitalWrite(RGBLED_BluePin, HIGH);
  digitalWrite(RGBLED_GreenPin, LOW);
  digitalWrite(RGBLED_RedPin, LOW);
  delay(1000);
  //Yellow
  digitalWrite(RGBLED_RedPin, HIGH);
  digitalWrite(RGBLED_GreenPin, HIGH);
  digitalWrite(RGBLED_BluePin, LOW);
  delay(1000);
  //Purple
  digitalWrite(RGBLED_RedPin, HIGH);
  digitalWrite(RGBLED_GreenPin, LOW);
  digitalWrite(RGBLED_BluePin, HIGH);
  delay(1000);
  //Cyan
  digitalWrite(RGBLED_RedPin, LOW);
  digitalWrite(RGBLED_GreenPin,HIGH);
  digitalWrite(RGBLED_BluePin, HIGH);
  delay(1000);
  

}
