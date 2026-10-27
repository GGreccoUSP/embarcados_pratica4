#include <Wire.h>

#define ENDERECO_I2C 0x08

volatile int  comandoRecebido = -1;   
volatile bool novoComando     = false; 


void receberDados(int numBytes) {
  int valor = -1;
  while (Wire.available()) {
    valor = Wire.read();   
                          
  }
  comandoRecebido = valor;
  novoComando = true;
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  Serial.begin(9600);
  Wire.begin(ENDERECO_I2C);        
  Wire.onReceive(receberDados);   

  Serial.println("Arduino pronto - I2C no endereco 0x08");
}

void loop() {
  // Serial e LED sao tratados fora da interrupcao
  if (novoComando) {
    int cmd = comandoRecebido;
    novoComando = false;

    if (cmd == 1) {
      digitalWrite(LED_BUILTIN, HIGH);
      Serial.println("Comando recebido: 1 -> LED ACESO");
    } else if (cmd == 0) {
      digitalWrite(LED_BUILTIN, LOW);
      Serial.println("Comando recebido: 0 -> LED APAGADO");
    } else {
      Serial.print("Comando desconhecido: ");
      Serial.println(cmd);
    }
  }
}
