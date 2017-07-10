void setup() {
  Serial.begin(115200);
	delay(1000);
}

char c = '0';
unsigned long u = 0;

void loop() {
  // put your main code here, to run repeatedly:
  elapsedMicros t = 0;
  for (int i = 0; i < 636; i++) {
  	Serial.print('0');
  }
	Serial.print(":");
	Serial.println(u);
	Serial.send_now();
	//Serial.flush();
	while (Serial.available() == 0 && t < 10000);
	if (Serial.available()) {
		c = Serial.read();
		u = t;
	}
	delay(1000);
}
