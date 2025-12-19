// Developed by: Vitor Augusto Tibério  

// Undergraduate student at University of São Paulo (USP) - Electrical Engineering // 

// Definindo os pinos // 

# define ECHO 18
# define TRIGGER 5
# define LED 21

void setup() {
  Serial.begin(115200);
  pinMode(TRIGGER, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(LED, OUTPUT);
}

void loop() {
  // Gera o pulso do ultrassônico //
  digitalWrite(TRIGGER, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGER, HIGH);
  delayMicroseconds(10); // Pulso de 10 microssegundos
  digitalWrite(TRIGGER, LOW);
  // Calcula a distância do ultrassônico //
  long duration = pulseIn(ECHO, HIGH);
  float distanceCm = duration * 0.0343 / 2.0;
  // Lê (serialmente) o sensor //
  Serial.print("Distancia: ");
  Serial.print(distanceCm);
  Serial.println(" cm");
  delay(500);
}
