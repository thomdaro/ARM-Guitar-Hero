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
#include <ff.h>
#include <f3d_gyro.h>
#include <f3d_lcd_sd.h>
#include <f3d_i2c.h>
#include <f3d_accel.h>
#include <f3d_mag.h>
#include <f3d_nunchuk.h>
#include <f3d_guitar.h>
#include <f3d_rtc.h>
#include <f3d_systick.h>
#include <diskio.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <chart.h>

//guitar macros and corresponding button sequences + colors
#define BARDOWN(x) (((x.b_one) >> 6) & 0x1)
#define BARUP(x) ((x.b_two) & 0x1)
#define MINUS(x) (((x.b_one) >> 4) & 0x1)
#define PLUS(x) (((x.b_one) >> 2) & 0x1)
#define BUTTON(x, y) (((x.b_two) >> (3 + y)) & 0x1) //vals 0->4 y,g,b,r,o
#define CHAR_MAX 10
const char char_table[CHAR_MAX] = "0123456789";
guitar_t gui;


//File system stuff
FATFS Fatfs;		/* File system object*/
FIL Fil;		/* File object	     */
BYTE Buff[128];		/* File read buffer  */
/*
  FRESULT rc;	
  FIL fid;
  DIR dir;			
  FILINFO fno;		
*/
/*
void die (FRESULT rc) {
	printf("Failed with rc=%u.\n", rc);
	while (1);
}
*/

int main(void) { 
  /*file objects*/
  
  FRESULT rc;	
  FIL fid;
  DIR dir;			
  FILINFO fno;		
  
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
  f3d_guitar_init();
  
  f_mount(0,&Fatfs);

	f3d_lcd_fillScreen(WHITE);
	f3d_lcd_fillScreen(BLACK);

	f3d_lcd_drawString(20, 10, "Press   or   ", WHITE, BLACK);
	f3d_lcd_drawChar(56, 10, ' ', GREEN, GREEN);
	f3d_lcd_drawChar(86, 10, ' ', RED, RED);
	f3d_lcd_drawString(20, 20, "Input song num:", GREEN, BLACK);
	f3d_lcd_drawString(20, 30, "   ", GREEN, BLACK);
	f3d_lcd_drawString(44, 34, "___", GREEN, BLACK);

	// filename
	char input[12] = "song000.txt";
	uint8_t char_pos = 0;
	uint8_t char_sel = 0;
	f3d_guitar_read(&gui);
	Delay(10);
	f3d_lcd_drawString(20, 30, input, GREEN, BLACK);

	while (BUTTON(gui, 3)) {
		f3d_guitar_read(&gui);
		if (!BARUP(gui))
			char_sel = (char_sel + CHAR_MAX - 1) % CHAR_MAX;
		if (!BARDOWN(gui))
			char_sel = (char_sel + 1) % CHAR_MAX;
		if (!BUTTON(gui, 1)) {
			char_pos = (char_pos + 1) % 3;
			char_sel = 0;
		}
		input[char_pos+4] = char_table[char_sel];
		f3d_lcd_drawChar(44, 30, input[4], GREEN, BLACK);
		f3d_lcd_drawChar(50, 30, input[5], GREEN, BLACK);
		f3d_lcd_drawChar(56, 30, input[6], GREEN, BLACK);
		Delay(10);
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
