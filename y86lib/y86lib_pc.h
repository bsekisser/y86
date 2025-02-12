#pragma once

/* **** */

#include "y86lib_core.h"

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
			if(vm->flags.feature.pc_relative)
				PC += vm->val.c;
			else
				PC = vm->val.c;
			break;
		case _halt:
			PC = 0;
			break;
		case _jcc:
			if(vm->flags.cond) {
				if(vm->flags.feature.pc_relative)
					PC += vm->val.c;
				else
					PC = vm->val.c;
			} else
				PC = vm->val.p;
			break;
		case _ret:
			PC = vm->val.m;
			break;
	}

	ICOUNT++;
}
