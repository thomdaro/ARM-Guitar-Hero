#include <stm32f30x.h>
#include <f3d_lcd_sd.h>
#include <f3d_delay.h>
#include <stdio.h>

void main(void) {

  f3d_lcd_init();

  f3d_lcd_fillScreen(BLACK);
  f3d_lcd_drawString(55, 40, "DDR", WHITE, BLACK);
  f3d_lcd_drawString(40, 50, "But With", WHITE, BLACK);
  f3d_lcd_drawString(30, 60, "Wii Nunchuk", WHITE, BLACK);
  f3d_lcd_drawString(10, 100, "Green Fret to Start", WHITE, BLACK);
  
  while(1) {
    f3d_lcd_drawString(40, 20, "DDRBWWN", RED, BLACK);
    delay(50);
    f3d_lcd_drawString(40, 20, "DDRBWWN", YELLOW, BLACK);
    delay(50);
    f3d_lcd_drawString(40, 20, "DDRBWWN", GREEN, BLACK);
    delay(50);
    f3d_lcd_drawString(40, 20, "DDRBWWN", BLUE, BLACK);
    delay(50);
    f3d_lcd_drawString(40, 20, "DDRBWWN", CYAN, BLACK);
    delay(50);
  }
    
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t * file, uint32_t line) {
  /* Infinite loop */
  /* Use GDB to find out why we're here */
  while (1);
}
#endif
