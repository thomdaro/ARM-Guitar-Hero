/* f3d_nunchuk.c --- 
 * 
 * Filename: f3d_nunchuk.c
 * Description: 
 * Author: Bryce Himebaugh
 * Maintainer: 
 * Created: Thu Oct 31 09:40:37 2013
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

#include <f3d_i2c.h>
#include <f3d_guitar.h>
#include <stdio.h>

#define NUNCHUK_ADDRESS 0xA4

void f3d_guitar_init(void) {
  uint8_t realb = 0x00;
  uint8_t buf = 0x55;
  uint8_t buf2 = 0x00;

  // Use for factory Wii Nunchuk controllers
  // f3d_i2c1_write(NUNCHUK_ADDRESS,0x40,&realb);
  // Use for 3 party nunchuks like we have in the lab
  f3d_i2c1_write(NUNCHUK_ADDRESS,0xf0,&buf); 
  Delay(100);
  printf("first address written\n");
  Delay(100);
  f3d_i2c1_write(NUNCHUK_ADDRESS,0xfb,&buf2);
  printf("second written\n");
}

void f3d_guitar_read(guitar_t *n) {
  uint8_t data[6] = {0,0,0,0,0,0};
  int i;
  unsigned short tmp;

  f3d_i2c1_read_nunchuk(0x08, data, 6);//device changed to x08
  n->sx = data[0];
  n->sy = data[1];
  n->tb = data[2];
  n->wb = data[3];
  n->b_one = data[4];
  n->b_two = data[5];
}

/* f3d_guitar.c ends here */
