/* queue.c --- 
 * 
 * Filename: queue.c
 * Description: 
 * Author: Bryce Himebaugh
 * Maintainer: 
 * Created: Thu Feb  7 19:49:26 2013 (-0500)
 * Version: 
 * Last-Updated: 
 *           By: 
 *     Update #: 0
 * URL: 
 * Doc URL: 
 * Keywords: 
 * Compatibility: 
 * 
 */

/* Commentary: 
 * 
 * 
 * 
 */

/* Change Log:
 * 
 * 
 */

/* This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 3, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth
 * Floor, Boston, MA 02110-1301, USA.
 */

/* Code: */

#include "queue.h"

void init_queue(queue_t *buf) {
  buf->head = 0;
  buf->tail = 0;
}

int enqueue (queue_t *buf, int data) {
  if(queue_full(buf)){
    return 0;
  }
  buf->buffer[buf->tail] = data;
  buf->tail = (buf->tail + 1) % QUEUE_SIZE;
  return 1;
}

int dequeue (queue_t *buf, int *data) {
  if(queue_empty(buf)){
    return 0;
  }
  *data = buf->buffer[buf->head];
  buf->head = (buf->head + 1) % QUEUE_SIZE;
  return *data;
}

int queue_empty(queue_t *buf) {
  return buf->head == buf->tail;
}

int queue_full(queue_t *buf) {
  return ((buf->tail + 1 % QUEUE_SIZE) == buf->head);
}


/* queue.c ends here */
