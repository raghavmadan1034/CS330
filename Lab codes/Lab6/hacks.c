#include<hacks.h>
#include<sigexit.h>
#include<entry.h>
#include<lib.h>
#include<context.h>
#include<memory.h>

struct hack_config{
	long cur_hack_config;
	u64 usr_handler_addr;
};

//Used to store the current hack configuration
static struct hack_config hconfig = {-1, -1};


//system call handler to configure the hack sematics
//user space connection is already created, You need to
//store the hack semantics in the 'hconfig' structure (after validation)
//which will be used when division-by-zero occurs

long sys_config_hs(struct exec_context *ctx, long hack_mode, void *uhaddr)
{
	//printk("Inside sys_config_hs ||||| hack_mode %x || ush at %x\n", hack_mode, (unsigned long) uhaddr);
	/*if(hack_mode==DIV_ZERO_USH_EXIT){
	  hconfig.usr_handler_addr=(u64)uhaddr;
	  }*/
	if(hack_mode>=DIV_ZERO_MAX||hack_mode<0){
		hconfig.cur_hack_config=-1;
		return -EINVAL;	
	}


	if(hack_mode==DIV_ZERO_OPER_CHANGE||hack_mode==DIV_ZERO_SKIP||hack_mode==DIV_ZERO_SKIP_FUNC) {
		//printk("BLOCK 1\n");
		hconfig.cur_hack_config=hack_mode;
	} else if(hack_mode== DIV_ZERO_USH_EXIT){
		if((u64)uhaddr>=ctx->mms->start && (u64)uhaddr<=ctx->mms->end){//the user address should be present in the memory segment of address space  (i.e btw mms->start and mms->end)
			hconfig.cur_hack_config=hack_mode;
		//printk("BLOCK 2\n");
			hconfig.usr_handler_addr=(u64)uhaddr;
		}
		else{
			hconfig.cur_hack_config=-1;
			return -EINVAL;
		}
	}
	else{
		hconfig.cur_hack_config=-1;
		//printk("BLOCK 3\n");
		return -EINVAL;
	}
	return 0; 
}


/*This is the handler for division by zero
 * 'regs' is a structure defined in include/context.h which
 * is already filled with the user execution state (by the asm handler)
 * and will be restored back when the function returns 
 *
 */
int do_div_by_zero(struct user_regs *regs)
{	
	/* printk("%d\n",(regs->rbp));
	   printk("%d\n",(regs->rax));
	   printk("%d\n",(regs->rcx));
	// printk("%ld\n",(regs->rsp));
	printk("%d\n",(regs->rdi));
	printk("%d\n",(regs->eax));*/
	if(hconfig.cur_hack_config==DIV_ZERO_OPER_CHANGE){
		if(regs->rcx==0){	
			regs->rcx=10;
			regs->rax=0;
		}

	}
	else if(hconfig.cur_hack_config==DIV_ZERO_SKIP){

		/*if(regs->rcx==0){	
			regs->rcx=10;
			regs->rax=0;
		}*/
		regs->entry_rip +=3;         
	}
	else if(hconfig.cur_hack_config == DIV_ZERO_USH_EXIT){
		regs->rdi=regs->entry_rip;
		regs->entry_rip = hconfig.usr_handler_addr;
		//printk("DIV zero calling ush at %x\n", hconfig.usr_handler_addr);
	}

	else if(hconfig.cur_hack_config == DIV_ZERO_SKIP_FUNC){
		regs->rax = 1;
		regs->entry_rip=*((u64*)(regs->rbp)+1);
		regs->entry_rsp=(u64)((u64*)(regs->rbp)+2);
		regs->rbp=*((u64*)(regs->rbp));

	}
	else{
		printk("Error...exiting\n");
		do_exit(0);
	}

	//do_exit(0);
	return 0;   	
}
