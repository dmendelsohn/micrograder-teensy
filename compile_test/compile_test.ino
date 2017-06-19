#define TEST 1

#include "MicroGrader.h"
#include "MPU9250.h"
#include <U8g2lib.h>

void setup() {
	MicroGrader.begin();
	MicroGrader.debug("Setup");
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("This should not be sent");
  MicroGrader.debug("Loop");
  digitalWrite(13, HIGH);
  analogWriteRes(10);
  analogWrite(A0, 100);
  delay(1000);
}
