// Autor: Vitor Augusto Tibério - Estudante de Engenharia Elétrica - USP São Carlos
// Projeto de um gerador de corrente alternada caseiro integrado à IoT

// Importanto as bibliotecas:
#include <WiFi.h>
#include <WebServer.h>

// Configurações da Rede
const char* ssid = "TIBERIO 2.4G";      
const char* password = "Palio@1996"; 

// Definindo os pinos
#define PINO_SENSOR 34   
#define PINO_LED    4    

// FATOR DE CALIBRAÇÃO
// Como sua tensão é muito baixa (0.3V), talvez você precise diminuir MUITO isso ou aumentar o ganho no trimpot físico do sensor !!!!
float FATOR_CALIBRACAO = 730.0; 

WebServer server(80);
float tensaoRMS = 0.0;

// Calcula RMS calculando o nível DC automaticamente
float lerVoltagemRMS() {
  int N = 1000; // Número de amostras para média
  long somaLeituras = 0;
  double somaQuadrados = 0;
  
  // Descobre onde é o "Zero" do sensor agora (Auto-Offset)
  for (int i = 0; i < N; i++) {
    somaLeituras += analogRead(PINO_SENSOR);
    delayMicroseconds(5); 
  }
  float mediaDC = somaLeituras / (float)N; 

  // Calcula o RMS real usando esse Zero encontrado
  for (int i = 0; i < N; i++) {
    float leitura = analogRead(PINO_SENSOR);
    float valorInstante = leitura - mediaDC; // Remove o offset
    somaQuadrados += valorInstante * valorInstante;
    delayMicroseconds(5);
  }
  
  float media = somaQuadrados / N;
  float raiz = sqrt(media);
  float voltagem = (raiz * FATOR_CALIBRACAO) / 4095.0;

  // Controle do LED
  if (voltagem > 0.3) {
    digitalWrite(PINO_LED, HIGH); // Acende se gerar algo mínimo
  } else {
    digitalWrite(PINO_LED, LOW);
  }
  return voltagem;
}

// HTML para Navegador
void handleRoot() {
  tensaoRMS = lerVoltagemRMS();
  
  String html = "<!DOCTYPE html><html>";
  html += "<head><meta charset='UTF-8'><meta http-equiv='refresh' content='1'></head>";
  html += "<body style='font-family: sans-serif; text-align: center; margin-top: 50px;'>";
  html += "<h1>Gerador CA</h1>";
  
  if (tensaoRMS > 0.2) {
      html += "<p style='color: red; font-weight: bold;'>⚡ GERANDO...</p>";
  } else {
      html += "<p style='color: gray;'>Parado</p>";
  }

  // Mostra com 3 casas decimais
  html += "<p style='font-size: 4rem; font-weight: bold;'>" + String(tensaoRMS, 3) + " V</p>";
  html += "</body></html>";
  
  server.send(200, "text/html", html);
}

//  JSON para Python
void handleData() {
  tensaoRMS = lerVoltagemRMS();
  
  String json = "{\"v\":";
  json += String(tensaoRMS);
  json += "}";
  
  server.send(200, "application/json", json);
}

void setup() {
  Serial.begin(115200);
  
  // Configura os modos dos pinos
  pinMode(PINO_SENSOR, INPUT);
  pinMode(PINO_LED, OUTPUT);

  // Pisca o LED ao ligar para testar
  digitalWrite(PINO_LED, HIGH); delay(200);
  digitalWrite(PINO_LED, LOW); delay(200);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500); Serial.print(".");
  }
  Serial.println("\nConectado! IP: " + WiFi.localIP().toString());

  server.on("/", handleRoot);
  server.on("/dados", handleData);
  server.begin();
}

void loop() {
  server.handleClient();
}
