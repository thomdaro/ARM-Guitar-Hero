/* f3d_systick.c --- 
 * 
 * Filename: f3d_systick.c
 * Description: 
 * Author: Bryce Himebaugh
 * Maintainer: 
 * Created: Thu Nov 14 07:57:37 2013
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

/* Copyright (c) 2004-2007 The Trustees of Indiana University and 
 * Indiana University Research and Technology Corporation.  
 * 
 * All rights reserved. 
 * 
 * Additional copyrights may follow 
 */

/* Code: */

#include <f3d_systick.h>
#include <f3d_led.h> 
#include <f3d_uart.h>

volatile int systick_flag = 0;


void f3d_systick_init(void) {
  SysTick_Config (SystemCoreClock / 100);
}

void f3d_systick_config(int i){
  SysTick_Config(SystemCoreClock / i);
}

void SysTick_Handler(void) {
  if(systick_flag != 0x00){
    systick_flag--;
  }
}

void Delay(uint32_t nTime){
  systick_flag = nTime;
  while(systick_flag != 0);
}


/* f3d_systick.c ends here */
