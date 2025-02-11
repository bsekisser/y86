#pragma once

/* **** */

#include "y86_exception.h"
#include "y86.h"

/* **** */

#include <endian.h>
#include <inttypes.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

/* **** */

static
int fetch(y86_ref vm, void* x, y86_reg_ref valP, size_t size)
{
	if(*valP >= (vm->mem.alloc - size))
		goto fetch_abort;

	void* p = vm->mem.data + *valP;
	*valP += size;

	CYCLE += size;

	uint64_t data = 0;

	switch(size) {
		case 1:
			data = (*(uint8_t*)x = *(uint8_t*)p);
			break;
		case 2:
			data = (*(uint16_t*)x = le16toh(*(uint16_t*)p));
			break;
		case 4:
			data = (*(uint32_t*)x = le16toh(*(uint32_t*)p));
			break;
		case 8:
			data = (*(uint64_t*)x = le16toh(*(uint64_t*)p));
			break;
		default:
			goto fetch_abort;
	}

	if(0) {
		printf("fetch: %01zu:[0x%016" PRIx64 "]", size, (uint64_t)*valP);
		printf("-->0x%016" PRIx64 "\n", data);
	}

	return(0);

fetch_abort:
	return(y86_exception_fetch_abort(vm));
}

static
int y86_fetch(y86_ref vm)
{
	uint8_t *const ir = &vm->ir.raw;
	y86_reg_ref valP = &vm->val.p;
	uint8_t *const rAB = &vm->ir.r.raw;

	*valP = PC;

	if(fetch(vm, ir, valP, 1))
		return(-1);

	vm->ir.code = (*ir >> 4) & 15;
	vm->ir.op.raw = *ir & 15;

	switch(vm->ir.code) {
		case _aluop:
		case _irmov:
		case _mrmov:
		case _pop:
		case _push:
		case _rmmov:
		case _rrmov:
			if(fetch(vm, rAB, valP, 1))
				return(-1);
			vm->ir.r.a = (*rAB >> 4) & 15;
			vm->ir.r.b = *rAB & 15;
			break;
		case _call:
		case _halt:
		case _jcc:
		case _nop:
		case _ret:
			break;
		default:
			return(y86_exception_illegal_instruction(vm));
	}

	switch(vm->ir.code) {
		case _call:
		case _irmov:
		case _jcc:
		case _mrmov:
		case _rmmov:
			if(fetch(vm, &vm->val.c, valP, sizeof(y86_reg_t)))
				return(-1);
			break;
		case _rrmov:
			if(vm->ir.op.alu & ~3)
				return(y86_exception_illegal_instruction(vm));
//
		case _aluop:
		case _halt:
		case _nop:
		case _pop:
		case _push:
		case _ret:
			break;
	}

	return(0);
}
