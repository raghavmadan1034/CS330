#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#define ARRAY_SIZE(arr) (sizeof(arr)/sizeof((arr)[0]));
int main (int argc, char **argv) {

    /**
     * TODO: Write your code here.
     */
	if(argc!=3){
		printf("Error\n");
		return 0;
	}
	char * term =argv[1];
	char * path=argv[2];
	if(term[0]=='\0'){
		printf("Error\n");
		return 0;
	}
	int fd=open(path,O_RDONLY);
	if(fd<0){
		printf("Error\n");
		return 0;
	}
	off_t n=lseek(fd,0,SEEK_END);
	off_t k=lseek(fd,0,SEEK_SET);
	if(n<0){
		close(fd);
		printf("Error\n");
		return 0;
	}
	if(k<0){
		close(fd);
		printf("Error\n");
		return 0;
	}
	char * buf=(char*) malloc((size_t)(n+1));
	if(!buf){
		close(fd);
		printf("Error\n");
		return 0;
	}
	ssize_t total=0;
	while(total<n){
		ssize_t r=read(fd,buf+total,(size_t)(n-total));
		if(r<0){
			free(buf);
			close(fd);
			printf("Error\n");
			return 0;
		}
		if(r==0)break;
		total+=r;
	}
	close(fd);
	buf[total]='\0';
	if(strstr(buf,term)!=NULL){
		printf("FOUND\n");
	}
	else printf("NOT FOUND\n");
	free(buf);



    return 0;
}
