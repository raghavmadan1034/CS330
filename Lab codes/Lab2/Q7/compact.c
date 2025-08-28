#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

void compact(void *start, void *end)
{
  /* 
   * TODO
   * You
   * r code goes here*/


	char * r = (char*)start;
	char * w = (char*) start;
	char * e = (char*) end;

	for(;r<e;r++){
		if(*r != '\0'){
			if(*r!=*w) *w = *r;
			w++;
		}}
	intptr_t delta =( intptr_t)(w-e);

	if(delta!=0) (void)sbrk(delta);
}  
