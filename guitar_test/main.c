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
const uint16_t fret_colors[5] = {0x06BF, BLUE, YELLOW, RED, GREEN};
const uint8_t fret_pos[5] = {2,4,1,3,0};
const uint8_t button_pos[5] = {4,2,0,3,1};
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

//read chart from sd card
void read_chart(song_t *s,  sync_track_t *syn, event_t *ev, single_t *notes, FRESULT rc, DIR dir, FILINFO fno){
  f_mount(0, &Fatfs);		/* Register volume work area (never fails) */
  rc = f_opendir(&dir, "/charts");
  printf("A:");
  if (rc) die(rc);
  rc = f_open(&Fil, "/charts/notes.txt", FA_READ);
  printf("B:");
  if (rc) die(rc);
  init_song(&Fil,s);
  init_sync_track(&Fil, syn);
  init_event(&Fil, ev);
  init_single(&Fil, notes);
  Delay(1000);

  f_close(&Fil);
}


void draw_countdown(int *countdown) {
  draw_rectangle(50, 5, 75, 20, WHITE);
  draw_rectangle(52, 7,  73, 18, BLACK);
  f3d_lcd_drawChar(60, 10, '3', WHITE, BLACK);
  Delay(62000);
  f3d_lcd_drawChar(60, 10, '2', WHITE, BLACK);
  Delay(62000);
  f3d_lcd_drawChar(60, 10, '1', WHITE, BLACK);
  Delay(62000);
  draw_rectangle(50, 5, 75, 20, BLACK);
  draw_rectangle(63, 5, 63, 20, WHITE);
  *countdown = 0;
}

void draw_pause(int *menu_ctl, int *menu_drawn, int *menu_item, guitar_t gui, int* countdown, int* play_menu_sound){ //menuctl = 4 for pause
  if(*menu_ctl == 4){
    if(!(*menu_drawn)){
      f3d_lcd_fillScreen(BLACK);
      f3d_lcd_drawString(55, 40, "PAUSE", WHITE, BLACK);
      f3d_lcd_drawString(40, 60, "Resume", WHITE, BLACK);
      f3d_lcd_drawString(40, 70, "Restart", WHITE, BLACK);
      f3d_lcd_drawString(40, 80, "Song Select", WHITE, BLACK);
      f3d_lcd_drawString(40, 90, "Main Menu", WHITE, BLACK);
      f3d_lcd_drawChar(30, 60, '>', WHITE, BLACK);
      *menu_drawn = 1;
    }  
    if(!BARDOWN(gui)){
      *menu_item = (*menu_item + 1) % 4;
      f3d_lcd_drawChar(30, (*menu_item) % 4 * 10 + 60, '>', WHITE, BLACK);
      if (*menu_item != 0) f3d_lcd_drawChar(30, (*menu_item - 1) % 4 * 10 + 60, '>', BLACK, BLACK);
      else f3d_lcd_drawChar(30, 90, '>', BLACK, BLACK);
      *play_menu_sound = 1;
      Delay(10000);
    }
    if(!BARUP(gui)){
      *menu_item = (*menu_item + 3) % 4;
      f3d_lcd_drawChar(30, (*menu_item) % 4 * 10 + 60, '>', WHITE, BLACK);
      f3d_lcd_drawChar(30, (*menu_item + 1) % 4 * 10 + 60, '>', BLACK, BLACK);
      *play_menu_sound = 1;
      Delay(10000);
    }
    if ((*menu_item == 0 && !BUTTON(gui, 1)) || !MINUS(gui)) {
      *menu_ctl = 0;
      *menu_drawn = 0;
      *countdown = 1;
    }
  }
}

