#include "MicroGrader.h"
MG_Mode mode = INACTIVE;
uint8_t test_pins[] = {6, 13};

#include "MPU9250.h"
#include <U8g2lib.h>

#define SPI_CLK 14
#define SCREEN U8G2_SH1106_128X64_NONAME_F_4W_HW_SPI
SCREEN oled(U8G2_R2, 10, 15, 16); //declare oled with shorter SCREEN class name


MPU9250 imu;

void setup() {
	//MicroGrader.begin(mode, test_pins);
	//MicroGrader.debug("Setup");
	Serial.begin(9600);
	delay(100);
	Serial.println("Inactive mode: setup");
	pinMode(13, OUTPUT);
	pinMode(6, INPUT_PULLUP);
	pinMode(9, INPUT_PULLUP);
  SPI.setSCK(SPI_CLK); // move the SPI SCK pin from default of 13
  SPI.begin();  //SPI for OLED
	oled.begin();
	oled.clearBuffer();
}

void loop() {
	Serial.println("Inactive mode: loop"); // Only prints when not in test mode
	if (!digitalRead(6)) { // Button pressed
		oled.drawBox(20,10,20,10); // Change dimensions to break the test
		oled.sendBuffer();
		digitalWrite(13, HIGH);
	} else { // Button not pressed
		oled.clearBuffer();
		oled.sendBuffer();
		digitalWrite(13, LOW);
	}
	delay(50);  // Increase delay to break test
}
