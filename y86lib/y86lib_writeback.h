#pragma once

/* **** */

#include "y86lib_core.h"

/* **** */

static
void y86_writeback(y86_ref vm)
{
	switch(vm->ir.code) {
		case _aluop:
		case _irmov:
			REGx(vm->ir.r.b) = vm->val.e;
			break;
		case _call:
		case _push:
		case _ret:
			SP = vm->val.e;
			break;
		case _pop:
			SP = vm->val.e;
		__attribute__((fallthrough));
		case _mrmov:
			REGx(vm->ir.r.a) = vm->val.m;
			break;
		case _rrmov:
			if(vm->flags.cond)
				*vm->val.b = vm->val.e;
//				REGx(vm->ir.r.b) = vm->val.e;
			break;
//
		case _halt:
		case _jcc:
		case _nop:
		case _rmmov:
			break;
	}
}
