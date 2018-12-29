#include <stm32f30x.h>
#include <diskio.h>
#include <f3d_lcd_sd.h>
#include <f3d_uart.h>
#include <f3d_delay.h>
#include <f3d_systick.h>
#include <f3d_i2c.h>
#include <f3d_chart.h>
#include <ff.h>
#include <stdio.h>

#define CHECK_FILE(X) if(X) {				\
	printf ("DEAD l:%-4d", __LINE__);	\
	die(X);														\
}

void die (FRESULT rc);

FATFS Fatfs;		/* File system object   		*/
FIL Fil;				/* File object		  				*/
BYTE Buff[128];	/* File read buffer	 				*/

void main(void) {
	FRESULT rc;		/* Result code							*/
	DIR dir;			/* Directory object			   	*/
	FILINFO fno;	/* File information object	*/
	unsigned int retval;
	unsigned int bytes_read; /* Number of bytes read by f_read 	*/
	unsigned int num_of_songs = 4;
	unsigned int current_song = 0;
	unsigned int i;

	setvbuf(stdin, NULL, _IONBF, 0);
	setvbuf(stdout, NULL, _IONBF, 0);
	setvbuf(stderr, NULL, _IONBF, 0);

	f3d_systick_init();		delay(10);
	f3d_delay_init();	delay(10);
	f3d_led_init();		delay(10);
	f3d_i2c1_init();	delay(10);
	f3d_uart_init();	delay(10);
	f3d_lcd_init();		delay(10);

  f3d_lcd_fillScreen(BLACK);
  f3d_lcd_drawString(55, 40, "DDR", WHITE, BLACK);
  f3d_lcd_drawString(40, 50, "But With", WHITE, BLACK);
  f3d_lcd_drawString(30, 60, "Wii Nunchuk", WHITE, BLACK);
  f3d_lcd_drawString(10, 100, "Green Fret to Start", WHITE, BLACK);
  
	f_mount(0, &Fatfs);	/* Register volume work area */

	printf("\nOpen root directory.\n");
	rc = f_opendir(&dir, "");
	CHECK_FILE(rc);
	delay(200);

	printf("\nDirectory listing...\n");
	for (;;) {
		rc = f_readdir(&dir, &fno);	 	/* Read a directory item */
		if (rc || !fno.fname[0]) break; /* Error or end of dir   */
		if (fno.fattrib & AM_DIR)
			printf("   <dir>  %s\n", fno.fname);
		else
			printf("%8lu  %s\n", fno.fsize, fno.fname);
	}
	CHECK_FILE(rc);
}

void die (FRESULT rc) {
	printf("Failed with rc=%u.\n", rc);
	while (1);
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t * file, uint32_t line) {
  /* Infinite loop */
  /* Use GDB to find out why we're here */
  while (1);
}
#endif
