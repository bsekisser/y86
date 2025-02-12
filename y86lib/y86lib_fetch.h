#pragma once

/* **** */

#include "y86lib_exception.h"
#include "y86lib_core.h"

/* **** */

#include <endian.h>
#include <inttypes.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

/* **** */

static
int _fetch(void *const dst, const void *const src, size_t size)
{
	switch(size) {
		case 1:
			*(uint8_t*)dst = *(uint8_t*)src;
			break;
		case 2:
			*(uint16_t*)dst = le16toh(*(uint16_t*)src);
			break;
		case 3:
			*(uint32_t*)dst = le32toh(*(uint32_t*)src) & ((1 << 24) -1);
			break;
		case 4:
			*(uint32_t*)dst = le32toh(*(uint32_t*)src);
			break;
		case 8:
			*(uint64_t*)dst = le64toh(*(uint64_t*)src);
			break;
		default:
			return(-1);
	}

	return(0);
}

static
int fetch(y86_ref vm, void* x, y86_reg_ref valP, size_t size)
{
	if(*valP >= (vm->mem.alloc - size))
		return(y86_exception_fetch_abort(vm));

	void* p = vm->mem.data + *valP;
	*valP += size;

	CYCLE += size;

	if(0 > _fetch(x, p, size))
		return(y86_exception_data_abort(vm));

	return(0);
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
		case _jcc:
			if(vm->flags.feature.pc_relative) {
				if(fetch(vm, &vm->val.c, valP, vm->flags.feature.pc_relative))
					return(-1);
			} else if(fetch(vm, &vm->val.c, valP, sizeof(y86_reg_t)))
				return(-1);
			break;
		case _mrmov:
		case _rmmov:
			if(vm->flags.feature.displacement_size) {
				if(fetch(vm, &vm->val.c, valP, vm->flags.feature.displacement_size))
					return(-1);
				break;
			}
		__attribute__((fallthrough));
		case _irmov:
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
