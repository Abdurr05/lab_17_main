#include "oled-wing-adafruit.h"
#include "LIS3DH.h"
#include <blynk.h>

SYSTEM_THREAD(ENABLED);

OledWingAdafruit display;
LIS3DHSPI accel(SPI, D5, WKP);
int movingX;
int movingY;
const unsigned long PRINT_SAMPLE_PERIOD = 100;
unsigned long lastPrintSample = 0;



int x = 10;
int y = 10;
bool chooseJolystick;
bool newGame = false;
bool chooseGame = true;
int maze;

const unsigned char maze1[] PROGMEM = {
  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
  0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,
  0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,
  0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,
  0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,
  0x87,0xE0,0x00,0x01,0xFF,0xC0,0x00,0x03,0xF0,0xFF,
  0x84,0x20,0x00,0x00,0x00,0x40,0x00,0x02,0x00,0xFF,
  0x84,0x20,0x00,0x00,0x00,0x40,0x00,0x02,0x00,0xFF,
  0x84,0x20,0x00,0x00,0x00,0x40,0x00,0x02,0x00,0xFF,
  0x84,0x20,0x00,0x00,0x00,0x40,0x00,0x02,0x00,0xFF,
  0x84,0x3F,0x07,0xFF,0xFF,0xFF,0xF0,0x02,0x0F,0xFF,
  0x84,0x01,0x00,0x00,0x00,0x00,0x00,0x02,0x00,0x7F,
  0x84,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x00,0x7F,
  0x84,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x00,0x7F,
  0x84,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x00,0x7F,
  0x84,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x00,0x7F,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x10,0xFF,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x10,0xFF,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x10,0xFF,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x10,0xFF,
  0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x10,0xFF,
  0x87,0xFF,0xFF,0xC1,0xFF,0xFF,0xFF,0xFC,0x10,0xFF,
  0x84,0x00,0x00,0x01,0x00,0x40,0x00,0x00,0x10,0xFF,
  0x84,0x00,0x00,0x01,0x00,0x40,0x00,0x00,0x10,0xFF,
  0x84,0x00,0x00,0x01,0x00,0x40,0x00,0x00,0x10,0xFF,
  0x84,0x00,0x00,0x01,0x00,0x40,0x00,0x00,0x10,0xFF,
  0x84,0x1F,0x07,0xFE,0x08,0x3F,0xF0,0x03,0xF0,0xFF,
  0x84,0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0xFF,
  0x84,0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0xFF,
  0x84,0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0xFF,
  0x84,0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0xFF,
  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF
};
const unsigned char maze2[] PROGMEM = {
  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
  0x80,0x20,0x00,0x00,0x08,0x40,0x00,0x00,0x1F,
  0x80,0x20,0x00,0x00,0x08,0x40,0x00,0x00,0x1F,
  0x80,0x20,0x00,0x00,0x08,0x40,0x00,0x00,0x1F,
  0x80,0x20,0x00,0x00,0x08,0x40,0x00,0x00,0x1F,
  0x80,0x20,0xFF,0xC0,0x08,0x41,0xFF,0xFC,0x1F,
  0x84,0x00,0x00,0x01,0x00,0x40,0x00,0x00,0x1F,
  0x84,0x00,0x00,0x01,0x00,0x40,0x00,0x00,0x1F,
  0x84,0x00,0x00,0x01,0x00,0x40,0x00,0x00,0x1F,
  0x84,0x00,0x00,0x01,0x00,0x40,0x00,0x00,0x1F,
  0x87,0xFF,0xFF,0xFF,0xF0,0x7E,0x00,0x03,0xFF,
  0x80,0x20,0x00,0x00,0x00,0x40,0x00,0x00,0x1F,
  0x80,0x20,0x00,0x00,0x00,0x40,0x00,0x00,0x1F,
  0x80,0x20,0x00,0x00,0x00,0x40,0x00,0x00,0x1F,
  0x80,0x20,0x00,0x00,0x00,0x40,0x00,0x00,0x1F,
  0x80,0x20,0x00,0x00,0x00,0x40,0x10,0x00,0x1F,
  0x04,0x00,0x00,0x00,0x08,0x40,0x10,0x00,0x1F,
  0x04,0x00,0x00,0x00,0x08,0x40,0x00,0x00,0x1F,
  0x04,0x00,0x00,0x00,0x08,0x40,0x00,0x00,0x1F,
  0x04,0x00,0x00,0x00,0x08,0x40,0x00,0x00,0x1F,
  0x04,0x00,0x00,0x00,0x08,0x40,0x00,0x00,0x1F,
  0x87,0xFF,0xFF,0xC1,0x07,0xFE,0x00,0x7C,0x1F,
  0x84,0x01,0x00,0x01,0x00,0x00,0x00,0x00,0x1F,
  0x84,0x01,0x00,0x01,0x00,0x00,0x00,0x00,0x1F,
  0x84,0x01,0x00,0x01,0x00,0x00,0x00,0x00,0x1F,
  0x84,0x01,0x00,0x01,0x00,0x00,0x00,0x00,0x1F,
  0x80,0x20,0x00,0x3F,0xFF,0xFF,0xFF,0x83,0xFF,
  0x80,0x20,0x00,0x00,0x00,0x00,0x00,0x00,0x0F,
  0x80,0x20,0x00,0x00,0x00,0x00,0x00,0x00,0x0F,
  0x80,0x20,0x00,0x00,0x00,0x00,0x00,0x00,0x0F,
  0x80,0x20,0x00,0x00,0x00,0x00,0x00,0x00,0x0F,
  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF
};
const unsigned char maze3[] PROGMEM = {
  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
  0x84,0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x40,0x10,0x00,0x00,0x03,
  0x84,0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x40,0x10,0x00,0x00,0x03,
  0x84,0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x40,0x10,0x00,0x00,0x03,
  0x84,0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x40,0x10,0x00,0x00,0x03,
  0x84,0x00,0xFF,0xFF,0x00,0x3F,0xFF,0xFE,0x00,0x43,0xF0,0x7C,0x1F,0xFF,
  0x80,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x00,0x40,0x00,0x00,0x10,0x07,
  0x80,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x00,0x40,0x00,0x00,0x10,0x07,
  0x80,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x00,0x40,0x00,0x00,0x10,0x07,
  0x80,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x00,0x40,0x00,0x00,0x10,0x07,
  0xFF,0xE0,0x00,0x3F,0xFF,0xC0,0x00,0x41,0xF0,0x7F,0xFF,0x80,0x1F,0x07,
  0x80,0x00,0x00,0x00,0x00,0x00,0x08,0x00,0x00,0x40,0x00,0x00,0x10,0x07,
  0x80,0x00,0x00,0x00,0x00,0x00,0x08,0x00,0x00,0x40,0x00,0x00,0x10,0x07,
  0x80,0x00,0x00,0x00,0x00,0x00,0x08,0x00,0x00,0x40,0x00,0x00,0x10,0x07,
  0x80,0x00,0x00,0x00,0x00,0x00,0x08,0x00,0x00,0x40,0x00,0x00,0x10,0x07,
  0x80,0x01,0x04,0x00,0x00,0x20,0x08,0x02,0x00,0x40,0x00,0x00,0x10,0x07,
  0x00,0x20,0x04,0x01,0x00,0x20,0x00,0x02,0x00,0x40,0x10,0x00,0x00,0x87,
  0x00,0x20,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x40,0x10,0x00,0x00,0x87,
  0x00,0x20,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x40,0x10,0x00,0x00,0x87,
  0x00,0x20,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x40,0x10,0x00,0x00,0x87,
  0x00,0x20,0x00,0x01,0x00,0x00,0x00,0x02,0x00,0x40,0x10,0x00,0x00,0x87,
  0x84,0x1F,0x00,0x3F,0x07,0xFF,0xF0,0x7E,0x00,0x7F,0xF0,0x7F,0xFF,0x07,
  0x84,0x00,0x00,0x01,0x00,0x00,0x00,0x40,0x00,0x00,0x10,0x00,0x10,0x07,
  0x84,0x00,0x00,0x01,0x00,0x00,0x00,0x40,0x00,0x00,0x10,0x00,0x10,0x07,
  0x84,0x00,0x00,0x01,0x00,0x00,0x00,0x40,0x00,0x00,0x10,0x00,0x10,0x07,
  0x84,0x00,0x00,0x01,0x00,0x00,0x00,0x40,0x00,0x00,0x10,0x00,0x10,0x07,
  0x87,0xFF,0x07,0xE1,0xF8,0x3F,0xF8,0x01,0xFF,0xFE,0x00,0x00,0x1F,0x07,
  0x80,0x00,0x00,0x20,0x00,0x00,0x08,0x00,0x00,0x02,0x00,0x00,0x00,0x07,
  0x80,0x00,0x00,0x20,0x00,0x00,0x08,0x00,0x00,0x02,0x00,0x00,0x00,0x07,
  0x80,0x00,0x00,0x20,0x00,0x00,0x08,0x00,0x00,0x02,0x00,0x00,0x00,0x07,
  0x80,0x00,0x00,0x20,0x00,0x00,0x08,0x00,0x00,0x02,0x00,0x00,0x00,0x07,
  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF
};



