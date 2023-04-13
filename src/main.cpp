#include <Arduino.h>
#ifdef ESP32
#include <bootloader_random.h>
#include <esp_random.h>
#endif
#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>

#define DATA_PIN (5)

Adafruit_NeoMatrix *matrix = new Adafruit_NeoMatrix(8, 8, 4, 2, DATA_PIN,
                                                    NEO_MATRIX_TOP + NEO_MATRIX_LEFT + NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG +
                                                    NEO_TILE_TOP + NEO_TILE_LEFT + NEO_TILE_ROWS + NEO_TILE_PROGRESSIVE,
                                                    NEO_GRB + NEO_KHZ800);

const uint16_t colors[] = {matrix->Color(255, 0, 0), matrix->Color(0, 255, 0), matrix->Color(0, 0, 255)};

uint16_t color = matrix->Color(0, 0, 0);

auto x1 = matrix->width();
auto x2 = matrix->width();
auto pass = 0;

const auto text = "Der Code zum Logbuch lautet";
const auto code = "1234";

void setColor() {
#ifdef ESP32
  color = esp_random();
#else
  color = matrix->Color(random(255), random(255), random(255));
#endif
}

void setup()
{
  #ifdef ESP32
  bootloader_random_enable();
  #else
  randomSeed(millis() + analogRead(A0));
  #endif
  setColor();

  matrix->begin();
  matrix->setTextWrap(false);
  matrix->setBrightness(10);
  matrix->cp437(true);
}

void loop()
{
  matrix->clear();

  // top row
  matrix->setCursor(x1, 0);
  matrix->setTextColor(color);
  matrix->print(text);

  if (--x1 < -170)
  {
    setColor();
    x1 = matrix->width();
    if (++pass >= 3)
      pass = 0;
  }

  // bottom row
  matrix->setCursor(x2, 8);
  matrix->setTextColor(colors[2]);
  matrix->print(code);

  if (--x2 < -25)
  {
    x2 = matrix->width();
  }

  matrix->show();
  delay(100);
}
