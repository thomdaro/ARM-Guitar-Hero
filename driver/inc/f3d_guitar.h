/* f3d_nunchuk.h --- 
 * 
 * Filename: f3d_nunchuk.h
 * Description: 
 * Author: Bryce Himebaugh
 * Maintainer: 
 * Created: Thu Oct 31 09:41:40 2013
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
#include <stm32f30x.h>

typedef struct guitar_data { 
  unsigned char sx;//1b bits 0->5
  unsigned char sy;//1b bits 0->5
  unsigned char tb;//1b bits 0->4
  unsigned char wb;//1b bits 0->4
  unsigned char b_one;//1b can we make these last two one? bits 2,4,6 == +,-,bardown
  unsigned char b_two;//"" bits 0,3->7 == barup, ylw,grn,blu,red,orng
} guitar_t; 

typedef struct fret_data {
  uint8_t fret; //0->4
  uint8_t y_pos;
  uint8_t active;
} fret_t;

void f3d_guitar_init(void);
void f3d_guitar_read(guitar_t *);

/* f3d_nunchuk.h ends here */
