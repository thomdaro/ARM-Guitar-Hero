#include <stm32f30x.h>
#include <f3d_lcd_sd.h>
#include <f3d_uart.h>
#include <f3d_delay.h>
#include <f3d_guitar.h>
#include <f3d_i2c.h>
#include <stdio.h>

#define BARDOWN(x) ((x.b_one) >> 6)
#define BARUP(x) ((x.b_two) & 0x1)
#define MINUS(x) (((x.b_one) >> 4) & 0x1)
#define PLUS(x) (((x.b_one) >> 2) & 0x1)
#define BUTTON(x, y) (((x.b_two) >> (3 + y)) & 0x1)) //vals 0->4 y,g,b,r,o

void selected_item(int x, int y, char* str){
  f3d_lcd_drawString(x, y, str, RED, BLACK);
  delay(50);
  f3d_lcd_drawString(x, y, str, YELLOW, BLACK);
  delay(50);
  f3d_lcd_drawString(x, y, str, GREEN, BLACK);
  delay(50);
  f3d_lcd_drawString(x, y, str, BLUE, BLACK);
  delay(50);
  f3d_lcd_drawString(x, y, str, CYAN, BLACK);
  delay(50);
}

void main(void) {
  int menu_drawn_pause = 0;
  int menu_item = 0;

  f3d_lcd_fillScreen(BLACK);
  f3d_lcd_drawString(55, 40, "DDR", WHITE, BLACK);
  f3d_lcd_drawString(40, 50, "But With", WHITE, BLACK);
  f3d_lcd_drawString(30, 60, "Wii Nunchuk", WHITE, BLACK);
  f3d_lcd_drawString(10, 100, "Green Fret to Start", WHITE, BLACK);
  
  //pause menu
  while(1) {
    if(!menu_drawn_pause){
      f3d_lcd_fillScreen(BLACK);
      f3d_lcd_drawString(55, 40, "PAUSE", WHITE, BLACK);
      f3d_lcd_drawString(40, 50, "Restart", WHITE, BLACK);
      f3d_lcd_drawString(40, 60, "Song Select", WHITE, BLACK);
      f3d_lcd_drawString(40, 70, "Main Menu", WHITE, BLACK);
      menu_drawn_pause = 1;
    }
    switch(menu_item){
    case 0:
      selected_item(40, 50, "Restart");
      break;
    case 1:
      selected_item(40, 60, "Song Select");
      break;
    case 2:
      selected_item(40, 70, "Main Menu");
      break;
    }
    
    
  }
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