void draw_fret(fret_t *fret, uint16_t color){
  int draw_x_pos = ST7735_width  - (20 * fret->fret) - 30;
  draw_rectangle(draw_x_pos, fret->y_pos, draw_x_pos + 10, fret->y_pos + 5, color);
  f3d_lcd_drawPixel((uint8_t)(draw_x_pos + 5), (uint8_t)fret->y_pos, WHITE);
  if (fret->y_pos > ST7735_height - 30 && fret->y_pos <= ST7735_height - 26)
    draw_rectangle(draw_x_pos, fret->y_pos - 1, draw_x_pos + 10, fret->y_pos - 1, WHITE);
}

int check_strummed(fret_t *fret) {
  if (fret->y_pos >= ST7735_height - 35 && fret->y_pos <= ST7735_height - 26 && 
      !BUTTON(gui, button_pos[fret->fret]) && (!BARDOWN(gui) || !BARUP(gui))) {
    fret->active = 0;
    int draw_x_pos = ST7735_width - (20 * fret->fret) - 30;
    draw_rectangle(draw_x_pos, ST7735_height - 35, draw_x_pos + 4, ST7735_height - 30, BLACK);
    draw_rectangle(draw_x_pos + 6, ST7735_height - 35, draw_x_pos + 10, ST7735_height - 30, BLACK);
    draw_rectangle(draw_x_pos + 5, ST7735_height - 35, draw_x_pos + 5, ST7735_height - 30, WHITE);
    draw_rectangle(draw_x_pos, ST7735_height - 30, draw_x_pos + 10, ST7735_height - 27, WHITE);
  }
  return fret->active;
}

//colors frets that are currently pressed
void draw_active_frets(guitar_t gui){
  int i;
  int draw_x_pos;
  for(i=4;i>-1;i--){
    if(!BUTTON(gui,i)){
      draw_x_pos = ST7735_width  - (20 * fret_pos[i]) - 30;
      draw_rectangle(draw_x_pos, ST7735_height - 5, draw_x_pos + 10, ST7735_height, fret_colors[fret_pos[i]]);
      }
  }
}

void draw_inactive_frets(guitar_t gui){
  int i;
  int draw_x_pos;
  for(i=4;i>-1;i--){
    if(BUTTON(gui,i)){
      draw_x_pos = ST7735_width  - (20 * fret_pos[i]) - 30;
      draw_rectangle(draw_x_pos, ST7735_height - 5, draw_x_pos + 10, ST7735_height, WHITE);
      }
  }
}

void drop_fret(fret_t *fret){// drops frets and stops them at the end of the board
  
  if(fret->y_pos < ST7735_height){
    fret->y_pos++;
  }
  else {
    fret->y_pos = ST7735_height - 5;
    fret->active = 0;
  }
}

void drop_frets(fret_t frets[20]){ //drops all frets marked ACTIVE in list
  int i;
  for(i=0;i<20;i++){
    if(frets[i].active){
      draw_fret(&frets[i], BLACK); 
      draw_rectangle(&frets[i].y_pos, ST7735_width - (20 * (int)(&frets[i].fret)) - 30,
		     &frets[i].y_pos + 5, ST7735_width - (20 * (int)(&frets[i].fret)) - 30);
      drop_fret(&frets[i]);
      if (check_strummed(&frets[i]))
	draw_fret(&frets[i], fret_colors[frets[i].fret]);
    }
  }
}

//should take single_t ptr actually, doesn't handle removing used notes.
void add_frets(int counter, single_t *notes, fret_t frets[50]){
  int i;
  single_t temp;
  if(notes->time == counter){
    for(i=0;i<50;i++){
      if(!frets[i].active){
	frets[i].fret = notes->num0;
	frets[i].active = 1;
	frets[i].y_pos = 0;
	temp = *(notes->next);
	*notes = temp;
	break;
      }
    }
    add_frets(counter,&temp,frets);
  }
}

