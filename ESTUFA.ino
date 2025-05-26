#include <Wire.h>
#include <Adafruit_ADS1X15.h>
#include <DHT.h>

// === DHT11 ===
#define DHTPIN 5
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// === ADS1115 ===
Adafruit_ADS1115 ads;

// === Pinos do motor de passo 1 ===
int Pin1_1 = 15;
int Pin2_1 = 16;
int Pin3_1 = 0;
int Pin4_1 = 33;

// === Pinos do motor de passo 2 ===
int Pin1_2 = 26;
int Pin2_2 = 27;
int Pin3_2 = 32;
int Pin4_2 = 14;

// === Sensor de umidade do solo ===
int pinoSensorUmidade = 4;

// === Relé da bomba de irrigação ===
int pinoRele = 18;

// === Sequência para motor de passo ===
int pole1[] = {0, 0, 0, 0, 0, 1, 1, 1};
int pole2[] = {0, 0, 0, 1, 1, 1, 0, 0};
int pole3[] = {0, 1, 1, 1, 0, 0, 0, 0};
int pole4[] = {1, 1, 0, 0, 0, 0, 0, 1};
int poleStep = 0;

void setup() {
  Serial.begin(115200);
  Wire.begin();

  if (!ads.begin()) {
    Serial.println("Erro ao inicializar ADS1115!");
    while (1);
  }
  ads.setGain(GAIN_TWOTHIRDS);
  dht.begin();

  // Inicializa pinos dos dois motores
  int motorPins[] = {Pin1_1, Pin2_1, Pin3_1, Pin4_1, Pin1_2, Pin2_2, Pin3_2, Pin4_2};
  for (int i = 0; i < 8; i++) pinMode(motorPins[i], OUTPUT);

  pinMode(pinoSensorUmidade, INPUT);
  pinMode(pinoRele, OUTPUT);
  digitalWrite(pinoRele, LOW);

  Serial.println("Sistema iniciado.");
}

void loop() {
  float humidityAr = dht.readHumidity();
  float temperature = dht.readTemperature();
  int16_t ldrValue = ads.readADC_SingleEnded(0);
  float voltage = ldrValue * (6.144 / 32768.0);
  int16_t chuvaValue = ads.readADC_SingleEnded(1); // Sensor de chuva no canal A1

  Serial.print("Temperatura (°C): ");
  Serial.println(isnan(temperature) ? 0 : temperature);
  Serial.print("Umidade do Ar (%): ");
  Serial.println(isnan(humidityAr) ? 0 : humidityAr);
  Serial.print("Luminosidade (V): ");
  Serial.println(voltage, 4);
  Serial.print("Sensor de chuva (A1): ");
  Serial.println(chuvaValue);

  int leituraUmidade = analogRead(pinoSensorUmidade);
  Serial.print("Umidade do solo: ");
  Serial.println(leituraUmidade);

  if (leituraUmidade <= 3500) {
    Serial.println("Solo úmido. Nada a fazer. 😌");
    digitalWrite(pinoRele, LOW);
  } else {
    if (chuvaValue < 10000) {
      Serial.println("🌧️ Solo seco e CHOVENDO → usando irrigação natural...");
      abrirTeto(3000);
      delay(3000);
      fecharTeto(3000);
      Serial.println("🌿 Irrigação com chuva finalizada.");
    } else {
      Serial.println("☀️ Solo seco e SEM CHUVA → ativando bomba...");
      digitalWrite(pinoRele, HIGH);
      delay(3000);
      digitalWrite(pinoRele, LOW);
      Serial.println("🚿 Irrigação finalizada com bomba.");
    }
  }

  delay(1000);
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
