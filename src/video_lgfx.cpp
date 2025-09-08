#include "lcd.h"
#include <LovyanGFX.hpp>

// ---- Defina qual controlador sua shield usa: ILI9341 é o mais comum em 2.4" ----
#define USE_PANEL_ILI9341   1   // se sua placa for R61581/RM68140, troque aqui

class LGFX_Para : public lgfx::LGFX_Device {
  lgfx::Bus_Parallel8  _bus;
#if USE_PANEL_ILI9341
  lgfx::Panel_ILI9341  _panel;
#else
  lgfx::Panel_R61581   _panel;   // exemplo alternativo; troque conforme seu shield
#endif
public:
  LGFX_Para() {
    { // --- BUS 8080 8-bit ---
      auto cfg = _bus.config();
      cfg.port = 0;                 // I80 usando I2S0
      cfg.freq_write = 30000000;    // 30 MHz (desça p/ 10 MHz se instável)
      cfg.pin_wr  = 11;
      cfg.pin_rd  = 12;
      cfg.pin_rs  = 10;             // RS/DC
      cfg.pin_d0  = 1;  cfg.pin_d1 = 2;  cfg.pin_d2 = 3;  cfg.pin_d3 = 4;
      cfg.pin_d4  = 5;  cfg.pin_d5 = 6;  cfg.pin_d6 = 7;  cfg.pin_d7 = 8;
      _bus.config(cfg);
      _panel.setBus(&_bus);
    }
    { // --- PANEL ---
      auto cfg = _panel.config();
      cfg.pin_cs   = 9;
      cfg.pin_rst  = 13;
      cfg.pin_busy = -1;
      cfg.memory_width  = 240;   // ajuste se seu controlador for 320x480 etc.
      cfg.memory_height = 320;
      cfg.panel_width   = 240;
      cfg.panel_height  = 320;
      cfg.offset_x = 0;
      cfg.offset_y = 0;
      cfg.offset_rotation = 0;
      cfg.dummy_read_pixel = 8;
      cfg.dummy_read_bits  = 1;
      cfg.readable = false;
      cfg.invert = false;
      cfg.rgb_order = false;
      cfg.dlen_16bit = false;
      cfg.bus_shared = false;
      _panel.config(cfg);
    }
    setPanel(&_panel);
  }
};

static LGFX_Para gfx;

void lgfx_lcd_init() {
  gfx.init();
  gfx.setRotation(1);       // Landscape: 320x240
  gfx.fillScreen(TFT_BLACK);
  // Se seu shield tiver pino BL separado, já deixamos ele em HIGH (via fio externo).
}

// O framebuffer do GB é 160x144. Vamos centralizar e escalar para altura 240 mantendo aspect ratio.
void lgfx_lcd_blit_rgb565(const uint16_t* fb) {
  const int gb_w = 160, gb_h = 144;
  const int screen_w = 320, screen_h = 240;

  int x = (screen_w - gb_w) / 2;
  int y = (screen_h - gb_h) / 2;

  gfx.startWrite();
  gfx.pushImage(x, y, gb_w, gb_h, fb);
  gfx.endWrite();
}
extern "C" void video_init() {
  lgfx_lcd_init();
}

extern "C" void lcd_blit_rgb565(const uint16_t* fb) {
  lgfx_lcd_blit_rgb565(fb);
}
