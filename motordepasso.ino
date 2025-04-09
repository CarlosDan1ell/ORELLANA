int Pin1 = 17;  // IN1 ligado ao GPIO17
int Pin2 = 18;  // IN2 ligado ao GPIO18  
int Pin3 = 19;  // IN3 ligado ao GPIO19  
int Pin4 = 21;  // IN4 ligado ao GPIO21

int pole1[] = {0,0,0,0, 0,1,1,1, 0};
int pole2[] = {0,0,0,1, 1,1,0,0, 0};
int pole3[] = {0,1,1,1, 0,0,0,0, 0};
int pole4[] = {1,1,0,0, 0,0,0,1, 0};

int poleStep = 0; 

void setup() { 
 pinMode(Pin1, OUTPUT);  
 pinMode(Pin2, OUTPUT);  
 pinMode(Pin3, OUTPUT);  
 pinMode(Pin4, OUTPUT);   
} 

void loop() { 
  // Girar no sentido horário por 10 segundos
  unsigned long tempoInicio = millis();
  while(millis() - tempoInicio < 60000){ 
    poleStep++; 
    driveStepper(poleStep);
    if(poleStep > 7) poleStep = 0;
    delay(5); // velocidade ajustável
  }

  delay(1000); // Pausa de 1 segundo
  
  // Girar no sentido anti-horário por 10 segundos
  tempoInicio = millis();
  while(millis() - tempoInicio < 10000){ 
    poleStep--; 
    driveStepper(poleStep);
    if(poleStep < 0) poleStep = 7;
    delay(10); // velocidade ajustável
  }

  delay(1000); // Pausa de 1 segundo
}

void driveStepper(int c){
 digitalWrite(Pin1, pole1[c]);  
 digitalWrite(Pin2, pole2[c]); 
 digitalWrite(Pin3, pole3[c]); 
 digitalWrite(Pin4, pole4[c]);   
}
