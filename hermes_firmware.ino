#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH1106.h>
#include "U8glib.h"

U8GLIB_SSD1306_128X32 u8g(U8G_I2C_OPT_NONE);
#define OLED_RESET -1
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_ADDR 0x78
Adafruit_SH1106 display (OLED_RESET);

#define MPU_ADDR 0x68

float ballX = SCREEN_WIDTH / 2;
const float ballY = SCREEN_HEIGHT / 2;
const uint8_t ballR = 4;
lastTime = millis();

void setup() {  
  Serial.begin(9600);
  Wire.begin();

  display.begin(SH1106_SWITCH, 0x3c);
  display.clearDisplay();
  display.settTextColor(WHITE);
  display.setTextSize(2);
  display.setCursor(15,5);
  display.println("Ball game");
  display.display();
  delay(500);
}
void initMPU(){
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x6B);
  Wire.write(0x00);
  Wire.endTransmission();

  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x1B);
  Wire.write(0x00);
  Wire.endTransmission();
}

void readMPU(int16_t &ax, int16_t &ay, int16_t &az){
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x43);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_ADDR, 6);
  
  int16_t gyroX, gyroY, gyroZ;
  gyroX = (Wire.read() << 8) | Wire.read();
  gyroY = (Wire.read() << 8) | Wire.read();
  gyroZ = (Wire.read() << 8) | Wire.read();
}

void loop (){
  readMPU();
  int16_t ax, ay, az;
  readMPU(ax, ay, az);
  unsigned long now = millis();
  float dt = (now - lastTime) / 1000.0; 
  //ball
  if (ballX < ballR) ballX = ballR;
  if (ballX > SCREEN_WIDTH - ballR) ballX = SCREEN_WIDTH - ballR;
  if (ballY < ballR) ballY = ballR;
  if (ballY > SCREEN_HEIGHT - ballR) ballY = SCREEN_HEIGHT - ballR;


  const float g = 40.0;
  float velX = ax * g;
  float velY = ay * g;
  ballX += velX * dt;
  ballY += velY * dt;
   display.fillCircle(int(ballX), int(ballY), ballR, WHITE);
   delay(10);

}



