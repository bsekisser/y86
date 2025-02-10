#pragma once

/* **** */

#include "y86.h"

/* **** */

static
void y86_decode(y86_ref vm)
{
	switch(vm->ir.code) {
		case _aluop:
		case _push:
		case _rmmov:
		case _rrmov:
			vm->val.a = &REGx(vm->ir.r.a);
			break;
		case _pop:
		case _ret:
			vm->val.a = &SP;
			break;
	}

	switch(vm->ir.code) {
		case _aluop:
		case _mrmov:
		case _rmmov:
		case _rrmov:
			vm->val.b = &REGx(vm->ir.r.b);
			break;
		case _call:
		case _pop:
		case _push:
		case _ret:
			vm->val.b = &SP;
			break;
	}
}
