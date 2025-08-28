#include <stdio.h>
#include <stdlib.h>
#include "buffer_api.h"

int main(int argc, char* argv[]) {
	if(argc != 2){
		perror("usage: ./solution <testcase number>");
		return -1;
	}
	buffer_init(atoi(argv[1]));
	const struct ring_buffer *rb = buffer_get_base();

	/* ------ YOUR CODE ENDS HERE ------*/
	u64 tail=rb->data_tail;
	u64 head=rb->data_head;
	u64 size=rb->data_size;
	void* start=rb->data_base;
	u64 lost=0,combine_size=0;
	if(head<tail){
		while(tail<size){
			struct perf_event_header *hdr=(struct perf_event_header*)(start+tail);
			if(hdr->type==PERF_RECORD_SAMPLE){
				struct sample_event* se=(struct sample_event*)(hdr);
				printf("0x%lx\n",se->addr);
			}
			else if(hdr->type==PERF_RECORD_LOST){
				struct lost_event* le=(struct lost_event*)(hdr);
			}
			else combine_size+=hdr->size;
			tail+=hdr->size;
		}
		tail=tail&(size-1);
	}
	while(tail<head){
		u64 mod=tail&(size-1);
		struct perf_event_header *hdr=(struct perf_event_header*)(start+mod);
		if(hdr->type==PERF_RECORD_SAMPLE){
			struct sample_event* se=(struct sample_event*)(hdr);
			printf("0x%lx\n",se->addr);
		}
		else if(hdr->type==PERF_RECORD_LOST){
			struct lost_event* le=(struct lost_event*)(hdr);
			lost+=le->lost;
		}
		else combine_size+=hdr->size;
		tail+=hdr->size;
	}
			
	/* print formats */
	// printf("0x%...."); // to print sample addresses
	 printf("number of lost records: %ld\n",lost);
	 printf("unknown size: %ld\n",combine_size);

	/* ------ YOUR CODE ENDS HERE ------*/
	buffer_exit();
	return 0;
}

