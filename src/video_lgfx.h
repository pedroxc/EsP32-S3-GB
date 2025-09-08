#pragma once
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void video_init();                                // chama no setup do emulador
void lcd_blit_rgb565(const uint16_t* fb);       // desenha 160x144 RGB565
                                                // (se o projeto chamar nome diferente,
                                                //  ajuste os nomes aqui e no .cpp)

#ifdef __cplusplus
}
#endif
