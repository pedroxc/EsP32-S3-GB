// src/app.cpp
#include <Arduino.h>
#include <SPI.h>
#include <SD.h>

#include <WiFi.h>
#include <esp_bt.h>

#include "timer.h"
#include "mem.h"
#include "cpu.h"
#include "lcd.h"
#include "sdl.h"
#include "rom.h"   // cabeçalho do projeto (NÃO o rom.h gerado por bin2h)

#define SD_SCK   18
#define SD_MOSI  17
#define SD_MISO  16
#define SD_CS    15
#define ROM_PATH "/gb.gb"   // coloque sua ROM com esse nome na raiz do SD

// (opcional) se seu shield exigir BL enable num “pino Arduino”:
#define BL_PIN   14         // ajuste ou comente se não precisar

static void fatal(const char* msg) {
  Serial.println(msg);
  while (1) { delay(1000); }
}

void setup() {
    WiFi.mode(WIFI_OFF);
  btStop();
  Serial.begin(115200);
  delay(200);
  File root = SD.open("/");
  Serial.printf("PSRAM: %u bytes\n", ESP.getPsramSize());
  

while (true) {
  File entry = root.openNextFile();
  if (!entry) break;
  Serial.println(entry.name());
  entry.close();
}


#ifdef BL_PIN
  pinMode(BL_PIN, OUTPUT);
  digitalWrite(BL_PIN, HIGH);   // liga backlight, se aplicável ao seu shield
#endif

  // SPI/SD
  SPI.begin(SD_SCK, SD_MISO, SD_MOSI, SD_CS);
  if (!SD.begin(SD_CS)) fatal("SD FAIL");
  Serial.println("SD OK");

  // Carrega ROM do cartão
  if (!rom_load(ROM_PATH)) fatal("ROM load FAIL");
  Serial.println("ROM OK");

  // Vídeo (adaptador sdl.cpp -> LovyanGFX)
  sdl_init();

  // Núcleo do emulador
  gb_mem_init();  Serial.println("Mem OK");
  cpu_init();  Serial.println("CPU OK");
  Serial.println("Emulador iniciado");
}

void loop() {
  if (!cpu_cycle()) fatal("CPU halt");
  if (!lcd_cycle()) fatal("LCD halt");
  timer_cycle();
  (void)sdl_update();  // quando mapear botões, leia aqui
}
