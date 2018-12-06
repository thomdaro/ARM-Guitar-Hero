#include <stm32f30x.h>
#include <f3d_lcd_sd.h>
#include <f3d_uart.h>
#include <f3d_delay.h>
#include <f3d_guitar.h>
#include <f3d_i2c.h>
#include <stdio.h>
#include <ff.h>
#include <diskio.h>
#include "chart.h"

#define BARDOWN(x) (((x.b_one) >> 6) & 0x1)
#define BARUP(x) ((x.b_two) & 0x1)
#define MINUS(x) (((x.b_one) >> 4) & 0x1)
#define PLUS(x) (((x.b_one) >> 2) & 0x1)
#define BUTTON(x, y) (((x.b_two) >> (3 + y)) & 0x1) //vals 0->4 y,g,b,r,o

FATFS Fatfs;		/* File system object   */
FIL Fil;			/* File object		  */
BYTE Buff[128];		/* File read buffer	 */
guitar_t gui;

void die (FRESULT rc) {
	printf("Failed with rc=%u.\n", rc);
	while (1);
}

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
	FRESULT rc;				 	/* Result code					*/
	DIR dir;				 		/* Directory object			   */
	FILINFO fno;			 	/* File information object		*/

  setvbuf(stdin, NULL, _IONBF, 0);
  setvbuf(stdout, NULL, _IONBF, 0);
  setvbuf(stderr, NULL, _IONBF, 0);

  f3d_timer2_init();
  f3d_delay_init();
  f3d_rtc_init();
  f3d_systick_init();
  f3d_uart_init();
  f3d_lcd_init();

	f_mount(0, &Fatfs);		/* Register volume work area (never fails) */

  f3d_lcd_fillScreen(BLACK);

	rc = f_opendir(&dir, "/charts");
	printf("A:");
	if (rc) die(rc);
	rc = f_open(&Fil, "/charts/notes.txt", FA_READ);
	printf("B:");
	if (rc) die(rc);
	song_t 			s_song = {0};
	sync_track_t 	s_sync = {0};
	event_t 		s_event = {0};
	single_t 		s_sing = {0};

	uint16_t i = 20;

	printf("\nDirectory listing...\n");
	for (;;) {
		rc = f_readdir(&dir, &fno);			// Read a directory item
		if (rc || !fno.fname[0]) break;	// Error or end of dir
		f3d_lcd_drawString(40, i, fno.fname, GREEN, BLACK);
		i += 10;
	}
	if (rc) die(rc);

	Delay(10000);

	/*
	init_song(&Fil, &s_song);
	init_sync_track(&Fil, &s_sync);
	init_event(&Fil, &s_event);
	init_single(&Fil, &s_sing);

  f3d_lcd_drawString(10, 20, "NAME:", GREEN, BLACK);
  f3d_lcd_drawString(10, 30, s_song.name, WHITE, BLACK);
  f3d_lcd_drawString(10, 50, "ARTIST:", GREEN, BLACK);
  f3d_lcd_drawString(10, 60, s_song.artist, WHITE, BLACK);
  f3d_lcd_drawString(10, 70, "GENRE:", GREEN, BLACK);
  f3d_lcd_drawString(10, 80, s_song.genre, WHITE, BLACK);
  f3d_lcd_drawString(10, 90, "PATH:", GREEN, BLACK);
  f3d_lcd_drawString(10, 100, s_song.music_stream, WHITE, BLACK);
	*/

	f_close(&Fil); // NOTE: MAKE SURE YOU DON'T CALL IT RIGHT AFTER INITs. WILL BREAK THE CARD

}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t * file, uint32_t line) {
  /* Infinite loop */
  /* Use GDB to find out why we're here */
  while (1);
}
#endif
