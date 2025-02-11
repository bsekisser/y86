#pragma once

/* **** */

#include "y86_exception.h"
#include "y86_core.h"

/* **** */

static
int y86_rwio(y86_ref vm, y86_reg_t pat, y86_reg_p read, y86_reg_p write)
{
	if(pat >= (vm->mem.alloc - sizeof(y86_reg_t)))
		return(y86_exception_data_abort(vm));

	y86_reg_p m = vm->mem.data + pat;

	switch(sizeof(y86_reg_t)) {
		case 8:
			if(read) *read = le64toh(*m);
			if(write) *m = htole64(*write);
			break;
		case 4:
			if(read) *read = le32toh(*m);
			if(write) *m = htole32(*write);
			break;
	}

	return(0);
}

static
int y86_memory(y86_ref vm)
{
	switch(vm->ir.code) {
		case _call:
			return(y86_rwio(vm, vm->val.e, 0, &vm->val.p));
		case _mrmov:
			return(y86_rwio(vm, vm->val.e, &vm->val.m, 0));
		case _pop:
		case _ret:
			return(y86_rwio(vm, *vm->val.a, &vm->val.m, 0));
		case _push:
		case _rmmov:
			return(y86_rwio(vm, vm->val.e, 0, vm->val.a));
//
		case _aluop:
		case _halt:
		case _irmov:
		case _jcc:
		case _nop:
		case _rrmov:
			break;
	}

	return(0);
}
