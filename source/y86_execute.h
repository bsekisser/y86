#pragma once

/* **** */

#include "y86.h"

/* **** */

static
int y86_aluop(y86_ref vm)
{
	const y86_reg_t valA = *vm->val.a;
	const y86_reg_t valB = *vm->val.b;
	const y86_reg_t shift = valA & 0xff;

	y86_reg_t valE = valB;

	switch(vm->ir.op.alu) {
		case _add:
			valE += valA;
			break;
		case _and:
			valE &= valA;
			break;
		case _asr:
			valE = (signed)valB >> shift;
			break;
		case _bic:
			valE &= ~valA;
			break;
		case _lsl:
			valE <<= shift;
			break;
		case _lsr:
			valE >>= shift;
			break;
		case _mov:
			valE = valA;
			break;
		case _mvn:
			valE = ~valA;
			break;
		case _orr:
			valE |= valA;
			break;
		case _ror:
			valE = (valB >> shift) | (valB << ((~shift) & 0xff));
			break;
		case _rsb:
			valE = valA - valB;
			break;
		case _sub:
			valE -= valA;
			break;
		case _xor:
			valE ^= valA;
			break;
		default:
			vm->state = state_ins;
			vm->flags.illegal_instruction = 1;
			return(-1);

	}

	vm->val.e = valE;

	vm->flags.zf = (0 == valE);
	vm->flags.sf = (0 > ((signed)valE));
	return(0);
}

static
int y86_cond_cc(y86_ref vm, y86_ccx_t cc)
{
	switch(cc) {
		case cc_al:
			return(1);
		case cc_e:
			return(vm->flags.zf);
		case cc_g:
			return(!vm->flags.sf);
		case cc_ge:
			return(!vm->flags.sf || vm->flags.zf);
		case cc_l:
			return(vm->flags.sf);
		case cc_le:
			return(vm->flags.sf || vm->flags.zf);
		case cc_ne:
			return(!vm->flags.zf);
		default:
			vm->state = state_ins;
			vm->flags.illegal_instruction = 1;
			return(-1);
	}

	return(0);
}

static
int y86_execute(y86_ref vm)
{
	int cond = -1;

	switch(vm->ir.code) {
		case _aluop:
			if(0 > y86_aluop(vm))
				return(-1);
			break;
		case _call:
		case _push:
			vm->val.e = *vm->val.b - 8;
			break;
		case _halt:
			vm->state = state_halt;
			break;
		case _irmov:
			vm->val.e = vm->val.c;
			break;
		case _mrmov:
		case _rmmov:
			vm->val.e = *vm->val.b + vm->val.c;
			break;
		case _pop:
		case _ret:
			vm->val.e = *vm->val.b + 8;
			break;
		case _rrmov:
			vm->val.e = *vm->val.a;
		__attribute__((fallthrough));
		case _jcc:
			cond = y86_cond_cc(vm, vm->ir.op.cc);
			if(0 > cond)
				return(-1);
			else
				vm->flags.cond = cond;
			break;
	}

	return(0);
}
