import smbus

EA = 0x08 
bus = smbus.SMBus(1)

print("Controle do LED do Arduino via I2C")
print("  1 = acender | 0 = apagar | outro valor = sair")

while True:
    entrada = input("Comando: ").strip()

    if entrada not in ("0", "1"):
        print("Encerrando programa...")
        break

    try:
        bus.write_byte(EA, int(entrada))
        print("Estado: ", entrada)
    except OSError as erro:
        print("Erro de comunicacao I2C:", erro)

bus.close()
