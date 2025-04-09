// Define o pino analógico onde o LDR está conectado
const int LDRPin = 34;  // GPIO34 é uma entrada analógica do ESP32
int LDRValue = 0;  // Variável para armazenar o valor lido do LDR

void setup() {
  // Inicializa a comunicação serial para visualizar os dados
  Serial.begin(115200);  // 115200 é a taxa de comunicação comum para ESP32
}

void loop() {
  // Lê o valor do LDR (valor de 0 a 4095)
  LDRValue = analogRead(LDRPin); 
  
  // Envia o valor para o monitor serial
  Serial.print("LDR Value: ");
  Serial.println(LDRValue);
  
  // Aguarda 500 milissegundos antes de ler novamente
  delay(500);
}

