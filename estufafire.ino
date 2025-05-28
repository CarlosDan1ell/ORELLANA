#include <Wire.h>
#include <Adafruit_ADS1X15.h>
#include <DHT.h>
#include <WiFi.h>
#include <IOXhop_FirebaseESP32.h>

// === CONFIG WI-FI E FIREBASE ===
#define WIFI_SSID "BlameEllen"
#define WIFI_PASSWORD "eiwf6349"
#define FIREBASE_HOST "orellana-a5229-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "BTMzgnjtJuTNkhprtUAL7cI91rrhMGkHr7jPKyDF"

// === DHT11 ===
#define DHTPIN 5
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// === ADS1115 ===
Adafruit_ADS1115 ads;

// === Pinos motor de passo 1 ===
int Pin1_1 = 15;
int Pin2_1 = 16;
int Pin3_1 = 0;
int Pin4_1 = 33;

// === Pinos motor de passo 2 ===
int Pin1_2 = 26;
int Pin2_2 = 27;
int Pin3_2 = 32;
int Pin4_2 = 14;

// === Sensor de umidade do solo ===
int pinoSensorUmidade = 35;

// === Relé da bomba ===
int pinoRele = 18;

// === Sequência motor de passo ===
int pole1[] = {0, 0, 0, 0, 0, 1, 1, 1};
int pole2[] = {0, 0, 0, 1, 1, 1, 0, 0};
int pole3[] = {0, 1, 1, 1, 0, 0, 0, 0};
int pole4[] = {1, 1, 0, 0, 0, 0, 0, 1};
int poleStep = 0;

void setup() {
  Serial.begin(115200);
  Serial.println("Iniciando sistema...");
  Wire.begin();

  dht.begin();
  if (!ads.begin()) {
    Serial.println("Erro ao iniciar ADS1115!");
    while (1);
  }
  ads.setGain(GAIN_TWOTHIRDS);

  // Inicializa pinos motores
  int motorPins[] = {Pin1_1, Pin2_1, Pin3_1, Pin4_1, Pin1_2, Pin2_2, Pin3_2, Pin4_2};
  for (int i = 0; i < 8; i++) pinMode(motorPins[i], OUTPUT);

  pinMode(pinoSensorUmidade, INPUT);
  pinMode(pinoRele, OUTPUT);
  digitalWrite(pinoRele, LOW);

  // Conecta Wi-Fi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Conectando ao Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("\nWi-Fi conectado!");

  // Inicia Firebase
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Serial.println("Sistema iniciado.");
}

void loop() {
  float temperatura = dht.readTemperature();
  float umidadeAr = dht.readHumidity();
  int16_t ldrValue = ads.readADC_SingleEnded(0);
  float ldrVolts = ldrValue * (6.144 / 32768.0);
  int16_t chuvaValue = ads.readADC_SingleEnded(1);
  int umidadeSolo = analogRead(pinoSensorUmidade);

  // Luminosidade em % (map simples)
  int lumPct = map(ldrValue, 0, 32767, 0, 100);
  lumPct = constrain(lumPct, 0, 100);

  // === ENVIO AO FIREBASE ===
  Firebase.setFloat("/Sensores/Temperatura", isnan(temperatura) ? 0 : temperatura);
  Firebase.setFloat("/Sensores/Umidade", isnan(umidadeAr) ? 0 : umidadeAr);
  Firebase.setInt("/Sensores/Luminosidade", lumPct);
  Firebase.setInt("/Sensores/SensorDeChuva", chuvaValue);
  Firebase.setInt("/Sensores/SensorDeUmidadeDoSolo", umidadeSolo);

  // === Lógica de irrigação ===
  if (umidadeSolo <= 3500) {
    Serial.println("Solo úmido. Nada a fazer.");
    digitalWrite(pinoRele, LOW);
  } else {
    if (chuvaValue > 10000) {
      Serial.println("🌧️ Solo seco e CHOVENDO → usando chuva...");
      abrirTeto(3000);
      delay(3000);
      fecharTeto(3000);
      Serial.println("Irrigação com chuva finalizada.");
    } else {
      Serial.println("☀️ Solo seco e SEM CHUVA → ativando bomba...");
      digitalWrite(pinoRele, HIGH);
      delay(3000);
      digitalWrite(pinoRele, LOW);
      Serial.println("🚿 Irrigação finalizada com bomba.");
    }
  }

  // Serial Monitor
  Serial.println("=== DADOS ===");
  Serial.print("Temperatura: "); Serial.println(temperatura);
  Serial.print("Umidade do Ar: "); Serial.println(umidadeAr);
  Serial.print("Umidade do Solo: "); Serial.println(umidadeSolo);
  Serial.print("Chuva: "); Serial.println(chuvaValue);
  Serial.print("Luminosidade (V): "); Serial.println(ldrVolts, 4);
  Serial.print("Luminosidade (%): "); Serial.println(lumPct);
  Serial.println("==================");

  delay(5000);
}

void abrirTeto(int tempo) {
  unsigned long inicio = millis();
  while (millis() - inicio < tempo) {
    poleStep++;
    if (poleStep > 7) poleStep = 0;
    driveSteppers(poleStep);
    delay(10);
  }
}

void fecharTeto(int tempo) {
  unsigned long inicio = millis();
  while (millis() - inicio < tempo) {
    poleStep--;
    if (poleStep < 0) poleStep = 7;
    driveSteppers(poleStep);
    delay(10);
  }
}

void driveSteppers(int c) {
  digitalWrite(Pin1_1, pole1[c]);
  digitalWrite(Pin2_1, pole2[c]);
  digitalWrite(Pin3_1, pole3[c]);
  digitalWrite(Pin4_1, pole4[c]);

  digitalWrite(Pin1_2, pole1[c]);
  digitalWrite(Pin2_2, pole2[c]);
  digitalWrite(Pin3_2, pole3[c]);
  digitalWrite(Pin4_2, pole4[c]);
}
