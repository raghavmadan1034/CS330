#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<assert.h>
#include<sys/mman.h>
#include <stdint.h>
#include<stdbool.h>
long how_large(void *address)
{
  /*
   *  TODO: Your code
   */ 
   long pagesize=sysconf(_SC_PAGESIZE);
   if(pagesize==0)pagesize=4096;
   long a =((long)address)&(~(pagesize-1));
   
   
   void * res=mmap((void*)a,pagesize, PROT_READ|PROT_WRITE, MAP_ANONYMOUS|MAP_PRIVATE,0,0);
   //printf("hello1");
   if(res==(void*)a){
	   munmap(res,pagesize);
	   return 0;
   }

   void *start=(void*)a;
   //printf("hello1.2");
   while(1){
	  //printf("hello");
	   void*  prev=start-pagesize;
	   void * res=mmap(prev,pagesize,PROT_READ|PROT_WRITE,MAP_ANONYMOUS|MAP_PRIVATE,0,0);
	   if(res==(void*)prev){
		   munmap(res,pagesize);
		   break;
	   }
	   start=prev;
   }
   //printf("hello");
   void* end=(void*)a+pagesize;
   while(1){
	   void * res=mmap((void*)end,pagesize,PROT_READ|PROT_WRITE,MAP_ANONYMOUS|MAP_PRIVATE,0,0);
	   if(res==(void*)end){
		   munmap(res,pagesize);
		   break;
	   }
	   end+=pagesize;
   }
   //printf("hello3");
  
  //printf("%ld",(long)(end-start));

  return end-start;    
}
