#include "y86lib_exception.h"
#include "y86lib_core.h"

/* **** */

int y86_exception_data_abort(y86_ref vm)
{
	vm->state = state_adr;
	vm->flags.exception.abort.data = 1;

	return(-1);
}

int y86_exception_halt(y86_ref vm)
{
	vm->state = state_halt;

	return(-1);
}

int y86_exception_illegal_instruction(y86_ref vm)
{
	vm->state = state_ins;
	vm->flags.exception.illegal_instruction = 1;

	return(-1);
}

int y86_exception_fetch_abort(y86_ref vm)
{
	vm->state = state_adr;
	vm->flags.exception.abort.fetch = 1;

	return(-1);
}
