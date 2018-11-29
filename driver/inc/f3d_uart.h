/* f3d_uart.h ---
 *
 * Filename: f3d_uart.h
 * Description:
 * Author: Bryce Himebaugh
 * Maintainer:
 * Created: Thu Sep 26 07:05:43 2013
 * Last Modified by: dgalants
 * Date Last Modified: 09/26/2018
 * Assignment: Lab4
 *
 */


/* Commentary:
 *
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

void f3d_uart_init(void);
int putchar(int);
int getchar(void);
int getchar_non_blocking(void);
void putstring(char *);

/* f3d_uart.h ends here */
