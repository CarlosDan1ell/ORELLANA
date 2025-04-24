int pinoSensorUmidade = 4; 
int pinoRele = 18; // Pino conectado ao relé

void setup() {
  Serial.begin(9600);
  pinMode(pinoSensorUmidade, INPUT);
  pinMode(pinoRele, OUTPUT);
  digitalWrite(pinoRele, LOW); // Relé desligado inicialmente
}

void loop() {
  int leitura = analogRead(pinoSensorUmidade);
  float tensao = leitura * (3.3 / 4095.0); // converte para tensão (ESP32)

  Serial.print("Leitura bruta: ");
  Serial.print(leitura);
  Serial.print(" | Tensão: ");
  Serial.print(tensao);
  Serial.println(" V");

  if (leitura >= 4000) {
    digitalWrite(pinoRele, HIGH); // Liga o relé
    Serial.println(">>> Relé ATIVADO!");
  } else {
    digitalWrite(pinoRele, LOW);  // Desliga o relé
    Serial.println("Relé DESLIGADO.");
  }

  delay(1000);
}
