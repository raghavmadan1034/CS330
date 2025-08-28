#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>
#include <unistd.h>

// Use Library function such as fopen, geline, fread etc
// use manpages as mentioned in the writeup. 
// -----------------------
//
//
// Your solution begins here
int main(int argc, char *argv[]) {
        FILE *stream = fopen(argv[2], "r");
	char* line =NULL;
	size_t len=0;
	ssize_t nread;
	if(stream==NULL){
	     perror("fopen");
	     exit(EXIT_FAILURE);
	}
	int character=0,word=0,lines=0;
	while((nread=getline(&line,&len,stream))!=-1){
		character+=nread;
		lines++;
		if(line[0]!=' ' && line[0]!='\n')word++;
		for(int i=0;i<nread-1;i++){
			if(line[i+1]!=' ' && line[i]==' ')word++;
		}
	}
	if(strcmp(argv[1],"-c")==0)printf("%d\n",character);
	else if(strcmp(argv[1],"-w")==0)printf("%d\n",word);
	else if(strcmp(argv[1],"-l")==0) printf("%d\n",lines);
//	printf("%d %d %d\n",lines,character,word);
        else return -1;

	return 0;
}
