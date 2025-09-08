// src/sdl.cpp  (substitui o que usa Adafruit_*)
// ---> NADA de Adafruit_* aqui <---

#include "sdl.h"
#include "video_lgfx.h"
#include <string.h>
#include <stdint.h>

#define GB_W 160
#define GB_H 144
#define PIN_UP     35
#define PIN_DOWN   36
#define PIN_LEFT   37
#define PIN_RIGHT  38
#define PIN_A      39
#define PIN_B      40
#define PIN_START  41
#define PIN_SELECT 42
// Framebuffer 2bpp (4 pixels por byte) que o PPU do emulador escreve:
static byte     fb2bpp[GB_W * GB_H / 4];
// Buffer de saída RGB565 para mandar ao display:
static uint16_t fb565 [GB_W * GB_H];

static inline uint16_t gb2rgb565(uint8_t idx) {
  // 0=branco, 1=cinza claro, 2=cinza escuro, 3=preto (ajuste se quiser)
  switch (idx & 0x3) {
    case 0: return 0xFFFF; // white
    case 1: return 0xC618; // light gray
    case 2: return 0x7BEF; // dark gray
    default:return 0x0000; // black
  }
}

void sdl_init(void) {
  video_init();   

 
  pinMode(PIN_UP,     INPUT_PULLUP);
  pinMode(PIN_DOWN,   INPUT_PULLUP);
  pinMode(PIN_LEFT,   INPUT_PULLUP);
  pinMode(PIN_RIGHT,  INPUT_PULLUP);
  pinMode(PIN_A,      INPUT_PULLUP);
  pinMode(PIN_B,      INPUT_PULLUP);
  pinMode(PIN_START,  INPUT_PULLUP);
  pinMode(PIN_SELECT, INPUT_PULLUP);
}

int sdl_update(void) {
  // se você quiser encerrar o emulador, pode colocar alguma condição aqui
  return 0;
}

// unsigned int sdl_get_buttons(void)   { return 0; }  // START/SELECT/B/A (por enquanto, nenhum)
// unsigned int sdl_get_directions(void){ return 0; }  // CIMA/BAIXO/ESQ/DIR

byte* sdl_get_framebuffer(void) {
  return fb2bpp;           // o PPU escreve aqui (usa |=, por isso limpamos no frame)
}

void sdl_frame(void) {
  uint16_t* dst = fb565;
  for (int i = 0; i < GB_W * GB_H / 4; ++i) {
    uint8_t b = fb2bpp[i];
    *dst++ = gb2rgb565((b >> 0) & 0x3);
    *dst++ = gb2rgb565((b >> 2) & 0x3);
    *dst++ = gb2rgb565((b >> 4) & 0x3);
    *dst++ = gb2rgb565((b >> 6) & 0x3);
  }
  lcd_blit_rgb565(fb565);
  memset(fb2bpp, 0, sizeof(fb2bpp));
}
void sdl_quit(void) {
  // nada a fazer no bare-metal
}
unsigned int sdl_get_directions(void) {
  unsigned int dir = 0;
  if (!digitalRead(PIN_UP))    dir |= (1 << 0);
  if (!digitalRead(PIN_DOWN))  dir |= (1 << 1);
  if (!digitalRead(PIN_LEFT))  dir |= (1 << 2);
  if (!digitalRead(PIN_RIGHT)) dir |= (1 << 3);
  return dir;
}

unsigned int sdl_get_buttons(void) {
  unsigned int btn = 0;
  if (!digitalRead(PIN_A))      btn |= (1 << 0);
  if (!digitalRead(PIN_B))      btn |= (1 << 1);
  if (!digitalRead(PIN_START))  btn |= (1 << 2);
  if (!digitalRead(PIN_SELECT)) btn |= (1 << 3);
  return btn;
}