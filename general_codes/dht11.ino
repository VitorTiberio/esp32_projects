// Developed by Vitor Augusto Tiberio - Undergraduate Student at University of São Paulo 
// Electrical Engineering Department 

# include <DHT.h>

#define DHTPIN 4 
#define DHTTYPE DHT11 

DHT dht(DHTPIN, DHTTYPE);

void setup(){
  Serial.begin(115200);

  dht.begin();

  Serial.println('Teste do sensor DHT11 iniciado...');
}

void loop(){
  delay(2000);
  float umidade = dht.readHumidity();
  float temperatura = dht.readTemperature();

  if (isnan(umidade) || isnan(temperatura)){
    Serial.println("Falha ao ler o sensor!");
    return; 
  }

  Serial.print("Umidade: ");
  Serial.print(umidade);
  Serial.println(" %");
  Serial.print("Temperatura: ");
  Serial.print(temperatura);
  Serial.println(" *C");

  Serial.println("----------------------");
}
