#include <WiFi.h>
#include <WebServer.h>

// --- CONFIGURAÇÕES DE REDE ---
const char* ssid = "NOME_DA_SUA_REDE";      
const char* password = "SENHA_DA_SUA_REDE"; 

// CONFIGURAÇÕES DO SENSOR ZMPT101B 
#define PINO_SENSOR 34   // GPIO 34 (Entrada Analógica)
float FATOR_CALIBRACAO = 730.0; // AJUSTE AQUI: Fator para converter a leitura analógica em Volts reais. Se o valor lido for menor que o multímetro, AUMENTE este número. Se for maior, diminua.

WebServer server(80);

// Variável global
float tensaoRMS = 0.0;

// Função que calcula a Tensão RMS
float lerVoltagemRMS() {
  float somaQuadrados = 0.0;
  int amostras = 0;
  unsigned long tempoInicio = millis();
  
  // Lê o sensor por 100ms (cobre cerca de 6 ciclos de 60Hz)
  while (millis() - tempoInicio < 100) {
    // Lê o valor analógico (0 a 4095)
    int leitura = analogRead(PINO_SENSOR);
    
    // O ZMPT101B (em 3.3V) centraliza a onda em ~1.65V (aprox leitura 1800-2000)
    // Precisamos remover esse "offset" para pegar só a oscilação
    // Um valor fixo aproximado de offset é 1850 (ajuste se necessário)
    float valorInstante = leitura - 1850; 
    
    somaQuadrados += valorInstante * valorInstante;
    amostras++;
    delayMicroseconds(200); // Pequena pausa para estabilidade
  }
  
  // Cálculo da média quadrática (RMS matemático)
  float media = somaQuadrados / amostras;
  float raiz = sqrt(media);
  
  // Converte para Volts usando o fator de calibração
  // A lógica é: (Leitura ADC * Fator) / Resolução Máxima
  return (raiz * FATOR_CALIBRACAO) / 4095.0;
}

// ---  Página HTML ---
void handleRoot() {
  // Lê a tensão atual
  tensaoRMS = lerVoltagemRMS();
  
  // Pequeno filtro para zerar ruído se for muito baixo
  if (tensaoRMS < 5.0) tensaoRMS = 0.0;

  String html = "<!DOCTYPE html><html>";
  html += "<head><meta charset='UTF-8'><meta http-equiv='refresh' content='2'></head>";
  html += "<body style='font-family: sans-serif; text-align: center; margin-top: 50px;'>";
  html += "<h1>Gerador CA Inteligente</h1>";
  html += "<h2>Monitoramento de Tensão</h2>";
  
  // Mostra o valor bem grande
  html += "<p style='font-size: 4rem; font-weight: bold;'>" + String(tensaoRMS, 1) + " V</p>";
  
  html += "<p><small>Tensão RMS (Rede)</small></p>";
  html += "</body></html>";
  
  server.send(200, "text/html", html);
}

// --- JSON  ---
void handleData() {
  tensaoRMS = lerVoltagemRMS();
  if (tensaoRMS < 5.0) tensaoRMS = 0.0;
  
  // JSON simples: {"v": 127.5}
  String json = "{\"v\":";
  json += String(tensaoRMS);
  json += "}";
  
  server.send(200, "application/json", json);
}

void setup() {
  Serial.begin(115200);
  pinMode(PINO_SENSOR, INPUT);

  WiFi.begin(ssid, password);
  Serial.print("Conectando ao WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/dados", handleData);
  server.begin();
}

void loop() {
  server.handleClient();
}
