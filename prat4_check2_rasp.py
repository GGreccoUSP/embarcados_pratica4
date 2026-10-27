import time
import smbus

EA = 0x08
bus = smbus.SMBus(1)

print("Lendo potenciometro do Arduino via I2C (Ctrl+C para sair)")

try:
    while True:
        try:
            # Requisita 2 bytes ao Arduino (registrador 0x00 e apenas um "dummy")
            dados = bus.read_i2c_block_data(EA, 0x00, 2)

            byte_high  = dados[0]
            byte_low = dados[1]

            # Reconstrucao: desloca o byte alto 8 bits e combina com o baixo
            valor = (byte_high << 8) | byte_low

            if 0 <= valor <= 1023:
                print(f"Leitura: {valor:4d}  (high={byte_high}, low={byte_low})")
            else:
                print(f"Valor fora da faixa: {valor} -> {dados}")

        except OSError as erro:
            print("Erro de comunicacao I2C:", erro)

        time.sleep(0.3)

except KeyboardInterrupt:
    print("\nEncerrando.")
finally:
    bus.close()
