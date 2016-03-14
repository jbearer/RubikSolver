const char RECEIVE_INDICATOR = 10;
const char SEND_INDICATOR = 11;
const char OUTPUT_PIN = 12;

void setup() {
  pinMode(RECEIVE_INDICATOR, OUTPUT);
  pinMode(SEND_INDICATOR, OUTPUT);
  pinMode(OUTPUT_PIN, OUTPUT);
}

void loop() {
  digitalWrite(RECEIVE_INDICATOR, HIGH);
  digitalWrite(SEND_INDICATOR, HIGH);
  digitalWrite(OUTPUT_PIN, HIGH);

  delay(500);

  digitalWrite(RECEIVE_INDICATOR, LOW);
  digitalWrite(SEND_INDICATOR, LOW);
  digitalWrite(OUTPUT_PIN, LOW);

  delay(500);
}
