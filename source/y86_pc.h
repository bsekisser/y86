#pragma once

/* **** */

#include "y86.h"

/* **** */

static
void y86_pc(y86_ref vm)
{
	switch(vm->ir.code) {
		case _aluop:
		case _irmov:
		case _mrmov:
		case _nop:
		case _pop:
		case _push:
		case _rmmov:
		case _rrmov:
			PC = vm->val.p;
			break;
		case _call:
			PC = vm->val.c;
			break;
		case _halt:
			PC = 0;
			break;
		case _jcc:
			PC = vm->flags.cond ? vm->val.c : vm->val.p;
			break;
		case _ret:
			PC = vm->val.m;
			break;
	}

	ICOUNT++;
}
