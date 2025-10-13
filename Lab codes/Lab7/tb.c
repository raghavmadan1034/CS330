#include<tb.h>
#include<lib.h>
#include<file.h>
#include<context.h>
#include<memory.h>

/*
 * *  Trace buffer implementation.
 *
 */


///////////////////////////////////////////////////////////////////////////
////           TODO:     Trace buffer functionality                   /////
///////////////////////////////////////////////////////////////////////////

int mine(int a, int b){
	if(a<=b)return a;
	else return b;
}
// Check whether passed buffer is valid memory location for read.
static int tb_validate(unsigned long buff, u32 count, int acflags) 
{
  struct exec_context* ctx=get_current_ctx();
  struct mm_segment *mms = ctx->mms;
  if(buff>=mms[MM_SEG_DATA].start && buff+count<=mms[MM_SEG_DATA].next_free-1){return 1;}
  if(buff>=mms[MM_SEG_STACK].start && buff+count<=mms[MM_SEG_STACK].end-1){return 1;}
  if(buff>=mms[MM_SEG_CODE].start && buff+count<=mms[MM_SEG_CODE].next_free-1){
	  if((acflags&2))return 0;
	  else{
		// printk("CODE");
		  return 1;
	  }
  }
  if(buff>=mms[MM_SEG_RODATA].start && buff+count<=mms[MM_SEG_RODATA].next_free-1){
	  
 		// printk("RODATA");
	  if((acflags&2))return 0;
	  else return 1;
  }
  /*if(buff>=ctx->vm_area->vm_start && buff+count<=ctx->vm_area->vm_end-1){
	  if((acflags&(ctx->vm_area->access_flags))==acflags)return 1;
  }
  int flag=0;*/
  while(ctx->vm_area!=NULL){
	 
  	if(buff>=ctx->vm_area->vm_start && buff+count<=ctx->vm_area->vm_end-1){
	 	 if((acflags&(ctx->vm_area->access_flags))==acflags)return 1;
  	}
	ctx->vm_area=ctx->vm_area->vm_next;
  }
//printk("Error\n");	
      return 0;
}

static long tb_close(struct file *filep)
{
       // printk("Error\n");
       os_page_free(USER_REG, filep->tb->arr);
       os_free(filep->tb, sizeof( struct tb_info));
       os_free(filep-> fops, sizeof(struct fileops));
	 os_free(filep,sizeof(struct file));
        return 0;
}

static int tb_read(struct file *filep, char *buff, u32 count)
{ 

    if(!tb_validate((unsigned long)buff,count,2))return -EBADMEM;
    int read =0;
    
    struct tb_info* tb = filep->tb;
    int flag=tb->flag;
    while(read<count && flag){
	    buff[read]= tb->arr[tb->R];
	    read++;
	    tb->R++;
	    tb->R%=4097;
	    flag--;
    }
   filep->tb->flag=flag;
   filep->tb->R=tb->R; 
   return read;
    //printk("Error\n");	
    //return -1;

}

static int tb_write(struct file *filep, char *buff, u32 count)
{
    //printk("Error\n");
   
    if(!tb_validate((unsigned long)buff,count,1))return -EBADMEM;
    u32 r=filep->tb->R;
    u32 w=filep->tb->W;
     /*if(w>=r){
  	 if(count>4096-(w-r))count=4096-(w-r);
	 }
     else{
	     if(count>(r-w))count=r-w;
     }*/
    if(count>4096-filep->tb->flag)count=4096-filep->tb->flag;

    /*int j=0;
    for(int i=w;i<mine(4096,w+count);i++){
	    filep->tb->arr[i]=buff[j];
	    j++;
    }
    filep->tb->W=mine(4096,w+count);
    if(j<count){
	for(int i=0;i<count-(4096-w+1);i++){
	    filep->tb->arr[i]=buff[j];
    	    j++;
	}
	filep->tb->W=count-(4096-w+1);
    }
    filep->tb->flag+=j;
    return j;*/
   int write=0;
    while(write<count){
	  filep->tb->arr[w]=buff[write];
	  w++;
	  write++;
	  filep->tb->flag++;
	  w%=4097;
    }
    filep->tb->W=w;  

    
    return count;
}

int sys_create_tb(struct exec_context *current, int mode)
{
    int ret_fd = -1;
    for(int i=0;i<MAX_OPEN_FILES;i++){
	    if((current->files)[i]==NULL){
		    ret_fd=i;
		    break;
	    }
    }
    if(ret_fd==-1)return -EINVAL;

    struct file* f=(struct file*)os_alloc(sizeof(struct file));
    f->type=TRACE_BUFFER;
    f->mode=mode;
    f->offp=0;
    f->ref_count=1;

    f->inode=NULL;
    f->pipe=NULL;
    f->tb=(struct tb_info*)os_alloc(sizeof(struct tb_info));
    f->tb->arr=(char*)os_page_alloc(USER_REG);
    f->tb->R=0;
    f->tb->W=0;
    f->tb->flag=0;
    f->fops=(struct fileops*)os_alloc(sizeof(struct fileops));
    f->fops->read=&tb_read;
    f->fops->write=&tb_write;
    f->fops->lseek=NULL;
    f->fops->close=&tb_close;
    (current->files)[ret_fd]=f;
   // printk("Error\n");	
    return ret_fd;
}
