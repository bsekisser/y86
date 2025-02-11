#include "y86.h"

/* **** */

#include "y86_fetch.h"
#include "y86_decode.h"
#include "y86_execute.h"
#include "y86_memory.h"
#include "y86_pc.h"
#include "y86_writeback.h"

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