BLYNK_WRITE(V1) {
  int xx = param[0].asInt();
  int yy = param[1].asInt();
  
  Serial.print("X = ");
  Serial.print(xx);
  Serial.print("; Y = ");
  Serial.println(yy);
  // int pinValue = param.asInt(); 
  // Serial.print("V1 Slider value is: ");
  // Serial.println(pinValue);
  if(chooseJolystick){
    movingY = yy;
    movingX = xx;
  }
}
// setup() runs once, when the device is first turned on.
void setup() {
  // Put initialization like pinMode and begin functions here.
  Serial.begin(9600);

  display.setup();
	display.clearDisplay();
	display.display();

  Blynk.begin("KFgwPygKZdZF5I_kkJtpz3rqrdW55Z0J", IPAddress(167, 172, 234, 162), 9090);

  LIS3DHConfig config;
	config.setPositionInterrupt(16);
	bool setupSuccess = accel.setup(config);
	Serial.printlnf("setupSuccess=%d", setupSuccess);



  
  // Put initialization like pinMode and begin functions here.

}

// loop() runs over and over again, as quickly as it can execute.
void loop() {
  Blynk.run();
  display.loop();
  LIS3DHSample sample;
  while(chooseGame){
    display.loop();
    display.clearDisplay();
		display.setTextSize(1);
		display.setTextColor(WHITE);
		display.setCursor(0,0);
		display.println("Choose the difficulty");
    display.println("A: easy difficulty");
    display.println("B: medium difficulty");
    display.println("C: hard difficulty");
		display.display();
    if (display.pressedA()) {
      maze = 1;
      newGame = true;
      chooseGame = false;
    }
    if (display.pressedB()) {
      maze = 2;
      newGame = true;
      chooseGame = false;
    }
    if (display.pressedC()) {
      maze = 3;
      newGame = true;
      chooseGame = false;
    }
  }
  while(newGame){
    display.loop();
    display.clearDisplay();
		display.setTextSize(1);
		display.setTextColor(WHITE);
		display.setCursor(0,0);
		display.println("Press A to play with the accelerometer");
    display.println("Press B to play with the Jolystick");
		display.display();
    if (display.pressedA()) {
      chooseJolystick = false;
      newGame = false;
    }
    if (display.pressedB()) {
      chooseJolystick = true;
      newGame = false;
    }
  }
if(!newGame){
  if(!chooseJolystick){
    if (millis() - lastPrintSample >= PRINT_SAMPLE_PERIOD){
      lastPrintSample = millis();
      if (accel.getSample(sample)){
        Serial.printlnf("%d,%d,%d", sample.x, sample.y, sample.z);
        movingX = sample.x;
        movingY = sample.y + 300;
      }
      else{
        Serial.println("no sample");
      }
    }
    movingY = movingY / 2000;
    movingX = movingX / 2000;
  }
  if(movingY > 0){
    for (size_t i = 0; i < movingY ; i++){
      if(display.getPixel(x,y-1) == 0 && y > 0){
        y = y-1;
      }
    }
  }
  if(movingX > 0){
    for (size_t i = 0; i < movingX ; i++){
      if(display.getPixel(x+1,y) == 0 && x != 123){
        x = x+1;
      }
    }
  }
  if(movingY < 0){
    for (int i = 0; i > movingY ; i--){
      if(display.getPixel(x,y+1) == 0 && y != 31){
        y = y+1;
      }
    }
  }
   if(movingX < 0){
    for (int i = 0; i > movingX ; i--){
      if(display.getPixel(x-1,y) == 0 && x > 0){
        x = x-1;
      }
    }
  }


  display.clearDisplay();
	display.setTextSize(1);
	display.setTextColor(WHITE);
  display.drawPixel(x,y,WHITE);
  if(maze == 1){
    display.drawBitmap(25,0, maze1,73,32,WHITE);
  }
  if(maze == 2){
    display.drawBitmap(25,0, maze2,73,32,WHITE);
  }
  if(maze == 3){
    display.drawBitmap(13,0, maze3,112,32,WHITE);
  }
	display.display();
  delay(200);
  if(x == 123){
    display.clearDisplay();
		display.setTextSize(1);
		display.setTextColor(WHITE);
		display.setCursor(0,0);
		display.println("You Win!");
		display.display();
    delay(5000);
    newGame = false;
    chooseGame = true;
    x = 10;
    y = 10;
    movingX = 0;
    movingY = 0;
  }
  // The core of your code will likely live here.
}
}