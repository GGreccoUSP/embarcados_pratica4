# SEL0337 - Prática 4: Protocolos de comunicação em sistemas embarcados de alto nível

-- Davi Andrade Laet - 15697361

-- Giovanne Tomaszewski Grecco - 16228852


A prática foi programada em python e C++, e implementada em Raspberry Pi e Arduino, respectivamente. Além disso, a integração entre as duas placas foi feita a partir de um módulo de conexão I2C e uma protoboard. Para o checkpoint 2 também utilizou-se um potenciômetro.

## Checkpoint 1: controle do LED

Arduino (cp1_arduino.ino): ele entra no barramento I2C como responder no endereço 0x08 e fica esperando dados. Quando a Raspberry envia um byte, a função receberDados (acionada por interrupção) guarda o valor e levanta uma flag. O loop() percebe a flag, acende o LED_BUILTIN se o valor for 1, apaga se for 0 e imprime o comando recebido no monitor serial.

Raspberry (cp1_raspberry.py): ela atua como controller. O programa lê o que o usuário digita no terminal e, se for 0 ou 1, envia o valor ao endereço 0x08 com bus.write_byte. Qualquer outra entrada encerra o programa.

Integração: o fluxo é de um sentido só, Raspberry → Arduino. O que liga os dois códigos é o endereço 0x08, que precisa ser igual nos dois, e o protocolo combinado, em que um byte 1 significa acender e 0 apagar. O level-shifter faz a ponte física entre os 3,3 V da Raspberry e os 5 V do Arduino. O LED e o monitor serial servem como prova visual de que o dado chegou.

## Checkpoint 2: leitura do potenciômetro

Arduino (cp2_arduino.ino): o fluxo agora é de resposta, não de recepção. Quando a Raspberry faz uma requisição, a função aoRequisitar roda analogRead(A0), que devolve um valor de 0 a 1023 em uint16_t. Como o I2C transmite 8 bits por vez, o valor é dividido em dois bytes: highByte (os 2 bits mais significativos) e lowByte (os 8 bits menos significativos). Os dois são enviados juntos, nessa ordem. O valor enviado também é impresso no monitor serial.

Raspberry (cp2_raspberry.py): a cada 0,3 s ela requisita 2 bytes ao Arduino com read_i2c_block_data. Depois reconstrói o valor original com (byte_alto << 8) | byte_baixo, confere se está entre 0 e 1023 e imprime no terminal.

Integração: o fluxo inverteu e passou a ser Arduino → Raspberry, iniciado por uma requisição da própria Raspberry. A integração depende de três coisas combinadas entre os códigos:

Mesmo endereço, 0x08.
Mesma ordem dos bytes: o Arduino manda o alto primeiro, e a Raspberry o trata como alto ao deslocá-lo 8 bits.
Mesmo tamanho: 2 bytes enviados e 2 bytes lidos.
