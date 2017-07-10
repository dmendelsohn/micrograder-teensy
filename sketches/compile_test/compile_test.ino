#include "MicroGrader.h"
MG_Mode mode = TESTING;
uint8_t test_pins[] = {6, 13};

#include <U8g2lib.h>
#define SPI_CLK 14
#define SCREEN U8G2_SH1106_128X64_NONAME_F_4W_HW_SPI
SCREEN oled(U8G2_R2, 10, 15, 16); //declare oled with shorter SCREEN class name

#include "MPU9250.h"
MPU9250 imu;

void setup() {
	MicroGrader.begin(mode, test_pins);
	MicroGrader.debug("MG Setup");
	
	Serial.begin(9600);
	delay(250);
	Serial.println("Setup");

	pinMode(13, OUTPUT);
	pinMode(A0, OUTPUT);
	
  SPI.setSCK(SPI_CLK); // move the SPI SCK pin from default of 13
  SPI.begin();  //SPI for OLED
	oled.begin();
	oled.clearBuffer();
	oled.drawBox(10, 10, 10, 10);
	
	setup_angle();
}


String s;
void loop() {
	Serial.println("Loop");
  MicroGrader.debug("Loop");
  oled.sendBuffer();
  digitalWrite(13, HIGH);
	analogWrite(A0, 150);


  imu.readAccelData(imu.accelCount);
  s = three_axis_string(imu.accelCount);
  Serial.println(s);
  MicroGrader.debug(s);
  
  imu.readGyroData(imu.gyroCount);
  s = three_axis_string(imu.gyroCount);
  Serial.println(s);
  MicroGrader.debug(s);
  
	imu.readMagData(imu.magCount);
  s = three_axis_string(imu.magCount);
  Serial.println(s);
  MicroGrader.debug(s);
  
  Serial.println();

	delay(1000);
}

String three_axis_string(int16_t *vals) {
	return String(vals[0]) + ", " + String(vals[1]) + ", " + String(vals[2]);
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

