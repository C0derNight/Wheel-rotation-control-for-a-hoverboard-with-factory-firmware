import serial
import csv
import time

ser = serial.Serial(
    port='COM14',
    baudrate=115200,
    timeout=1
)

print("Порт открыт. Идёт запись... (Ctrl+C для остановки)")

count = 0
start_time = time.time()  # время начала записи

try:
    with open('serial_data.csv', 'w', newline='', encoding='utf-8') as f:
        writer = csv.writer(f, delimiter=';')
        writer.writerow(['Value', 'Time_ms'])

        while True:
            line = ser.readline().decode('utf-8').strip()  # читаем строку
            if line:
                try:
                    value = int(line)  # если приходит целое число
                    elapsed_ms = int((time.time() - start_time) * 1000)
                    writer.writerow([value, elapsed_ms])
                    count += 1
                except ValueError:
                    # если вдруг пришло не число — просто игнорируем
                    pass

except KeyboardInterrupt:
    ser.close()
    print(f"Запись завершена. Сохранено значений: {count}")
