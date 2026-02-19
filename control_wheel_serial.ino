
float A_value = 0.00;
float B_value = 0.00;

uint8_t intPart = 0;   // 0–255
uint8_t fracPart = 0;  // 0–99





void setup() {
  Serial.begin(115200);

  pinMode(8, OUTPUT); // connect to rx motherboard motor 1
  pinMode(11, OUTPUT); // connect to rx motherboard motor 2


}

void loop() {

  readSerialCommand();

  
  if (A_value == 0.00){
      control_wheel(A_value,        177, 75, 8);
  }
  else{
    control_wheel(A_value,        85, 75, 8);
  }


  if (B_value == 0.00){
      control_wheel(B_value,        177, 75, 11);
  }
  else{
    control_wheel(B_value,        85, 75, 11);
  }

}


//enable_control = 177 -> stop motor
//enable_control = 85 -> run motor
void control_wheel(float F_speed, uint8_t enable_control, uint8_t Acc, uint8_t BB_TX_PIN) {




  float absVal = fabsf(F_speed);
  uint8_t absInt = (uint8_t)absVal;
  float frac = absVal - absInt;

  if (F_speed >= 0) {
    intPart = absInt;
    fracPart = (uint8_t)(frac * 130.0f);
  } else {
    intPart = 256 - absInt;

    if (frac == 0.0f) {
      fracPart = 0;  // ← ВАЖНО
    } else {
      fracPart = (uint8_t)((1.0f - frac) * 130.0f);
    }
  }

  // защита
  if (fracPart > 130) fracPart = 130;


  Serial.print(intPart);
  Serial.print(";");
  Serial.println(fracPart);

  digitalWriteFast(BB_TX_PIN, 1);
  delayMicroseconds(141);  // idle
  digitalWriteFast(BB_TX_PIN, 0);
  delayMicroseconds(341);
  digitalWriteFast(BB_TX_PIN, 1);  // stop
  delayMicroseconds(75);

  digitalWriteFast(BB_TX_PIN, 0);
  delayMicroseconds(38);
  digitalWriteFast(BB_TX_PIN, 1);
  delayMicroseconds(38);



  for (int i = 0; i < 8; i++) {
    if (fracPart & (1 << i))
      digitalWriteFast(BB_TX_PIN, HIGH);
    else
      digitalWriteFast(BB_TX_PIN, LOW);

    delayMicroseconds(38);
  }

  digitalWriteFast(BB_TX_PIN, 1);  //stop
  delayMicroseconds(114);

  digitalWriteFast(BB_TX_PIN, 0);
  delayMicroseconds(38);  //start

  // speed
  for (int i = 0; i < 8; i++) {
    if (intPart & (1 << i))
      digitalWriteFast(BB_TX_PIN, HIGH);
    else
      digitalWriteFast(BB_TX_PIN, LOW);

    delayMicroseconds(38);
  }

  digitalWriteFast(BB_TX_PIN, 0);
  delayMicroseconds(38);  //stop

  digitalWriteFast(BB_TX_PIN, 1);
  delayMicroseconds(151);
  digitalWriteFast(BB_TX_PIN, 0);
  delayMicroseconds(38);  // start
  digitalWriteFast(BB_TX_PIN, 1);
  delayMicroseconds(38);



  for (int i = 0; i < 8; i++) {
    if (fracPart & (1 << i))
      digitalWriteFast(BB_TX_PIN, HIGH);
    else
      digitalWriteFast(BB_TX_PIN, LOW);

    delayMicroseconds(38);
  }

  digitalWriteFast(BB_TX_PIN, 1);
  delayMicroseconds(114);  //stop + idle

  digitalWriteFast(BB_TX_PIN, 0);
  delayMicroseconds(37);  //start

  // speed
  for (int i = 0; i < 8; i++) {
    if (intPart & (1 << i))
      digitalWriteFast(BB_TX_PIN, HIGH);
    else
      digitalWriteFast(BB_TX_PIN, LOW);

    delayMicroseconds(38);
  }


  //digitalWriteFast(BB_TX_PIN, 1); delayMicroseconds(303);

  digitalWriteFast(BB_TX_PIN, 0);
  delayMicroseconds(37);  //stop


  digitalWriteFast(BB_TX_PIN, 1);
  delayMicroseconds(152);  // idle
  digitalWriteFast(BB_TX_PIN, 0);
  delayMicroseconds(38);  //start


  //==================================================== 85 - run   ====================================================//



  for (int i = 0; i < 8; i++) {
    if (enable_control & (1 << i))
      digitalWriteFast(BB_TX_PIN, HIGH);
    else
      digitalWriteFast(BB_TX_PIN, LOW);

    delayMicroseconds(38);
  }


  digitalWriteFast(BB_TX_PIN, 0);
  delayMicroseconds(38);  //stop

  digitalWriteFast(BB_TX_PIN, 1);  //idle
  delayMicroseconds(492);
  digitalWriteFast(BB_TX_PIN, 0);  //start
  delayMicroseconds(38);


  for (int i = 0; i < 8; i++) {
    if (Acc & (1 << i))
      digitalWriteFast(BB_TX_PIN, HIGH);
    else
      digitalWriteFast(BB_TX_PIN, LOW);

    delayMicroseconds(38);
  }


  digitalWriteFast(BB_TX_PIN, 0);  // stop
  delayMicroseconds(38);
  digitalWriteFast(BB_TX_PIN, 1);  // idle
  delayMicroseconds(38);
  digitalWriteFast(BB_TX_PIN, 0);  //start
  delayMicroseconds(38);


  for (int i = 0; i < 8; i++) {
    if (Acc & (1 << i))
      digitalWriteFast(BB_TX_PIN, HIGH);
    else
      digitalWriteFast(BB_TX_PIN, LOW);

    delayMicroseconds(38);
  }



  digitalWriteFast(BB_TX_PIN, 0);  // stop
  delayMicroseconds(38);
}


char buf[16];
byte idx = 0;

void readSerialCommand() {

  while (Serial.available()) {

    char c = Serial.read();

    if (c == '\n' || c == '\r') {

      buf[idx] = '\0';

      if (idx > 1) {

        char type = buf[0];
        float value = atof(&buf[1]);   // <<< FLOAT

        if (type == 'A' || type == 'a') {
          A_value = value;
        }
        else if (type == 'B' || type == 'b') {
          B_value = value;
        }
      }

      idx = 0;
    }
    else if (idx < sizeof(buf) - 1) {
      buf[idx++] = c;
    }
  }
}




