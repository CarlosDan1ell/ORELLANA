// Pinos do motor de passo
int Pin1 = 17;  // IN1 ligado ao GPIO17
int Pin2 = 18;  // IN2 ligado ao GPIO18  
int Pin3 = 19;  // IN3 ligado ao GPIO19  
int Pin4 = 21;  // IN4 ligado ao GPIO21

// Sensor de chuva
const int sensorChuvaPin = 16; // Pino do sensor de chuva
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
  pinMode(sensorChuvaPin, INPUT_PULLUP);  // Configura o sensor de chuva

  Serial.begin(115200);
}

void loop() {
  int estadoSensor = digitalRead(sensorChuvaPin);  // Lê o estado do sensor de chuva

  // Se chover (sensor de chuva detecta água)
  if (estadoSensor == LOW) {  
    Serial.println("🌧️ Chuva detectada! Abrindo o teto da estufa...");
    
    // Girar no sentido anti-horário por 30 segundos (abrir o teto)
    unsigned long tempoInicio = millis();
    while (millis() - tempoInicio < 30000) {  // 30000 ms = 30 segundos
      poleStep++;
      driveStepper(poleStep);
      if (poleStep > 7) poleStep = 0;  // Recomeçar a sequência após 7 passos
      delay(10); // Ajuste de velocidade
    }

    delay(1000);  // Pausa de 1 segundo antes de mudar de direção

    Serial.println("⏳ Esperando 1 minuto antes de fechar o teto...");

    // Espera de 1 minuto (60 segundos)
    delay(60000);  

    Serial.println("🌞 Fechando o teto da estufa...");

    // Girar no sentido horário por 30 segundos (fechar o teto)
    tempoInicio = millis();
    while (millis() - tempoInicio < 30000) {  // 30000 ms = 30 segundos
      poleStep--;
      driveStepper(poleStep);
      if (poleStep < 0) poleStep = 7;  // Recomeçar a sequência após passo 0
      delay(5); // Ajuste de velocidade
    }

    delay(1000);  // Pausa de 1 segundo antes de verificar novamente
  } 
  else {
    Serial.println("☀️ Sem chuva. Estufa em modo fechado.");
    delay(1000);  // Verifica o sensor a cada 1 segundo
  }
}

void driveStepper(int c) {
  digitalWrite(Pin1, pole1[c]);
  digitalWrite(Pin2, pole2[c]);
  digitalWrite(Pin3, pole3[c]);
  digitalWrite(Pin4, pole4[c]);
}
