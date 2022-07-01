#include "pico/stdlib.h"
#include <stdio.h>
#include <cstring>
#include <string>
#include <deque>
#include <algorithm>
#include "pico/time.h"
#include "pico/platform.h"

#include "common/pimoroni_common.hpp"
#include "drivers/st7789/st7789.hpp"
#include "libraries/pico_graphics/pico_graphics.hpp"
#include "tufty2040.hpp"
#include "button.hpp"

using namespace pimoroni;

Tufty2040 tufty;

ST7789 st7789(
  Tufty2040::WIDTH,
  Tufty2040::HEIGHT,
  ROTATE_180,
  ParallelPins{
    Tufty2040::LCD_CS,
    Tufty2040::LCD_DC,
    Tufty2040::LCD_WR,
    Tufty2040::LCD_RD,
    Tufty2040::LCD_D0, 
    Tufty2040::BACKLIGHT
  }
);

PicoGraphics_PenRGB332 graphics(st7789.width, st7789.height, nullptr);

Button button_a(Tufty2040::A);
Button button_b(Tufty2040::B);
Button button_c(Tufty2040::C);
Button button_up(Tufty2040::UP);
Button button_down(Tufty2040::DOWN);

int main() {
  stdio_init_all();

  st7789.set_backlight(255);

  Pen WHITE = graphics.create_pen(255, 255, 255);
  Pen BG = graphics.create_pen(120, 40, 60);

  uint8_t i = 0;

  graphics.set_pen(BG);
  graphics.clear();
  st7789.update(&graphics);

  std::deque<std::string> serial_text;
  for (int i = 0; i < 14; ++i) {
    serial_text.push_back("Hello world");
  }
  serial_text.push_back("");
  while(true) {  
    int c = getchar_timeout_us(500000);
    if (c != PICO_ERROR_TIMEOUT) {
      if (c == '\r') {
        serial_text.push_back("");
        if (serial_text.size() > 15) {
          serial_text.pop_front();
        }
      }
      else {
        serial_text.back() += (char)c;
  
        graphics.set_pen(BG);
        graphics.clear();

        graphics.set_pen(WHITE);
        Point text_location(30, 0);
        for (auto& text : serial_text) {
          graphics.text(text, text_location, 320);
          text_location.y += 16;
        }
      }

      st7789.update(&graphics);
    }

        graphics.set_pen(BG);
        graphics.clear();

        graphics.set_pen(WHITE);
        Point text_location(i % 32, 0);
        for (auto& text : serial_text) {
          graphics.text(text, text_location, 320);
          text_location.y += 16;
        }

      st7789.update(&graphics);

    i += 1;
    tufty.led(i);
  }

  return 0;
}
