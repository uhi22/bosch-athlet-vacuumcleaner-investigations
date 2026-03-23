#define PIN_PB4 12
#define PIN_PB5 13

void setup() {
  pinMode(PIN_PB4, OUTPUT);
  pinMode(PIN_PB5, OUTPUT);
}

void loop() {
  digitalWrite(PIN_PB4, HIGH);
  delay(250);
  digitalWrite(PIN_PB5, HIGH);
  delay(250);
  digitalWrite(PIN_PB4, LOW);
  delay(250);
  digitalWrite(PIN_PB5, LOW);
  delay(250);
}
