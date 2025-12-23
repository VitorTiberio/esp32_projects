// Autor: Vitor Augusto Tibério - Estudante de Engenharia Elétrica - USP São Carlos // 
// Contato: vitortiberio@usp.br

#include <ESP32Servo.h>

Servo servoBraco;
const int pinoServo = 13;

void setup() {
  Serial.begin(115200);
  servoBraco.attach(pinoServo);
}

void loop() {
  if (Serial.available()) {
    int angulo = Serial.parseInt();
    if (angulo >= 0 && angulo <= 180) {
      servoBraco.write(angulo);
    }
  }
}
