const int BUTTON_PIN = 9;

#include "MicroGrader.h"
MG_Mode mode = TESTING;
uint8_t test_pins[] = {BUTTON_PIN};

#include <MPU9250.h>
#include <U8g2lib.h>
#include <math.h>
#include <Wifi.h>
#include <SPI.h>
#include <Wire.h>
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define SCREEN U8G2_SH1106_128X64_NONAME_F_4W_HW_SPI
#define SPI_CLK 14

MPU9250 imu;
SCREEN oled(U8G2_R2, 10, 15,16);  //Arduboy (Production, Kickstarter Edition)
ESP8266 wifi = ESP8266(0,true);

// IOT variables and constants
const String SSID = "J2";
const String PASSWD = "18611865";
const String HOST = "iesc-s2.mit.edu";
const int PORT = 80;
const String KERBEROS = "dmendels";  // your kerberos (need to change)
String path = "/6S08dev/" + KERBEROS + "/ex04/wiki_interface.py";


class Button{
    int state;
    int flag;
    elapsedMillis t_since_change; //timer since switch changed value
    elapsedMillis t_since_state_2; //timer since entered state 2 (reset to 0 when entering state 0)
    unsigned long debounce_time;
    unsigned long long_press_time;
    int pin;
    bool button_pressed;
  public:
    Button(int p) {    
      state = 0;
      pin = p;
      t_since_change = 0;
      t_since_state_2= 0;
      debounce_time = 10;
      long_press_time = 1000;
      button_pressed = 0;
    }
    void read() {
      bool button_state = digitalRead(pin);  // true if HIGH, false if LOW
      button_pressed = !button_state; // Active-low logic is hard, inverting makes our lives easier.
    }
		int update() {
  		read();
  		flag = 0;
  		if (state==0) { // Unpressed, rest state
    		if (button_pressed) {
      		state = 1;
      		t_since_change = 0;
    		}
  		} else if (state==1) { //Tentative pressed
    		if (!button_pressed) {
      		state = 0;
      		t_since_change = 0;
    		} else if (t_since_change >= debounce_time) {
      		state = 2;
      		t_since_state_2 = 0;
    		}
  		} else if (state==2) { // Short press
    		if (!button_pressed) {
      		state = 4;
      		t_since_change = 0;
    		} else if (t_since_state_2 >= long_press_time) {
      		state = 3;
    		}
  		} else if (state==3) { //Long press
    		if (!button_pressed) {
      		state = 4;
      		t_since_change = 0;
    		}
  		} else if (state==4) { //Tentative unpressed
    		if (button_pressed && t_since_state_2 < long_press_time) {
      		state = 2; // Unpress was temporary, return to short press
      		t_since_change = 0;
    		} else if (button_pressed && t_since_state_2 >= long_press_time) {
      		state = 3; // Unpress was temporary, return to long press
      		t_since_change = 0;
    		} else if (t_since_change >= debounce_time) { // A full button push is complete
      		state = 0;
      		if (t_since_state_2 < long_press_time) { // It is a short press
        		flag = 1;
      		} else {  // It is a long press
        		flag = 2;
      		}
    		}
  		}
  		return flag;
		}
};

class WikipediaGetter{
  String alphabet=" ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
  String message;
  String query_string = "";
  int char_index;
  int state;
  elapsedMillis scrolling_timer;
  int scrolling_threshold = 150;
  float angle_threshold = 0.3;
  public:
    WikipediaGetter(){
      state = 0;
      message ="";
      char_index = 0;
    }
		String update(float angle, int button){
  		if (state==0){
    		if (button==2){
		      char_index=0;
		      state=1;
    		}
   			return message;
  		} else if(state==1){
    		if (button == 1){
      		query_string = query_string+alphabet.substring(char_index,char_index+1);
      		char_index = 0;
      		return query_string + alphabet.substring(0,1);
    		} else if (button==2){
      		state = 2;
      		return "";
    		} else {
      		if(angle<-1*angle_threshold && scrolling_timer >scrolling_threshold){
        		if (char_index ==0) char_index = alphabet.length();
        		char_index--;
        		scrolling_timer=0;
      		}else if(angle>angle_threshold && scrolling_timer >scrolling_threshold){
        		char_index++;
        		char_index %=alphabet.length();
        		scrolling_timer=0;
      		}
      		return query_string + alphabet.substring(char_index,char_index+1);
    		}
  		} else if(state==2) {
    		if (!wifi.isBusy()) {
      		String total_query = "topic="+query_string;
      		wifi.sendRequest(GET, HOST, PORT, path, total_query);
      		state = 3;
      		query_string = "";
      		return "Query sent";
    		} else {
      		return "Wifi is busy";
    		}
  		} else if(state==3) {
    		if ( wifi.hasResponse()) {
      		message = wifi.getResponse();
      		int htmlindex = message.indexOf("</html>");
      		message = message.substring(6,htmlindex);
      		state=0;
      		return message; 
    		}else{
      		return "Waiting for response";
    		}
  		} 
		}
};


WikipediaGetter wg;
Button button(BUTTON_PIN);


void setup() {
	MicroGrader.begin(mode, test_pins);
  Serial.begin(115200);
  Wire.begin();
  SPI.setSCK(14);
  oled.begin();
  pinMode(BUTTON_PIN, INPUT_PULLUP);//set up pin!
  setup_angle();
  wifi.begin();
  //while (!wifi.isConnected());
  wifi.connectWifi(SSID, PASSWD);
  oled.setFont(u8g2_font_5x7_mf); //small, stylish font
  MicroGrader.debug("Start");
}

void loop() { 
  float x,y;
  get_angle(x,y); //get angle values
  int bv = button.update(); //get button value
  String output = wg.update(y,bv); //input: angle and button, output String to display on this timestep
  oled.clearBuffer();
  pretty_print(0,8,output,5,7,0,oled);
  oled.sendBuffer();
}


void pretty_print(int startx, int starty, String input, int fwidth, int fheight, int spacing, SCREEN &display){
  int x = startx;
  int y = starty;
  String temp = "";
  for (int i=0; i<input.length(); i++){
     if (fwidth*temp.length()<= (SCREEN_WIDTH-fwidth -x)){
        if (input.charAt(i)== '\n'){
          display.setCursor(x,y);
          display.print(temp);
          y += (fheight + spacing);
          temp = "";
          if (y>SCREEN_HEIGHT) break;
        }else{
          temp.concat(input.charAt(i));
        }
     }else{
      display.setCursor(x,y);
      display.print(temp);
      temp ="";
      y += (fheight + spacing);
      if (y>SCREEN_HEIGHT) break;
      if (input.charAt(i)!='\n'){
        temp.concat(input.charAt(i));
      }else{
          display.setCursor(x,y);
          y += (fheight + spacing);
          if (y>SCREEN_HEIGHT) break;
      } 
     }
     if(i==input.length()-1){
        display.setCursor(x,y);
        display.print(temp);
     }
  }
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

void get_angle(float&x, float&y){
  imu.readAccelData(imu.accelCount);
  x = imu.accelCount[0]*imu.aRes;
  y = imu.accelCount[1]*imu.aRes;
}




