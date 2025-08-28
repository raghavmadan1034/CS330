#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
int hasprefix(char* buf,size_t pos,size_t,char* pat){

}
int main(int argc, char **argv)
{
    /**
     * TODO: Write your code here.
     */
     int cnt_openat=0,cnt_close=0,cnt_read=0,cnt_write=0,cnt_stat=0,cnt_execve=0;
     int fd=open(argv[1],O_RDONLY);
     if(fd<0){
	printf("Unable to execute\n");
   	return 0;
     }
     off_t n=lseek(fd,0,SEEK_END);
     off_t k=lseek(fd,0,SEEK_SET);
     char* buf=(char*)malloc((size_t)(n+1));
     size_t total=0;
     int cnt=0;
     while(total<n){
	   ssize_t r=read(fd,buf+total,(size_t)(n-total));
	   cnt++;
	   if(r<0){
		 close(fd);
		 free(buf);
		 printf("Unable to execute\n");
	       	 return 0;
           }
  	   if(r==0)break;
	   total+=r;
    }
    close(fd);
    buf[total]='\0';
    
    for(size_t i=0;i<total;i++){
	if(i+7<total && buf[i]==' ' && buf[i+1]=='o' && buf[i+2]=='p' && buf[i+3]=='e' && buf[i+4]=='n' && buf[i+5]=='a' && buf[i+6]=='t' && buf[i+7]=='(')cnt_openat++;
	if(i+6<total && buf[i]==' ' && buf[i+1]=='c' && buf[i+2]=='l' && buf[i+3]=='o' && buf[i+4]=='s' && buf[i+5]=='e' && buf[i+6]=='(')cnt_close++;
	if(i+5<total && buf[i]==' ' && buf[i+1]=='r' && buf[i+2]=='e' && buf[i+3]=='a' && buf[i+4]=='d' && buf[i+5]=='(')cnt_read++;
	if(i+6<total && buf[i]==' ' && buf[i+1]=='w' && buf[i+2]=='r' && buf[i+3]=='i' && buf[i+4]=='t' && buf[i+5]=='e'  && buf[i+6]=='(')cnt_write++;
	if(i+5<total && buf[i]==' ' && buf[i+1]=='s' && buf[i+2]=='t' && buf[i+3]=='a' && buf[i+4]=='t' && buf[i+5]=='(')cnt_stat++;
	if(i+7<total && buf[i]==' ' && buf[i+1]=='e' && buf[i+2]=='x' && buf[i+3]=='e' && buf[i+4]=='c' && buf[i+5]=='v' && buf[i+6]=='e' && buf[i+7]=='(')cnt_execve++;
    }
    printf("openat: %d\n",cnt_openat);
    printf("close: %d\n",cnt_close);
    printf("read: %d\n",cnt_read);
    printf("write: %d\n",cnt_write);
    printf("stat: %d\n",cnt_stat);
    printf("execve: %d\n",cnt_execve);    

    
     
  return 0;
}
