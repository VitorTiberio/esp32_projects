
#define POT_PIN 35

void setup() {
  Serial.begin(115200);
}

void loop() {
  int value = analogRead(POT_PIN);
  Serial.print("Potenciômetro: ");
  Serial.println(value);
  delay(300);
}
