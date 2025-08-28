#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<assert.h>
#include<string.h>

#define MAX_CHARS_IN_LINE 1000
int min(int a,int b){
	if(a<b)return a;
	else return b;
}
int main(int argc, char **argv)
{
	char buf[MAX_CHARS_IN_LINE+1];
	char obuf[MAX_CHARS_IN_LINE+1];

// NOTE: Do not modify anything above this line	
/***
 *      Your Code goes here
 */	
	int fd=open(argv[1],O_RDONLY);
	int fd_out =open(argv[2],O_WRONLY|O_CREAT,0644);
	/*
	off_t n=lseek(fd,0,SEEK_END);
	off_t k=lseek(fd,0,SEEK_SET);
	size_t total=0;
	char buf_temp[1000];
	int indx=0;
	size_t temp=0;
	ssize_t r=read(fd,buf,(size_t)(1000));
	printf("%ld\n",r);
	while(total<n){
		ssize_t r=read(fd,buf,(size_t)(1000));
		
		buf[1000]='\0';
		total+=r;
		printf("%ld\n",r);
			
		for(int i=0;i<temp;i++){
			if(buf[i]=='\n'){
				//process
				buf[indx]='\0';
				for(int i=0;i<1000;i++){
					if(buf_temp[i]=='\0'){printf("\n");break;}
					else printf("%c",buf_temp[i]);
				}
				indx=0;
		
			}
			else{
			    buf_temp[indx]=buf[i];
			    indx++;
			}
		}
		temp=0;
		
	}*/
	int parentc[2],childp[2];
	pipe(parentc);
	pipe(childp);

	pid_t pid=fork();
	if(pid==0){
		dup2(parentc[0],STDIN_FILENO);
		dup2(childp[1],STDOUT_FILENO);
		close(childp[0]);close(childp[1]);
		close(parentc[0]);close(parentc[1]);
		execl("./encrypt","encrypt",NULL);
		exit(-1);
	}
	else{
		close(childp[1]);close(parentc[0]);
		ssize_t n=0;
		while((n=read(fd,buf,1000))>0){
			int start=0;
			for(int i=0;i<n;i++){
				if(buf[i]=='\n'||i==n-1){
					char p[32];
					int s=sprintf(p,"%d\n",i-start+1);
                                        write(parentc[1],p,s);
					write(parentc[1],buf+start,i-start+1);
					start=i+1;
				}
			}
		}
		close(parentc[1]);
		while((n=read(childp[0],buf,1000))>0){
			write(fd_out,buf,n);
		}
		
	}	
	

	return 0;
}

