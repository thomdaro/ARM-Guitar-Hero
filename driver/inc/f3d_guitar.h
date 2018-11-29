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
  unsigned char jx;
  unsigned char jy;
  unsigned short ax;
  unsigned short ay;
  unsigned short az;
  unsigned char c;
  unsigned char z;
} guitar_t; 

void f3d_guitar_init(void);
void f3d_guitar_read(guitar_t *);

/* f3d_nunchuk.h ends here */
