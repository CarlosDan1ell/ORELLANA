// Pinos do motor de passo
int Pin1 = 15;
int Pin2 = 16;
int Pin3 = 0;
int Pin4 = 33;

// Sensor de chuva
const int sensorChuvaPin = 21; // Pino conectado ao OUT do módulo de chuva

int pole1[] = {0, 0, 0, 0, 0, 1, 1, 1, 0};
int pole2[] = {0, 0, 0, 1, 1, 1, 0, 0, 0};
int pole3[] = {0, 1, 1, 1, 0, 0, 0, 0, 0};
int pole4[] = {1, 1, 0, 0, 0, 0, 0, 1, 0};

int poleStep = 0;

void setup() {
  pinMode(Pin1, OUTPUT);  
  pinMode(Pin2, OUTPUT);  
  pinMode(Pin3, OUTPUT);  
  pinMode(Pin4, OUTPUT);   

  pinMode(sensorChuvaPin, INPUT);  // NÃO usar INPUT_PULLUP no caso do módulo com relé!

  Serial.begin(115200);
  Serial.println("Sistema iniciado. Aguardando leitura do sensor de chuva...");
}

void loop() {
  int estadoSensor = digitalRead(sensorChuvaPin);  // Lê o estado do sensor de chuva
  Serial.print("Estado do sensor de chuva (GPIO 21): ");
  Serial.println(estadoSensor);  // Mostra 0 ou 1 no Serial Monitor

  // Se o relé ativar (chove), geralmente a saída DO vai para LOW (0)
  if (estadoSensor == HIGH) {
    Serial.println("🌧️ Chuva detectada! Abrindo o teto da estufa...");

    // Girar no sentido anti-horário por 30 segundos (abrir o teto)
    unsigned long tempoInicio = millis();
    while (millis() - tempoInicio < 3000) {
      poleStep++;
      driveStepper(poleStep);
      if (poleStep > 7) poleStep = 0;
      delay(10);
    }

    delay(1000);  // Pausa de 1 segundo

    Serial.println("⏳ Esperando 1 minuto antes de fechar o teto...");
    delay(1000);  // Aguarda 1 segundo

    Serial.println("🌞 Fechando o teto da estufa...");
    tempoInicio = millis();
    while (millis() - tempoInicio < 3000) {
      poleStep--;
      driveStepper(poleStep);
      if (poleStep < 0) poleStep = 7;
      delay(5);
    }

    delay(1000);  // Pausa de 1 segundo
  } else {
    Serial.println("☀️ Sem chuva. Estufa permanece fechada.");
    delay(250);  // Verifica novamente após 1 segundo
  }
}

void driveStepper(int c) {
  digitalWrite(Pin1, pole1[c]);
  digitalWrite(Pin2, pole2[c]);
  digitalWrite(Pin3, pole3[c]);
  digitalWrite(Pin4, pole4[c]);
}
