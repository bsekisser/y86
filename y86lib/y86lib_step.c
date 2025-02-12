#include "y86lib_core.h"

/* **** */

#include "y86lib_fetch.h"
#include "y86lib_decode.h"
#include "y86lib_execute.h"
#include "y86lib_memory.h"
#include "y86lib_pc.h"
#include "y86lib_writeback.h"

/* **** */

uint32_t y86_run(y86_ref vm, uint32_t run_cycles)
{
	for(; run_cycles; run_cycles--) {
		if(0 > y86_step(vm))
			break;
	}

	return(run_cycles);
}

int y86_step(y86_ref vm)
{
	if(y86_fetch(vm))
		return(-1);
	y86_decode(vm);
	if(y86_execute(vm))
		return(-1);
	if(y86_memory(vm))
		return(-1);
	y86_writeback(vm);
	y86_pc(vm);

	return(0);
}
