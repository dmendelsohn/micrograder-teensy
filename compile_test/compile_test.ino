#define TEST 0

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
	setup_angle();
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("This should not be sent");
  MicroGrader.debug("Loop");
	imu.readMagData(imu.accelCount);
	String s = "IMU Data: ";
	s += String(imu.accelCount[0]);
	s += ", ";
	s += String(imu.accelCount[1]);
	s += ", ";
	s += String(imu.accelCount[2]);
	MicroGrader.debug(s);
	Serial.println(s);
	delay(1000);
}

void setup_angle(){
  char c = imu.readByte(MPU9250_ADDRESS, WHO_AM_I_MPU9250);
  Serial.print("MPU9250: "); Serial.print("I AM "); Serial.print(c, HEX);
  Serial.print(" I should be "); Serial.println(0x73, HEX);
  if (c == 0x73){
    imu.MPU9250SelfTest(imu.selfTest);
    imu.initMPU9250();
    imu.calibrateMPU9250(imu.gyroBias, imu.accelBias);
    imu.initMPU9250();
    imu.initAK8963(imu.factoryMagCalibration);
  } // if (c == 0x73)
  else
  {
    while(1) Serial.println("NOT FOUND"); // Loop forever if communication doesn't happen
  }
  imu.getAres();
  imu.getGres();
  imu.getMres();
}