//draw game board if switching game state & controls fret drop / rate of drop
void draw_game(int game_ctl,int* game_drawn, fret_t frets[20], int d_rate, int* counter, int* countdown){
  if(game_ctl == 0){
    if(!(*game_drawn)){
      f3d_lcd_fillScreen(BLACK);
      //frets
      draw_rectangle(ST7735_width - 30, ST7735_height - 5, ST7735_width-20, ST7735_height, WHITE);
      draw_rectangle(ST7735_width - 50, ST7735_height - 5, ST7735_width - 40, ST7735_height, WHITE);
      draw_rectangle(ST7735_width - 70, ST7735_height - 5, ST7735_width - 60, ST7735_height, WHITE);
      draw_rectangle(ST7735_width - 90, ST7735_height - 5, ST7735_width - 80, ST7735_height, WHITE);
      draw_rectangle(ST7735_width - 110, ST7735_height - 5, ST7735_width - 100, ST7735_height, WHITE);
      // "strings"
      draw_rectangle(ST7735_width - 25, 0, ST7735_width - 25, ST7735_height, WHITE);
      draw_rectangle(ST7735_width - 45, 0, ST7735_width - 45, ST7735_height, WHITE);
      draw_rectangle(ST7735_width - 65, 0, ST7735_width - 65, ST7735_height, WHITE);
      draw_rectangle(ST7735_width - 85, 0, ST7735_width - 85, ST7735_height, WHITE);
      draw_rectangle(ST7735_width - 105, 0, ST7735_width - 105, ST7735_height, WHITE);
      // hittable zone
      draw_rectangle(ST7735_width - 120, ST7735_height - 30, ST7735_width - 10, ST7735_height - 27, WHITE);
      // dividers
      draw_rectangle(ST7735_width - 122, 0, ST7735_width - 120, ST7735_height, WHITE);
      draw_rectangle(ST7735_width - 10,  0, ST7735_width - 8,  ST7735_height, WHITE);
      *game_drawn = 1;
    }
    //add_frets(counter, single_t notes, frets);
    draw_inactive_frets(gui);
    draw_active_frets(gui);
    drop_frets(frets);
    if (*countdown) {
      draw_countdown(countdown);
    }
    Delay(d_rate);
    *counter = *counter + 1;
  }
}



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
  Delay(10);
  f3d_gyro_init();
  
  f_mount(0,&Fatfs);

  /*Char data structs */
  song_t song_current;
  sync_track_t sync;
  event_t events;
  single_t notes;
  
  read_chart(&song_current,&sync,&events,&notes,rc,dir,fno);

  int d_rate = 100;//currently drop rate
  int counter = 0;//current number of ticks into game
  char button_vals[5] = {'y','g','b','r','o'};
  int game_ctl = 0;//controls which menu or on or whether we are in game
  int menu_drawn_pause = 0;//tells us whether the menu is on screen/if we shoudl redraw 
  int game_drawn = 0;//same as above, with game
  int menu_item =  0;//value of current menu item selected
  int play_menu_sound = 0;

  fret_t testnotes[50] = {{0}};//these represent notes in the game
  int countdown =  0;//determines when to draw unpause countdown



  float gyro_vals[3] = {0,0,0};

  testnotes[0].fret = 2;
  testnotes[0].y_pos = 0;
  testnotes[0].active = 1;
  playAudio(rc,dir,fno,Fatfs,fid,Buff,"sound2.wav\0");
  
  f3d_systick_config(62000); //we should use this and d_rate to configure BPM
  while(1){
    f3d_guitar_read(&gui);
    f3d_gyro_getdata(gyro_vals);
    //plus or shake to pause
    if(!PLUS(gui) || gyro_vals[0] < -10 || gyro_vals[0] > 10){
      game_ctl = 4;
      game_drawn = 0;
    }

    add_frets(counter,&notes,testnotes);
    printf("counter %d\n",counter);
    draw_game(game_ctl, &game_drawn, testnotes, d_rate, &counter, &countdown);
    draw_pause(&game_ctl, &menu_drawn_pause, &menu_item, gui, &countdown, &play_menu_sound);
    if(play_menu_sound){
      playAudio(rc,dir,fno,Fatfs,fid,Buff,"hit.wav\0");
      play_menu_sound = 0;}
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
