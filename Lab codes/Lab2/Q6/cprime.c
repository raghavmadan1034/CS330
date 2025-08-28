#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<math.h>
#include<sys/time.h>
#include<sys/wait.h>
#include<string.h>
#include<sys/syscall.h>
extern int isPrime(long x); // Returns zero if composite.
                            // Returns one if prime. 
long min(long a, long b){
	if(a<b)return a;
	else return b;
}
long find_primes(int num_cp, long M)
{
   /*
    *  TODO 
    *  Your code goes here
    */
    long base=M/num_cp;
    long rem=M%num_cp;
    
    int (*pipes)[2]=malloc(sizeof(int[2])*num_cp);
    for(int i=0;i<num_cp;i++)syscall(SYS_pipe,pipes[i]);
    for(int i=0;i<num_cp;i++){
	    pid_t pid=fork();
	    if(pid==0){
		long start=i*base+min(i,rem)+1;
		long extra=0;
		if(i<rem)extra=1;
		long end=start+base+extra-1;
		long local_cnt=0;
		for(int j=start;j<=end;j++){
			if(isPrime(j))local_cnt++;
		
		}
		ssize_t w=write(pipes[i][1],&local_cnt,sizeof(local_cnt));
		exit(local_cnt);
	    }
     }
    long ans=0;
    for(int i=0;i<num_cp;i++){
	    long local_cnt=0;
	    ssize_t r=read(pipes[i][0],&local_cnt,sizeof(local_cnt));
	    ans+=local_cnt;
    }
	
   return ans;   
}
