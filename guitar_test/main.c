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
#define BUTTON(x, y) (((x.b_two) >> (3 + y)) & 0x1) //vals 0->4 y,g,b,r,o
const uint16_t fret_colors[5] = {YELLOW, GREEN, BLUE, RED, 0xDCE8};
const uint8_t fret_pos[5] = {2,4,1,3,0};
//const uint16_t pressed_colors[5] = {};

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


void draw_fret(fret_t *fret, uint16_t color){
  int draw_x_pos = ST7735_width  - (20 * fret->fret) - 10;
  draw_rectangle(draw_x_pos, fret->y_pos, draw_x_pos + 10, fret->y_pos + 5, color);
}

//colors frets that are currently pressed
void draw_active_frets(guitar_t gui){
  int i;
  int draw_x_pos;
  for(i=4;i>-1;i--){
    if(!BUTTON(gui,i)){
      draw_x_pos = ST7735_width  - (20 * fret_pos[i]) - 10;
      draw_rectangle(draw_x_pos, ST7735_height - 5, draw_x_pos + 10, ST7735_height, fret_colors[i]);
      }
  }
}

void draw_inactive_frets(guitar_t gui){
  int i;
  int draw_x_pos;
  for(i=4;i>-1;i--){
    if(BUTTON(gui,i)){
      draw_x_pos = ST7735_width  - (20 * fret_pos[i]) - 10;
      draw_rectangle(draw_x_pos, ST7735_height - 5, draw_x_pos + 10, ST7735_height, BLACK);
      }
  }
}

void drop_fret(fret_t *fret){// drops frets and stops them at the end of the board
  if(fret->y_pos < ST7735_height){
    fret->y_pos++;
  }
  else{
    fret->y_pos = ST7735_height - 5;
    fret->active = 0;
  }
}

void drop_frets(fret_t frets[20]){ //drops all frets marked ACTIVE in list
  int i;
  for(i=0;i<20;i++){
    if(frets[i].active){
      draw_fret(&frets[i], WHITE);
      drop_fret(&frets[i]);
      draw_fret(&frets[i], BLACK);
    }
  }
}

//draw game board if switching game state & controls fret drop / rate of drop
void draw_game(int game_ctl,int* game_drawn,fret_t frets[20], int d_rate){
  if(game_ctl == 0){
    if(!(*game_drawn)){
      f3d_lcd_fillScreen(WHITE);
      //frets
      draw_rectangle(ST7735_width - 10, ST7735_height - 5, ST7735_width, ST7735_height, BLACK);
      draw_rectangle(ST7735_width - 30, ST7735_height - 5, ST7735_width - 20, ST7735_height, BLACK);
      draw_rectangle(ST7735_width - 50, ST7735_height - 5, ST7735_width - 40, ST7735_height, BLACK);
      draw_rectangle(ST7735_width - 70, ST7735_height - 5, ST7735_width - 60, ST7735_height, BLACK);
      draw_rectangle(ST7735_width - 90, ST7735_height - 5, ST7735_width - 80, ST7735_height, BLACK);
      //hittable zone??? this deletes if a fret goes through it so maybe not
      draw_rectangle(ST7735_width - 90, ST7735_height - 30, ST7735_width, ST7735_height - 27, BLACK);
      //divider zone......messed up doesn't draw full length =_= THIS IS A SIGNIFICANT ISSUE
      //the function seems to be valid but the draw won't finish.
      draw_rectangle(ST7735_width - 100, 0, ST7735_width - 98, ST7735_height, BLACK);
      *game_drawn = 1;
    }
    draw_inactive_frets(gui);
    draw_active_frets(gui);
    drop_frets(frets);
    Delay(d_rate);
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
  
  int d_rate = 1;//currently drop rate
  char button_vals[5] = {'y','g','b','r','o'};
  int game_ctl = 0;//controls which menu or on or whether we are in game
  int menu_drawn_pause = 0;//tells us whether the menu is on screen/if we shoudl redraw 
  int game_drawn = 0;//same as above, with game
  int menu_item =  0;//value of current menu item selected
  fret_t testnotes[20] = {{0}};//these represent notes in the game

  
  testnotes[0].fret = 3;
  testnotes[0].y_pos = 0;
  testnotes[0].active = 1;

  testnotes[1].fret = 0;
  testnotes[1].y_pos = 50;
  testnotes[1].active = 1;
  while(1){
    f3d_guitar_read(&gui);
    draw_game(game_ctl, &game_drawn, testnotes, d_rate);
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
