#define TEST 1

#include "MicroGrader.h"
#include "MPU9250.h"
#include <U8g2lib.h>

void setup() {
	MicroGrader.begin();
  // put your setup code here, to run once:
	MicroGrader.debug("Hello, world!");
	pinMode(10, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("This should not be sent");
  MicroGrader.debug("Loop");
  int n = digitalRead(10);
  MicroGrader.debug("Got reading of " + String(n));
  delay(1000);
}
