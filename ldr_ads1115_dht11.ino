#include <Wire.h>
#include <Adafruit_ADS1X15.h>
#include <DHT.h>

// Defina o pino para o DHT11 e o tipo de sensor
#define DHTPIN 23         // Pino do DHT11
#define DHTTYPE DHT11    // Tipo do sensor

// Crie o objeto para o ADS1115 e o sensor DHT11
Adafruit_ADS1115 ads;  
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);        // Inicializa a comunicação serial
  Wire.begin();                // Inicializa a comunicação I2C
  
  // Inicializa o ADS1115
  if (!ads.begin()) {
    Serial.println("Erro ao inicializar o ADS1115. Verifique a conexão.");
    while (1);
  }
  
  dht.begin();                 // Inicializa o sensor DHT11
  
  ads.setGain(GAIN_TWOTHIRDS); // Define o ganho para +/- 6.144V
}

void loop() {
  // Leitura da luminosidade (LDR)
  int16_t ldrValue = ads.readADC_SingleEnded(0);  // Canal A0 do ADS1115
  float voltage = ldrValue * (6.144 / 32768.0);   // Converte a leitura para voltagem
  
  // Leitura da temperatura e umidade do DHT11
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  
  // Verifica se a leitura do DHT11 falhou
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Falha ao ler o sensor DHT!");
  } else {
    // Exibe os valores no monitor serial
    Serial.print("Luminosidade (V): ");
    Serial.println(voltage, 4);  // Exibe o valor da luminosidade em Volts
    Serial.print("Temperatura (°C): ");
    Serial.println(temperature);
    Serial.print("Umidade (%): ");
    Serial.println(humidity);
  }
  
  delay(1000);  // Aguarda 2 segundos antes de fazer outra leitura
}
