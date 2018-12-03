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
  int menu_drawn_pause = 0;
  int menu_item = 0;

  while(1){
    f3d_guitar_read(&gui);
    /*
    for(i=0;i<5;i++){
      printf("BUTTON %c|%d\n",button_vals[i],BUTTON(gui,i);
    }
    printf("\n\n");
    Delay(100);
    printf("BARDOWN %d\n",BARDOWN(gui));
    printf("BARUP   %d\n",BARUP(gui));
    Delay(1000);
    printf("MINUS   %d\n",MINUS(gui));
    printf("PLUS    %d\n",PLUS(gui));
    */
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
    if(!BARDOWN(gui)){
      menu_item = (menu_item + 1) % 3;
    }
    if(!BARUP(gui)){
      menu_item = (menu_item + 2) % 3;
    }
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
