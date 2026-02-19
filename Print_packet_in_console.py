
#!!!download sketch "read1byte.ino"!!!


import serial
import time

ser = serial.Serial(
    port='COM14',
    baudrate=112500, #if use ch340 -> baudrate 27 777 
    timeout=0.01   # маленький таймаут, чтобы не блокироваться
)

print("Порт открыт. Приём пакетов... (Ctrl+C для остановки)")

PACKET_SIZE = 8
buffer = bytearray()
last_print_time = 0

try:
    while True:
        # читаем всё, что есть в буфере
        data = ser.read(64)
        if data:
            buffer.extend(data)

        # ищем пакеты
        while len(buffer) >= PACKET_SIZE:
            # синхронизация по нулевому байту
            if buffer[0] != 0:
                buffer.pop(0)
                continue

            packet = buffer[:PACKET_SIZE]
            buffer = buffer[PACKET_SIZE:]

            now = time.time()
            if now - last_print_time >= 0.1:  # 100 мс
                print(" ".join(str(b) for b in packet))
                last_print_time = now

        time.sleep(0.001)  # микропауза, CPU скажет спасибо

except KeyboardInterrupt:
    ser.close()
    print("Приём остановлен.")
