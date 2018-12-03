/* main.c --- 
 * 
 * Filename: main.c
 * Description: 
 * Author: 
 * Maintainer: 
 * Created: Thu Jan 10 11:23:43 2013
 * Last-Updated: 
 *           By: 
 *     Update #: 0
 * Keywords: 
 * Compatibility: 
 * 
 */

/* Commentary: 
 * 
 * 
 * 
 */

/* Change log:
 * 
 * 
 */
/* Code: */

#include <stm32f30x.h>  // Pull in include files for F30x standard drivers 
#include <f3d_led.h>     // Pull in include file for the local drivers
#include <f3d_uart.h>
#include <f3d_gyro.h>
#include <f3d_lcd_sd.h>
#include <f3d_i2c.h>
#include <f3d_accel.h>
#include <f3d_mag.h>
#include <f3d_nunchuk.h>
#include <f3d_guitar.h>
#include <f3d_rtc.h>
#include <f3d_systick.h>
#include <ff.h>
#include <diskio.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BARDOWN(x) (((x.b_one) >> 6) & 0x1)
#define BARUP(x) ((x.b_two) & 0x1)
#define MINUS(x) (((x.b_one) >> 4) & 0x1)
#define PLUS(x) (((x.b_one) >> 2) & 0x1)
#define BUTTON(x, y) (((x.b_two) >> (3 + y)) & 0x1)) //vals 0->4 y,g,b,r,o

guitar_t gui;

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
  f3d_lcd_drawString(x, y, str, WHITE, BLACK);
}

void draw_pause(int menu_ctl, int *menu_drawn, int *menu_item, guitar_t gui){ //menuctl = 4 for pause
  if(menu_ctl == 4){
    if(!(*menu_drawn)){
      f3d_lcd_fillScreen(BLACK);
      f3d_lcd_drawString(55, 40, "PAUSE", WHITE, BLACK);
      f3d_lcd_drawString(40, 60, "Resume", WHITE, BLACK);
      f3d_lcd_drawString(40, 70, "Restart", WHITE, BLACK);
      f3d_lcd_drawString(40, 80, "Song Select", WHITE, BLACK);
      f3d_lcd_drawString(40, 90, "Main Menu", WHITE, BLACK);
      *menu_drawn = 1;
    }  
    if(!BARDOWN(gui)){
      *menu_item = (*menu_item + 1) % 4;
    }
    if(!BARUP(gui)){
      *menu_item = (*menu_item + 3) % 4;
    }
    switch(*menu_item){
    case 0:
      selected_item(40, 70, "Restart");
      break;
    case 1:
      selected_item(40, 80, "Song Select");
      break;
    case 2:
      selected_item(40, 90, "Main Menu");
      break;
    case 3:
      selected_item(40, 60, "Resume");
      break;
    }
  }
}

void draw_game(int game_ctl,int* game_drawn){
  if(game_ctl == 0){
    if(!(*game_drawn)){
      //frets
      f3d_lcd_fillScreen(WHITE);
      draw_rectangle(ST7735_width - 10, ST7735_height - 5, ST7735_width, ST7735_height, BLACK);
      draw_rectangle(ST7735_width - 30, ST7735_height - 5, ST7735_width - 20, ST7735_height, BLACK);
      draw_rectangle(ST7735_width - 50, ST7735_height - 5, ST7735_width - 40, ST7735_height, BLACK);
      draw_rectangle(ST7735_width - 70, ST7735_height - 5, ST7735_width - 60, ST7735_height, BLACK);
      draw_rectangle(ST7735_width - 90, ST7735_height - 5, ST7735_width - 80, ST7735_height, BLACK);
      //hittable zone
      draw_rectangle(ST7735_width - 90, ST7735_height - 30, ST7735_width, ST7735_height - 27, BLACK);
      //divider zone......messed up doesn't draw full length =_= THIS IS A SIGNIFICANT ISSUE
      //the function seems to be valid but the draw won't finish.
      draw_rectangle(ST7735_width - 100, 0, ST7735_width - 98, ST7735_height, BLACK);
      *game_drawn = 1;
    }
  }
    
}
int main(void) { 
  setvbuf(stdin, NULL, _IONBF, 0);
  setvbuf(stdout, NULL, _IONBF, 0);
  setvbuf(stderr, NULL, _IONBF, 0);

  f3d_systick_init();
  Delay(10);
  f3d_i2c1_init();
  Delay(10);
  f3d_uart_init();
  Delay(10);
  f3d_lcd_init();
  Delay(10);
  f3d_timer2_init();
  Delay(10);
  f3d_dac_init();
  Delay(10);
  f3d_delay_init();
  Delay(10);
  f3d_rtc_init();
  Delay(10);
  Delay(100);
  f3d_guitar_init();
  Delay(10);
  
  int i;
  char button_vals[5] = {'y','g','b','r','o'};
  int game_ctl = 4;
  int menu_drawn_pause = 0;
  int game_drawn = 0;
  int menu_item = 0;
  
  while(1){
    f3d_guitar_read(&gui);
    draw_game(game_ctl, &game_drawn);
    draw_pause(game_ctl, &menu_drawn_pause, &menu_item, gui);
    
  }
  
  
  while (1){
    f3d_lcd_fillScreen(RED);
    f3d_lcd_fillScreen(BLUE);
    f3d_lcd_fillScreen(RED);
  }
  
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line) {
  /* Infinite loop */
  /* Use GDB to find out why we're here */
  while (1);
}
#endif


/* main.c ends here */
