// Autor: Vitor Augusto Tibério 
// Estudante de Engenharia Elétrica - Universidade de São Paulo (USP)

// Incluindo as Bibliotecas // 

#include <WiFi.h>
#include <WebServer.h>
#include <DHT.h>

// Configurações da Rede Wi-Fi // 
const char* ssid = "XXXXXX";
const char* password = "XXXXXXX";

// Configuração do Sensor DHT // 
#define DHTPIN 4      // Pino conectado ao DATA do sensor
#define DHTTYPE DHT11 // Define o modelo do sensor (DHT11 ou DHT22)

DHT dht(DHTPIN, DHTTYPE);
WebServer server(80);

// Variáveis para armazenar os dados // 
float temperatura;
float umidade;

// Função para ler o sensor // 
void readSensor() {
  temperatura = dht.readTemperature(); // Lê temperatura em Celsius
  umidade = dht.readHumidity();        // Lê umidade em %
  
  // Verifica se houve falha na leitura
  if (isnan(temperatura) || isnan(umidade)) {
    Serial.println("Falha ao ler do sensor DHT!");
    return; 
  }
}

// Função que monta a página HTML // 
void handleRoot() {
  readSensor(); // Atualiza os dados antes de enviar a página
  
  String html = "<!DOCTYPE html><html>";
  html += "<head>";
  html += "<meta charset='UTF-8'>"; // Para aceitar acentos
  html += "<meta http-equiv='refresh' content='2'>"; // Atualiza a página a cada 2 seg
  html += "<style>body{font-family: sans-serif; text-align: center; margin-top: 50px;} h1{font-size: 2.5rem;} p{font-size: 1.5rem;}</style>";
  html += "</head>";
  html += "<body>";
  html += "<h1>Monitoramento Ambiental (ESP32)</h1>";
  
  html += "<p>🌡️ Temperatura: <strong>" + String(temperatura) + " &deg;C</strong></p>";
  html += "<p>💧 Umidade: <strong>" + String(umidade) + " %</strong></p>";
  
  html += "</body></html>";
  
  server.send(200, "text/html", html);
}

void setup() {
  Serial.begin(115200);
  
  // Inicia o sensor
  dht.begin();

  // Conecta no Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Conectando");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.print("Conectado! IP: ");
  Serial.println(WiFi.localIP());

  // Define a rota principal
  server.on("/", handleRoot);
  server.begin();
}

void loop() {
  server.handleClient();
}
