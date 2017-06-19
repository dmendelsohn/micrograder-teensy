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
	MicroGrader.debug("Loop");
	//delay(100); // This delay breaks the timing
}

void loop() {
	oled.clearBuffer();
	oled.sendBuffer();
	digitalWrite(13, LOW);
	delay(1000);
	oled.drawBox(20,10,20,10); // Change dimensions to break the test
	oled.sendBuffer();
	digitalWrite(13, HIGH);
	delay(1000);
}
