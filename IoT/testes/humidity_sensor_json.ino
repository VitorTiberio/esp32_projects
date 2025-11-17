// Autor: Vitor Augusto Tibério - Estudante de Engenharia Elétrica - USP São Carlos

#include <WiFi.h>
#include <WebServer.h>
#include <DHT.h>

// Configuração do Wi-fi // 
const char* ssid = "XXXXXXX";      
const char* password = "XXXXXXX"; 

// Configurando o DHT11 
#define DHTPIN 4       // Pino conectado ao DATA do sensor
#define DHTTYPE DHT11  // Modelo do sensor

DHT dht(DHTPIN, DHTTYPE);
WebServer server(80);

// Variáveis globais para armazenar os valores
float temperatura = 0.0;
float umidade = 0.0;

// Função para leitura do sensor DHT11 // 
void readSensor() {
  float t = dht.readTemperature();
  float h = dht.readHumidity();

  // Só atualiza as variáveis globais se a leitura for válida
  if (!isnan(t) && !isnan(h)) {
    temperatura = t;
    umidade = h;
  } else {
    Serial.println("Falha na leitura do sensor!");
  }
}

// Criação da rota HTML // 
void handleRoot() {
  readSensor(); 
  
  String html = "<!DOCTYPE html><html>";
  html += "<head><meta charset='UTF-8'><meta http-equiv='refresh' content='2'></head>";
  html += "<body style='font-family: sans-serif; text-align: center; margin-top: 50px;'>";
  html += "<h1>Monitoramento ESP32</h1>";
  html += "<p>🌡️ Temperatura: <strong>" + String(temperatura) + " &deg;C</strong></p>";
  html += "<p>💧 Umidade: <strong>" + String(umidade) + " %</strong></p>";
  html += "<p><small>Para dados brutos acesse: <a href='/dados'>/dados</a></small></p>";
  html += "</body></html>";
  
  server.send(200, "text/html", html);
}

// Criação da rota JSON (para plot do gráfico em python) 
void handleData() {
  readSensor();
  
  // Monta o JSON manualmente: {"t": 25.5, "h": 60.0}
  String json = "{\"t\":";
  json += String(temperatura);
  json += ",\"h\":";
  json += String(umidade);
  json += "}";
  
  // Envia com o tipo "application/json"
  server.send(200, "application/json", json);
}

void setup() {
  Serial.begin(115200);
  dht.begin();

  // Conexão Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Conectando ao WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.print("Conectado! IP: ");
  Serial.println(WiFi.localIP());

  // Define as rotas
  server.on("/", handleRoot);      // Rota padrão
  server.on("/dados", handleData); // Rota de dados para o Python

  server.begin();
}

void loop() {
  server.handleClient(); // Mantém o servidor atendendo requisições
}
