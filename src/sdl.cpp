#include "lcd_lgfx.h"
#include <Arduino.h>

#define GAMEBOY_WIDTH 160
#define GAMEBOY_HEIGHT 144

static byte pixels[GAMEBOY_WIDTH * GAMEBOY_HEIGHT / 4];

static int button_start, button_select, button_a, button_b, button_down, button_up, button_left, button_right;

// Ajuste os pinos dos botões conforme seu hardware!
#define BTN_START 41
#define BTN_RIGHT 38

byte getColorIndexFromFrameBuffer(int x, int y) {
  int offset = x + y * GAMEBOY_WIDTH;
  return (pixels[offset >> 2] >> ((offset & 3) << 1)) & 3;
}

// Paleta Game Boy (preto, cinza escuro, cinza claro, branco)
const uint16_t color[] = {0x0000, 0x528A, 0xA514, 0xFFFF};

void SDL_Flip(byte *screen) {
  int i, j;
  gfx.startWrite();
  for (i = 0; i < GAMEBOY_WIDTH; i++) {
    for (j = 0; j < GAMEBOY_HEIGHT; j++) {
      gfx.drawPixel(i, j, color[getColorIndexFromFrameBuffer(i, j)]);
    }
  }
  gfx.endWrite();
}

void sdl_init(void)
{
  gfx.setRotation(1); // Landscape
  gfx.fillScreen(TFT_BLACK);

  pinMode(BTN_START, INPUT_PULLUP);
  pinMode(BTN_RIGHT, INPUT_PULLUP);

  // Se quiser testar a tela:
  gfx.fillScreen(TFT_RED);
}

int sdl_update(void) {
  button_start = !digitalRead(BTN_START);
  button_right = !digitalRead(BTN_RIGHT);
  // Adicione outros botões conforme necessário
  return 0;
}

unsigned int sdl_get_buttons(void)
{
  return (button_start*8) | (button_select*4) | (button_b*2) | button_a;
}

unsigned int sdl_get_directions(void)
{
  return (button_down*8) | (button_up*4) | (button_left*2) | button_right;
}

byte* sdl_get_framebuffer(void)
{
  return pixels;
}

void sdl_frame(void)
{
  static uint16_t rgb565[GAMEBOY_WIDTH * GAMEBOY_HEIGHT];

  for (int y = 0; y < GAMEBOY_HEIGHT; y++) {
    for (int x = 0; x < GAMEBOY_WIDTH; x++) {
      uint8_t idx = getColorIndexFromFrameBuffer(x, y);
      rgb565[x + y * GAMEBOY_WIDTH] = color[idx];
    }
  }
  int px = (320 - GAMEBOY_WIDTH) / 2;
  int py = (240 - GAMEBOY_HEIGHT) / 2;
  gfx.startWrite();
  gfx.pushImage(px, py, GAMEBOY_WIDTH, GAMEBOY_HEIGHT, rgb565);
  gfx.endWrite();
}


