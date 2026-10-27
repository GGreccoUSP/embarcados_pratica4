#include <Wire.h>

#define ENDERECO_I2C 0x08
#define PINO_POT     A0

volatile uint16_t ultimoValorEnviado = 0;
volatile bool     novoEnvio          = false;

void aoRequisitar() {
  uint16_t leitura = analogRead(PINO_POT);   
  uint8_t buffer[2];
  buffer[0] = highByte(leitura);   // 1o byte: bits 9..8  (0 a 3)
  buffer[1] = lowByte(leitura);    // 2o byte: bits 7..0  (0 a 255)

  Wire.write(buffer, 2);           

  ultimoValorEnviado = leitura;    
  novoEnvio = true;
}

void aoReceber(int numBytes) {
  while (Wire.available()) Wire.read();
}

void setup() {
  Serial.begin(9600);
  Wire.begin(ENDERECO_I2C);
  Wire.onRequest(aoRequisitar);
  Wire.onReceive(aoReceber);
  Serial.println("Arduino pronto - leitura do potenciometro via I2C");
}

void loop() {
  if (novoEnvio) {
    uint16_t v = ultimoValorEnviado;
    novoEnvio = false;
    Serial.print("Enviado: ");
    Serial.print(v);
    Serial.print("  (high=");
    Serial.print(highByte(v));
    Serial.print(", low=");
    Serial.print(lowByte(v));
    Serial.println(")");
  }
}
