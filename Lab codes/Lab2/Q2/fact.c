#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<math.h>
#include<sys/time.h>
#include<sys/wait.h>

int main(int argc, char **argv)
{
   /*Your code goes here */
   long long n=atoll(argv[1]);
   long long product=1; 
   if(argc>=3){
	   product=atoll(argv[2]);
    }
    if(n<=1){
          printf("%lld",product);
         return 0;
   }

   product*=n;
   char next_n_str[64];
   char prod_str[64];
   snprintf(next_n_str,sizeof(next_n_str),"%lld",n-1);
   snprintf(prod_str,sizeof(prod_str),"%lld",product);

   execl(argv[0],argv[0],next_n_str,prod_str,(char*)NULL);


   return 0;
}
