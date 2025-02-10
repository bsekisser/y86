#include "y86.h"

/* **** */

static
void push(y86_ref vm)
{
	rGPR(SP) -= sizeof(y86_reg_t);
	vm->memory[rGPR(SP)] = data;
}

static
y86_reg_t pop(y86_ref vm)
{
	y86_reg_t data = vm->memory[rGPR(SP)];
	rGPR(SP) += sizeof(y86_reg_t);
}

/* **** */

void y86_init(y86_ref vm)
{
	vm->flags.cold = 1;
}

void y86_reset(y86_ref vm)
{
	PC = 0;
}

void y86_step(y86_ref vm)
{
	CYCLE++;
	vm->val.p = PC;

	if(ifetch(vm, &vm->ir.raw, &vm->val.p, 1))
		return(-1);

	ICOUNT++;

	switch(IR >> 4) {
		case _halt:
			LOG("rax: 0x%08x", REG(rax));
			return(-1);
		case _irmov:
		case _mrmov:
		case _op:
		case _rmmov:
		case _rrmov:
			if(ifetch(vm, rR(AB), &PC, 1))
				return(-1);
			rR(A) = (rR(AB) >> 4) & 15;
			rR(B) = rR(AB) & 15;
		case _nop:
			return(0);
			break;
	}

	switch(IR >> 4) {
		case _rrmov:
			if(!check_cc(vm, IR & 15))
				return(0);
			__attribute__((fallthrough));
		case _irmov:
		case _rmmov:
			vR(B) = rGPR(A)
			break;
	}

	switch(IR >> 4) {
		case _irmov:
			if(ifetch(vm, &vR(I), &PC, 1))
				return(-1);
			break;
		case _mrmov:
		case _rmmov:
			if(ifetch(vm, &vR(D), &PC, 4))
				return(-1);
			break;
		case _op:
			switch(IR & 15) {
				case _asr: case _lsl: case _lsr: case _ror:
					vR(A) &= 0xff;
					break;
			}
			break;
	}

	switch(IR >> 4) {
		case _op:
			switch(IR & 15) {
				case _add:
					vR(B) += vR(A);
					break;
				case _and:
					vR(B) &= vR(A);
					break;
				case _asr:
					vR(B) = ((signed)vR(B)) >> vR(A);
					break;
				case _bic:
					vR(B) &= ~vR(A);
					break;
				case _lsl:
					vR(B) <<= vR(A);
					break;
				case _lsr:
					vR(B) >>= vR(A);
					break;
				case _mov:
					vR(B) = vR(A);
					break;
				case _mvn:
					vR(B) = ~vR(A);
					break;
				case _orr:
					vR(B) |= vR(A);
					break;
				case _ror:
					vR(B) = (vR(B) >> vR(A)) | (vR(B) << ~vR(A));
				case _rsb:
					vR(B) = vR(A) - vR(B);
					break;
				case _sub:
					vR(B) -= vR(A);
					break;
				case _xor:
					vR(B) ^= vR(A);
					break;
			}
			break;
		case _irmov:
		case _rrmov:
			vR(B) = vR(A);
			break;
		case _mrmov:
			vR(B) = vm->memory[vR(EA)];
			break;
		case _rmmov:
			vm->memory[vR(EA)] = vR(B);
			break;
	}


	switch(IR >> 4) {
		case _irmov:
		case _rrmov:
		case _mrmov:
			rGPR(B) = vR(B);
			break;
	}
}
