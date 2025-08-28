#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<math.h>
#include<sys/time.h>
#include<sys/wait.h>


extern int isPrime(long x); // Returns zero if composite.
                            // Returns one if prime. You may use this if you want. 
int find_primes(long M)
{
  //Your code goes in here
  	int cnt1=0,cnt2=0,status;
	pid_t pid1,pid2;
	pid1=fork();
	if(pid1<0||pid2<0){
		perror("fork");
		exit(-1);
	}
	
	if(!pid1){
		for(int i=2;i<=M/2;i++){
			if(isPrime(i))cnt1++;
		}
	exit(cnt1);	
	}
	pid2=fork();
	if(!pid2){
		for(int i=M/2+1;i<=M;i++){
			if(isPrime(i))cnt2++;
		}
	exit(cnt2);
	}
	wait(&cnt1);
	wait(&cnt2);

	return (WEXITSTATUS(cnt1)+WEXITSTATUS(cnt2))%256;
	
	
	

   	
   return 0;   
}
