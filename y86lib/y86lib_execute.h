#pragma once

/* **** */

#include "y86lib_exception.h"
#include "y86lib_core.h"

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
			return(y86_exception_illegal_instruction(vm));

	}

	vm->val.e = valE;

	// TODO: CF & VF
	SF = (0 > ((signed)valE));
	ZF = (0 == valE);

	return(0);
}

static
int y86_cond_cc(y86_ref vm, y86_ccx_t cc)
{
	switch(cc) {
		case cc_al:
			return(1);
		case cc_e:
			return(ZF);
		case cc_g:
			return(!SF);
		case cc_ge:
			return(!SF || ZF);
		case cc_l:
			return(SF);
		case cc_le:
			return(SF || ZF);
		case cc_ne:
			return(!ZF);
		default:
			return(y86_exception_illegal_instruction(vm));
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
			return(y86_exception_halt(vm));
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
//
		case _nop:
			break;
	}

	return(0);
}
