#include "y86.h"

/* **** */

#include <assert.h>
#include <stddef.h>
#include <stdlib.h>

/* **** */

y86_p y86_alloc(y86_href h2vm, void *const mem_data, const size_t mem_alloc)
{
	y86_ref vm = calloc(1, sizeof(y86_t));
	if(!vm) return(0);

	vm->mem.alloc = mem_alloc;
	vm->mem.data = mem_data ?: calloc(1, mem_alloc);
	vm->mem.flags.free = !!(vm->mem.data && !mem_data);

	if(!vm->mem.data)
		goto _fail;

	if(h2vm)
		*h2vm = vm;

	return(vm);

_fail:
	y86_exit(vm);
	return(0);
}

int y86_exit(y86_ref vm)
{
	if(!vm) return(-1);

	if(vm->mem.flags.free)
		free(vm->mem.data);

	free(vm);

	return(0);
}

int y86_reset(y86_ref vm)
{
	if(!vm) return(-1);

	for(unsigned x = 0; x < _y86_reg_count; x++)
		REGx(x) = 0;

	SP = vm->mem.alloc & ~(sizeof(y86_reg_t) - 1);
	PC = 0;

	vm->state = state_aok;

	return(0);
}

y86_state_t y86_state(y86_ref vm)
{
	if(!vm) return(0);

	return(vm->state);
}
