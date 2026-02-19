
//this sketch for "Save_time_value_in_file.py". 


#define SIG_PIN 7

volatile uint32_t last_time = 0;
volatile uint32_t duration = 0;
volatile uint8_t last_level = 1;
volatile bool new_data = false;

void isr_change() {
  uint32_t now = micros();
  uint8_t level = digitalReadFast(SIG_PIN);

  duration = now - last_time;
  last_time = now;
  last_level = level;
  new_data = true;
}

void setup() {
  Serial.begin(115200);
  pinMode(SIG_PIN, INPUT);

  last_time = micros();
  last_level = digitalReadFast(SIG_PIN);

  attachInterrupt(digitalPinToInterrupt(SIG_PIN), isr_change, CHANGE);
}

void loop() {
  if (new_data) {
    noInterrupts();
    uint32_t d = duration;
    uint8_t l = last_level;
    new_data = false;
    interrupts();

    //Serial.print(l);
   // Serial.print(';');
    Serial.println(d);
  }
}
