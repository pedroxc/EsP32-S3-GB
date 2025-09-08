#pragma once
#include <LovyanGFX.hpp>

class LGFX_ESP32S3_8080 : public lgfx::LGFX_Device {
  lgfx::Panel_ILI9341 _panel_instance;
  lgfx::Bus_Parallel8 _bus_instance;
public:
  LGFX_ESP32S3_8080() {
    {
      auto cfg = _bus_instance.config();
      cfg.port = 0;
      cfg.freq_write = 10000000; // Tente 10MHz para estabilidade
      cfg.pin_wr = 11;
      cfg.pin_rd = 12;
      cfg.pin_rs = 10;
      cfg.pin_d0 = 1;
      cfg.pin_d1 = 2;
      cfg.pin_d2 = 3;
      cfg.pin_d3 = 4;
      cfg.pin_d4 = 5;
      cfg.pin_d5 = 6;
      cfg.pin_d6 = 7;
      cfg.pin_d7 = 8;
      _bus_instance.config(cfg);
      _panel_instance.setBus(&_bus_instance);
    }
    {
      auto cfg = _panel_instance.config();
      cfg.pin_cs = 9;
      cfg.pin_rst = 13;
      cfg.memory_width = 320;
      cfg.memory_height = 240;
      cfg.panel_width = 320;
      cfg.panel_height = 240;
      cfg.offset_x = 0;
      cfg.offset_y = 0;
      cfg.offset_rotation = 0;
      cfg.dummy_read_pixel = 8;
      cfg.dummy_read_bits = 1;
      cfg.readable = false;
      cfg.invert = false;
      cfg.rgb_order = false;
      cfg.dlen_16bit = false;
      cfg.bus_shared = false;
      _panel_instance.config(cfg);
    }
    setPanel(&_panel_instance);
  }
};

extern LGFX_ESP32S3_8080 gfx;