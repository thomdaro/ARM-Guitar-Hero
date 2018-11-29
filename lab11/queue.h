/**********************************************************
 * queue.h
 * This program demonstrates uart input/output possibilities
 * Author: dgalants
 * Date Created: 11/05/2018
 * Last Modified by: dgalants
 * Date Last Modified: 11/05/2018
 * Assignment: Lab10
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

#define QUEUE_SIZE 32
/* add 1 in round-robin manner */
#define ADD1(x) ((x) + 1) % QUEUE_SIZE
/* subtract 1 in round-robin manner */
#define SUB1(x) ((x) + QUEUE_SIZE - 1) % QUEUE_SIZE

typedef struct queue {
	int head;
	int tail;
	int buffer[QUEUE_SIZE];
} queue_t;

void init_queue(queue_t *);
int enqueue(queue_t *, int);
int dequeue(queue_t *);
int queue_empty(queue_t *);

/* queue.h ends here */
