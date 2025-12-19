// Autor: Vitor Augusto Tibério - Estudante de Engenharia Elétrica //

// Sensor hall INTERNO da ESP-32! 

void setup() {
  Serial.begin(115200);
}

void loop() {
  int hallValue = hallRead();
  Serial.print("Sensor Hall: ");
  Serial.println(hallValue);
  delay(500);
}
