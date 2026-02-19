# download sketch "resad1byte.ino" or use ch340

import serial
import csv
import time

ser = serial.Serial(
    port='COM14',
    baudrate=115200,#if use ch340 -> baudrate 27 777 
    timeout=1
)

print("Порт открыт. Идёт запись... (Ctrl+C для остановки)")

count = 0
start_time = time.time()  # отметка времени начала записи

try:
    with open('serial_data.csv', 'w', newline='') as f:
        writer = csv.writer(f, delimiter=';')  # <- разделитель точка с запятой
        writer.writerow(['Byte', 'Time_ms'])  # заголовки столбцов

        while True:
            data = ser.read(1)  # читаем 1 байт
            if data:
                elapsed_ms = int((time.time() - start_time) * 1000)  # миллисекунды
                writer.writerow([data[0], elapsed_ms])
                count += 1

except KeyboardInterrupt:
    ser.close()
    print(f"Запись завершена. Сохранено байт: {count}")
