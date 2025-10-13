#ifndef __TB_H_
#define __TB_H_

#include <types.h>
#include <context.h>

///////////////////////////////////////////////////////////////////////
///////////////////// Trace buffer functionality /////////////////////
/////////////////////////////////////////////////////////////////////
#define TRACE_BUFFER_MAX_SIZE 4096

//Trace buffer information structure
struct tb_info
{
	//Modify as per the need
	u32 R;
	u32 W;
	char* arr;
	u32 flag;
};


extern int sys_create_tb(struct exec_context *current, int mode);
#endif
