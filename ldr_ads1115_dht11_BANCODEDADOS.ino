#include <Wire.h>
#include <Adafruit_ADS1X15.h>
#include <DHT.h>
#include <WiFi.h>
#include <IOXhop_FirebaseESP32.h>

// === CONFIG WI-FI E FIREBASE ===
#define WIFI_SSID "TURBO_SALES 2.4G"
#define WIFI_PASSWORD "DEUSEFIEL99322"
#define FIREBASE_HOST "orellana-a5229-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "BTMzgnjtJuTNkhprtUAL7cI91rrhMGkHr7jPKyDF"

// === DHT11 ===
#define DHTPIN 5
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// === ADS1115 ===
Adafruit_ADS1115 ads;

void setup() {
  Serial.begin(115200);
  Wire.begin();
  dht.begin();

  if (!ads.begin()) {
    Serial.println("Erro ao iniciar ADS1115!");
    while (1);
  }
  ads.setGain(GAIN_TWOTHIRDS);

  // Conectar ao Wi-Fi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Conectando ao Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("\nWi-Fi conectado!");

  // Iniciar Firebase
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Serial.println("Sistema iniciado.");
}

void loop() {
  // Leitura dos sensores
  float temperatura = dht.readTemperature();
  float umidade = dht.readHumidity();
  int16_t ldrValue = ads.readADC_SingleEnded(0);
  float luminosidadeVolts = ldrValue * (6.144 / 32768.0); // em volts

// === Calibração prática do LDR ===
const int LDR_MIN = 500;    // valor com luz máxima (lanterna)
const int LDR_MAX = 15000;  // valor no escuro (mão em cima)

  // Calcular luminosidade em % (0% = escuridão, 100% = luz forte)
int luminosidadePercentual = map(ldrValue, 0, 32767, 0, 100);
if (luminosidadePercentual < 0) luminosidadePercentual = 0;
if (luminosidadePercentual > 100) luminosidadePercentual = 100;

  // Enviar para Firebase
  Firebase.setFloat("/Sensores/DHT11/Temperatura", isnan(temperatura) ? 0 : temperatura);
  Firebase.setFloat("/Sensores/DHT11/Umidade", isnan(umidade) ? 0 : umidade);
  Firebase.setInt("/Sensores/Luminosidade", luminosidadePercentual);

  // Serial Monitor
  Serial.println("=== Dados enviados ===");
  Serial.print("Temperatura: "); Serial.println(temperatura);
  Serial.print("Umidade: "); Serial.println(umidade);
  Serial.print("Luminosidade (V): "); Serial.println(luminosidadeVolts, 4);
  Serial.print("Luminosidade (%): "); Serial.println(luminosidadePercentual);
  Serial.println("======================");

  delay(5000); // esperar 5s
}
