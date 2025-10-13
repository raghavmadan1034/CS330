#include<procinfo.h>
#include<lib.h>
#include<file.h>
#include<context.h>

static char* segment_names[MAX_MM_SEGS+1] = {"code", "rodata", "data", "stack", "invalid"}; 
static char* file_types[MAX_FILE_TYPE+1] = {"stdin", "stdout", "stderr", "reg", "pipe", "invalid"}; 

long get_process_info(struct exec_context *ctx, long cmd, char *ubuf, long len)
{
    long retval = -EINVAL;	
    /*
     * TODO your code goes in here
     * */
    if(cmd==GET_PINFO_GEN){	
    	//printk("user address %x\n", ubuf);    
    	if(ubuf==NULL)return retval;
    	if(sizeof(ctx->pid)+sizeof(ctx->ppid)+sizeof(ctx->name)+sizeof(ctx)>len)return retval;
    	struct general_info* gi =(struct general_info *)(ubuf);
    	gi->pid=ctx->pid;
    	gi->ppid=ctx->ppid;
    	gi->pcb_addr=(unsigned long)ctx;
    	strcpy(gi->pname,ctx->name);
   	 return 1;
    }
    else if(cmd==GET_PINFO_FILE){
   	if(ubuf==NULL)return retval;
	long no_of_files=0;
	for(int i=0;i<MAX_OPEN_FILES;i++){
		if((ctx->files)[i]!=NULL)no_of_files++;
	}
	long reqlen=sizeof(struct file_info)*no_of_files;
	if(reqlen>len)return retval;
	long j=0;
	for(int i=0;i<MAX_OPEN_FILES;i++){
		if((ctx->files)[i]==NULL)continue;
		long idx=j*sizeof(struct file_info);
		struct file_info * fi=(struct file_info*)(ubuf+idx);
		strcpy(fi->file_type,file_types[((ctx->files)[i])->type]);
		fi->mode=(ctx->files)[i]->mode;
		fi->ref_count=(ctx->files)[i]->ref_count;
		fi->filepos=(ctx->files)[i]->offp;
		j++;
	}
	return no_of_files;
		
	
    }
    else if(cmd==GET_PINFO_MSEG){
	    if(ubuf==NULL)return retval;
	    long noofseg=4;
	    long reqlen=sizeof(struct mem_segment_info)*noofseg;
	    if(reqlen>len)return retval;
	    for(int i=0;i<noofseg;i++){
		    struct mem_segment_info* f=(struct mem_segment_info*)(ubuf+i*sizeof(struct mem_segment_info));
	            strcpy(f->segname,segment_names[i]);
		    f->start=((ctx->mms[i]).start);
		    f->end=((ctx->mms[i]).end);
		    f->next_free=((ctx->mms[i]).next_free);
		    char sp[4];
		    long x=(ctx->mms[i]).access_flags;
		    if(x==0)strcpy(sp,"R__");
		    else if(x==1)strcpy(sp,"R__");
                    else if(x==2)strcpy(sp,"_W_");
                    else if(x==3)strcpy(sp,"RW_");
                    else if(x==4)strcpy(sp,"__X");
                    else if(x==5)strcpy(sp,"R_X");
                    else if(x==6)strcpy(sp,"_WX");
                    else if(x==7)strcpy(sp,"RWX");
                    else strcpy(sp,"ERR");
		    strcpy(f->perm,sp);
	    }
	    return noofseg;
    }

    else if(cmd==GET_PINFO_VMA){
	    if(ubuf==NULL)return retval;
	    int number_of_vms = 0;
	    struct vm_area * ctx_vm_area = (ctx->vm_area)->vm_next;
	    while(ctx_vm_area!=NULL){
		    number_of_vms++;
		    ctx_vm_area = ctx_vm_area -> vm_next;
	    }

	    int length = (number_of_vms)* sizeof(struct vm_area_info);
	    if(length > len) {return retval;}
	    ctx_vm_area = (ctx-> vm_area)-> vm_next;
	    int i = 0;

	    while(ctx_vm_area!= NULL){
		    struct vm_area_info * (vm_area_info_struct) = (struct vm_area_info*)(ubuf+i);
		    vm_area_info_struct->start = ctx_vm_area->vm_start;
		    vm_area_info_struct->end = ctx_vm_area->vm_end;

		    char sp[4];
		    long x=ctx_vm_area->access_flags;
		    if(x==0)strcpy(sp,"___");
		    else if(x==1)strcpy(sp,"R__");
		    else if(x==2)strcpy(sp,"_W_");
		    else if(x==3)strcpy(sp,"RW_");
		    else if(x==4)strcpy(sp,"__X");
		    else if(x==5)strcpy(sp,"R_X");
		    else if(x==6)strcpy(sp,"_WX");
		    else if(x==7)strcpy(sp,"RWX");
		    else strcpy(sp,"ERR");
		    strcpy(vm_area_info_struct->perm, sp);
		    ctx_vm_area = ctx_vm_area ->vm_next;
		    i+=sizeof(struct vm_area_info);
	    }
	    return number_of_vms;
    }

    return 1;
}
