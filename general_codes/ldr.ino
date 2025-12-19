// Autor: Vitor Augusto Tibério - Estudante de Engenharia Elétrica - USP // 

#define LDR_PIN 34  // GPIO analógico do ESP32

void setup() {
  Serial.begin(115200);
}

void loop() {
  int ldrValue = analogRead(LDR_PIN);
  Serial.print("Valor do LDR: ");
  Serial.println(ldrValue);
  delay(500);
}
