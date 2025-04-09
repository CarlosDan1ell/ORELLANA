const int LDRPin = 34;          // Pino analógico onde o LDR está conectado
const int numReadings = 10;     // Número de leituras para fazer a média
int readings[numReadings];      // Array para armazenar as leituras
int currentIndex = 0;           // Índice para controle das leituras
int total = 0;
int average = 0;

void setup() {
  Serial.begin(115200);         // Inicializa a comunicação serial
  analogReadResolution(12);     // Ajusta a resolução do ADC para 12 bits (0-4095)

  // Inicializa o array de leituras
  for (int i = 0; i < numReadings; i++) {
    readings[i] = 0;
  }
}

void loop() {
  // Subtrai a leitura mais antiga
  total = total - readings[currentIndex];
  
  // Faz a leitura do LDR
  readings[currentIndex] = analogRead(LDRPin);
  
  // Adiciona a nova leitura ao total
  total = total + readings[currentIndex];
  
  // Avança para o próximo índice do array
  currentIndex = currentIndex + 1;
  
  // Se o índice for maior que o número de leituras, reseta para 0
  if (currentIndex >= numReadings) {
    currentIndex = 0;
  }
  
  // Calcula a média
  average = total / numReadings;
  
  // Exibe o valor médio no monitor serial
  Serial.print("LDR Value (Filtered): ");
  Serial.println(average);
  
  delay(500);  // Aguarda 500ms antes de fazer outra leitura
}
