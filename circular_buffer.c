/*
  Copyright (C) 2013
  Fabien Gaud <fgaud@sfu.ca>, Baptiste Lepers <baptiste.lepers@inria.fr>,
  Fabien Mottet <fabien.mottet@inria.fr>

  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License
  version 2 or later, as published by the Free Software Foundation.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#include "circular_buffer.h"
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>

/******************open_circ********************/
circularBuffer * open_circ(circularBuffer *cb, int size){

	if(cb == NULL){
		return NULL;
	}

	cb->buffer = (void **)malloc((size+1) * sizeof(void *));

	if(cb->buffer == NULL)
	{
		return NULL;
	}
	else
	{
		bzero(cb->buffer, (size+1) * sizeof(void *));
		cb->readPos = 0;
		cb->writePos = 0;
		cb->size = size + 1;
	}

	return(cb);
}

void close_circ(circularBuffer *cb){

	if(cb == NULL){
		return;
	}

	free(cb->buffer);
	cb->readPos = 0;
	cb->writePos = 0;
	cb->size = 0;
}


/******************get_circ********************/
int get_circ(circularBuffer *cb, void ** value){

	//empty buffer
	if(cb->readPos == cb->writePos)
		return(CIRC_BUF_ERROR);

	*value = cb->buffer[cb->readPos];

	//Increment readPos and return the value pointed by it.
	if(++(cb->readPos)==cb->size)
		cb->readPos = 0;


	return 0;
}


/******************put_circ********************/
int put_circ(circularBuffer *cb, void * e){

	int p;

	//get where we will write
	p = cb->writePos + 1;

	//check if it's a valid position to write to
	if (p == cb->size)
	{
		p = 0;
	}
	if (p == cb->readPos)
	{
		return(CIRC_BUF_ERROR);
	}

	//If position is ok, we write the data and writePos goes to the next step.
	cb->buffer[cb->writePos] = e;
	cb->writePos = p;

	return 0;
}


/******************printfCircularBuffer********************/
void printfCircularBuffer(circularBuffer* cb ){

	int i;
	int size = (int)cb->size;

	printf("circBuffer-> size:%d, read_pos:%d, write_pos:%d\n", size, cb->readPos, cb->writePos);

	for(i=0 ; i< (size); i++){
		printf("\t|%p", cb->buffer[i]);
	}
	printf("\t|\n");
	for(i=0 ; i< (size); i++){
		printf("\t");
		if( i == cb->readPos && i == cb->writePos){
			printf(" rw");
		}
		else if( i == cb->writePos){
			printf(" w");
		}
		else if( i == cb->readPos){
			printf(" r");
		}
	}
	printf("\n");
}


//EOF
