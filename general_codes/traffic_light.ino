// Developed by Vitor Augusto Tibério - Undergraduate Student at University of São Paulo (USP)
// Electrical Engineering Department 


// Defining the Pins
int ledRed = 4;
int ledYellow = 2;
int ledGreen = 15;

void setup() {
  pinMode(ledRed, OUTPUT);
  pinMode(ledYellow, OUTPUT);
  pinMode(ledGreen, OUTPUT);
  digitalWrite(ledRed, LOW);
  digitalWrite(ledYellow, LOW);
  digitalWrite(ledGreen, LOW);
}

void loop() {
  digitalWrite(ledRed, LOW);
  digitalWrite(ledGreen, HIGH);
  delay(5000);
  digitalWrite(ledGreen, LOW);
  digitalWrite(ledYellow, HIGH);
  delay(1000);
  digitalWrite(ledYellow, LOW);
  digitalWrite(ledRed, HIGH);
  delay(8000);
}
