const int TX_PIN = 8;  // любой GPIO

// Тайминги (мкс)
const uint16_t timings[] = {
  141,341,75,38,76,265,114,38,303,38,151,38,76,265,114,37,
  304,37,152,38,38,38,37,38,38,38,38,76,492,38,114,38,
  37,38,38,76,38,38,113,38,38,38,38,76
};

const uint16_t timingsCount = sizeof(timings) / sizeof(timings[0]);

void setup() {
  pinMode(TX_PIN, OUTPUT);

  // UART idle = HIGH
  digitalWrite(TX_PIN, HIGH);
  
  noInterrupts();      // для точных таймингов (по желанию)

}

void loop() {

  bool level = HIGH;   //  первое число = 1

  
  for (uint16_t i = 0; i < timingsCount; i++) {
    digitalWriteFast(TX_PIN, level);
    delayMicroseconds(timings[i]);
    level = !level;    // ⬅️ смена уровня на каждом тайминге
  }

  //interrupts();

  // вернуть линию в idle
  //digitalWrite(TX_PIN, HIGH);

  // пауза между пакетами
  delayMicroseconds(1500); // 1 мс
}
