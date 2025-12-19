
// Incluindo as Bibliotecas // 
#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN      5
#define RST_PIN     22
#define RELAY_PIN   26

#define LED_GREEN   12
#define LED_RED     13
#define BUZZER_PIN  27

// UID autorizado
byte authorizedUID[4] = {0xDE, 0xAD, 0xBE, 0xEF};

MFRC522 rfid(SS_PIN, RST_PIN);

void setup() {
  Serial.begin(115200);

  pinMode(RELAY_PIN, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  digitalWrite(RELAY_PIN, LOW);
  digitalWrite(LED_GREEN, LOW);
  digitalWrite(LED_RED, LOW);
  digitalWrite(BUZZER_PIN, LOW);

  SPI.begin();
  rfid.PCD_Init();

  Serial.println("Fechadura RFID com LED e Buzzer pronta");
}

void loop() {
  if (!rfid.PICC_IsNewCardPresent()) return;
  if (!rfid.PICC_ReadCardSerial()) return;

  Serial.print("UID lido: ");
  for (byte i = 0; i < rfid.uid.size; i++) {
    Serial.print(rfid.uid.uidByte[i], HEX);
    Serial.print(" ");
  }
  Serial.println();

  if (checkUID()) {
    acesso_permitido();
  } else {
    acesso_negado();
  }

  rfid.PICC_HaltA();
}

bool checkUID() {
  for (byte i = 0; i < rfid.uid.size; i++) {
    if (rfid.uid.uidByte[i] != authorizedUID[i]) {
      return false;
    }
  }
  return true;
}

void acesso_permitido() {
  Serial.println("Acesso liberado");

  digitalWrite(LED_GREEN, HIGH);
  beep(2, 100);
  digitalWrite(RELAY_PIN, HIGH);

  delay(3000);

  digitalWrite(RELAY_PIN, LOW);
  digitalWrite(LED_GREEN, LOW);
}

void acesso_negado() {
  Serial.println("Acesso negado");

  digitalWrite(LED_RED, HIGH);
  beep(1, 600);
  delay(300);
  digitalWrite(LED_RED, LOW);
}

void beep(int times, int duration) {
  for (int i = 0; i < times; i++) {
    digitalWrite(BUZZER_PIN, HIGH);
    delay(duration);
    digitalWrite(BUZZER_PIN, LOW);
    delay(100);
  }
}
