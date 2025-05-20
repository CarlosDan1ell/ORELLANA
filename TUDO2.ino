#include <Wire.h>
#include <Adafruit_ADS1X15.h>
#include <DHT.h>

// === DHT11 ===
#define DHTPIN 5
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// === ADS1115 ===
Adafruit_ADS1115 ads;

// === Pinos do motor de passo ===
int Pin1 = 15;
int Pin2 = 16;
int Pin3 = 0;
int Pin4 = 33;

// === Sensor de chuva ===
const int sensorChuvaPin = 23; // OUT do módulo de chuva (LOW quando chove) não usa

// === Sensor de umidade do solo ===
int pinoSensorUmidade = 4;

// === Relé da bomba de irrigação ===
int pinoRele = 18;

int pole1[] = {0, 0, 0, 0, 0, 1, 1, 1, 0};
int pole2[] = {0, 0, 0, 1, 1, 1, 0, 0, 0};
int pole3[] = {0, 1, 1, 1, 0, 0, 0, 0, 0};
int pole4[] = {1, 1, 0, 0, 0, 0, 0, 1, 0};
int poleStep = 0;

void setup() {
  Serial.begin(115200);
   Wire.begin();

 // === Inicialização dos sensores adicionais ===
  if (!ads.begin()) {
    Serial.println("Erro ao inicializar ADS1115!");
    while (1);
  }
  ads.setGain(GAIN_TWOTHIRDS);
  dht.begin();

  pinMode(Pin1, OUTPUT);  
  pinMode(Pin2, OUTPUT);  
  pinMode(Pin3, OUTPUT);  
  pinMode(Pin4, OUTPUT);  

  pinMode(sensorChuvaPin, INPUT);  
  pinMode(pinoSensorUmidade, INPUT);
  pinMode(pinoRele, OUTPUT);
  digitalWrite(pinoRele, LOW); // bomba desligada inicialmente

  Serial.println("Sistema iniciado.");
}

void loop() {

// === Leitura de sensores adicionais ===
  float humidityAr = dht.readHumidity();
  float temperature = dht.readTemperature();
  int16_t ldrValue = ads.readADC_SingleEnded(0);
  float voltage = ldrValue * (6.144 / 32768.0);

  Serial.print("Temperatura (°C): ");
  Serial.println(isnan(temperature) ? 0 : temperature);
  Serial.print("Umidade do Ar (%): ");
  Serial.println(isnan(humidityAr) ? 0 : humidityAr);
  Serial.print("Luminosidade (V): ");
  Serial.println(voltage, 4);

  // === Leitura dos sensores ===
  int leituraUmidade = analogRead(pinoSensorUmidade);
  int estadoChuva = digitalRead(sensorChuvaPin); // LOW = chovendo

  Serial.print("Umidade do solo: ");
  Serial.print(leituraUmidade);
  Serial.print(" | Chuva: ");
  Serial.println(estadoChuva == LOW ? "Não" : "Sim");

  if (leituraUmidade <= 4000) {
    Serial.println("Solo úmido. Nada a fazer. 😌");
    digitalWrite(pinoRele, LOW); // Garante que bomba esteja desligada
  } else {
    // Solo seco
    if (estadoChuva == HIGH) {
      // Está chovendo → usar a chuva (abrir teto)
      Serial.println("🌧️ Solo seco e CHOVENDO → usando irrigação natural...");
      abrirTeto(3000);
      delay(3000); // tempo para a chuva molhar o solo
      fecharTeto(3000);
      Serial.println("🌿 Irrigação com chuva finalizada.");
    } else {
      // Sem chuva → usar bomba
      Serial.println("☀️ Solo seco e SEM CHUVA → ativando bomba...");
      digitalWrite(pinoRele, HIGH);
      delay(3000); // tempo de irrigação
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
    driveStepper(poleStep);
    delay(10);
  }
}

void fecharTeto(int tempo) {
  unsigned long inicio = millis();
  while (millis() - inicio < tempo) {
    poleStep--;
    if (poleStep < 0) poleStep = 7;
    driveStepper(poleStep);
    delay(10);
  }
}

void driveStepper(int c) {
  digitalWrite(Pin1, pole1[c]);
  digitalWrite(Pin2, pole2[c]);
  digitalWrite(Pin3, pole3[c]);
  digitalWrite(Pin4, pole4[c]);
}
