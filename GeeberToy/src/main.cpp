#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>

#define DISP_CS    D0
#define DISP_RES   D1
#define DISP_DC    D2

#define DISP_SDA   D10   // MOSI
#define DISP_SCL   D8    // SCK

Adafruit_ST7789 disp = Adafruit_ST7789(
  DISP_CS,
  DISP_DC,
  DISP_SDA,
  DISP_SCL,
  DISP_RES
);

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("Init display");

  disp.init(240, 240);
  disp.setRotation(0);
  disp.fillScreen(ST77XX_RED);

  Serial.println("Screen should be red");
}

void loop() {}