#define TEST 1

#include "MicroGrader.h"
#include "MPU9250.h"
#include <U8g2lib.h>

#define SPI_CLK 14
#define SCREEN U8G2_SH1106_128X64_NONAME_F_4W_HW_SPI
SCREEN oled(U8G2_R2, 10, 15, 16); //declare oled with shorter SCREEN class name


MPU9250 imu;

void setup() {
	MicroGrader.begin();
	MicroGrader.debug("Setup");
	pinMode(13, OUTPUT);
  SPI.setSCK(SPI_CLK); // move the SPI SCK pin from default of 13
  SPI.begin();  //SPI for OLED
	oled.begin();
	oled.clearBuffer();
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("This should not be sent");
  MicroGrader.debug("Loop");
	oled.drawBox(10,10,10,10);
	oled.sendBuffer();
	delay(1000);
}
