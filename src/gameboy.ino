#include <Arduino.h>
#include "lcd_lgfx.h"
#include "timer.h"
#include "rom.h"
#include "mem.h"
#include "cpu.h"
#include "lcd.h"
#include "sdl.h"
#include <SD.h>
#include <SPI.h>

#define SCK 16
#define MOSI 17
#define MISO 18
#define CS 15
#define ROM_MAX_SIZE (1024 * 1024) // 1MB, ajuste conforme necessário

uint8_t* gb_rom = nullptr;
size_t gb_rom_len = 0;

bool load_rom_from_sd(const char* filename) {
    File romFile = SD.open(filename, FILE_READ);
    if (!romFile) {
        Serial.println("Falha ao abrir ROM no SD!");
        return false;
    }

    gb_rom_len = romFile.size();
    if (gb_rom_len > ROM_MAX_SIZE) {
        Serial.println("ROM muito grande!");
        romFile.close();
        return false;
    }

    gb_rom = (uint8_t*)malloc(gb_rom_len);
    if (!gb_rom) {
        Serial.println("Falha ao alocar memória para ROM!");
        romFile.close();
        return false;
    }

    romFile.read(gb_rom, gb_rom_len);
    romFile.close();
    Serial.println("ROM carregada do SD com sucesso!");
    return true;
}

void setup() {
  delay(1200);
  Serial.begin(115200);
  SPI.begin(SCK, MISO, MOSI, CS);

  if (!SD.begin(CS)) {
      Serial.println("Falha ao inicializar o SD!");
      while (1);
  }
  Serial.println("SD OK!");

  if (!load_rom_from_sd("/gb.gb")) {
      Serial.println("Falha ao carregar ROM!");
      while (1);
  }

  gfx.init();
  gfx.setRotation(1);

  Serial.println("Iniciando ROM...");
  int r = rom_init(gb_rom);
  Serial.print("rom_init: "); Serial.println(r);
//   sdl_init();

  Serial.println("ROM OK!");
  gb_mem_init();
  Serial.println("Mem OK!");
  cpu_init();
  Serial.println("CPU OK!");
  Serial.println("Fim do setup");
}

void loop() {
  cpu_cycle();
  lcd_cycle();
  timer_cycle();
}
