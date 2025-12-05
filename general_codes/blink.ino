// Autor: Vitor Augusto Tibério - Engenharia Elétrica - USP São Carlos // 

// Blink Test - ESP 32 // 

int ledPin = 2; 

void setup() {
  pinMode(ledPin, OUTPUT);
}

void loop() {
  digitalWrite(ledPin, HIGH);
  delay(1000);
  digitalWrite(ledPin, LOW);
  delay(1000);
}
