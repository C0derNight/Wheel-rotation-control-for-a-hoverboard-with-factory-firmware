

//!!! this scetch for "Print_packet_in_console.py" !!!


void setup() {
  Serial.begin(115200);   // USB Serial
  Serial2.begin(27777);   

}

void loop() {


   
    if (Serial2.available()) {
     uint8_t b2 = Serial2.read();
     Serial.write(b2);   
    }

     
   
}
