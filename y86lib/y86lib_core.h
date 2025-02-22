#pragma once

/* **** */

#include "y86lib.h"

/* **** */

#include <stddef.h>
#include <stdint.h>

/* **** */

typedef uint32_t y86_reg_t;
typedef y86_reg_t* y86_reg_p;
typedef y86_reg_p const y86_reg_ref;

/* **** */

typedef enum y86_rreg_t y86_rreg_t;
enum y86_rreg_t {
	_y86_reg_rax, _y86_reg_rcx, _y86_reg_rdx, _y86_reg_rbx,
	_y86_reg_rsp, _y86_reg_rbp, _y86_reg_rsi, _y86_reg_rdi,
	_y86_reg_r8, _y86_reg_r9, _y86_reg_r10, _y86_reg_r11,
	_y86_reg_r12, _y86_reg_r13, _y86_reg_r14,
	_y86_reg_r15,
//
	_y86_reg_count
};

typedef enum y86_inst_t y86_inst_t;
enum y86_inst_t {
	_halt,
	_nop,
	_rrmov,
	_irmov,
	_rmmov,
	_mrmov,
	_aluop,
	_jcc,
	_call,
	_ret,
	_push,
	_pop,
};

typedef enum y86_ccx_t y86_ccx_t;
enum y86_ccx_t {
	cc_al, cc_le, cc_l, cc_e, cc_ne, cc_ge, cc_g
};

typedef enum y86_aluop_t y86_aluop_t;
enum y86_aluop_t {
	_add, _sub, _and, _xor,
	_asr, _lsl, _lsr, _ror,
	_bic, _mov, _mvn, _orr,
	_rsb
};

typedef enum y86_sspr32_t y86_sspr32_t;
enum y86_sspr32_t {
	y86_spr32_ip,
	y86_spr32_ir,
//	y86_spr32_pc,
//
	_y86_spr32_count
};

typedef enum y86_sspr64_t y86_sspr64_t;
enum y86_sspr64_t {
	y86_spr64_cycle,
	y86_spr64_icount,
//	y86_spr64_pc,
//
	_y86_spr64_count
};

typedef struct y86_tag {
	y86_reg_t r[_y86_reg_count];
#define REGx(_x) vm->r[_x]
#define rREG(_x) REGx(_y86_reg_##_x)

	uint64_t spr64[_y86_spr64_count];
#define SPR64x(_x) vm->spr64[_x]
#define rSPR64(_x) SPR64x(y86_spr64_ ## _x)

	uint32_t spr32[_y86_spr32_count];
#define SPR32x(_x) vm->spr32[_x]
#define rSPR32(_x) SPR32x(y86_spr32_ ## _x)

	struct {
		size_t alloc;
		void* data;
		struct {
			uint8_t free:1;
		}flags;
	}mem;

	struct {
		uint8_t raw;
		y86_inst_t code;
		union {
			uint8_t raw;
			y86_aluop_t alu;
			y86_ccx_t cc;
		}op;
		struct {
			uint8_t raw;
			y86_rreg_t a, b;
		}r;
	}ir;

	struct {
		y86_reg_p a, b;
		y86_reg_t c, e, m, p;
	}val;

	struct {
		struct {
			uint8_t cf:1;
			uint8_t sf:1;
			uint8_t vf:1;
			uint8_t zf:1;
		}cc;
		struct {
			struct {
				uint8_t data:1;
				uint8_t fetch:1;
			}abort;
			uint8_t illegal_instruction:1;
		}exception;
		struct {
			uint8_t displacement_size:4;
			uint8_t pc_relative:4;
		}feature;
		uint8_t cold:1;
		uint8_t cond:1;
		uint8_t run:1;
		uint8_t step:1;
	}flags;

	y86_state_t state;
}y86_t;

#define CF vm->flags.cc.cf
#define SF vm->flags.cc.sf
#define VF vm->flags.cc.vf
#define ZF vm->flags.cc.zf

#define CYCLE rSPR64(cycle)
#define ICOUNT rSPR64(icount)
#define IR rSPR32(ir)
#define IP rSPR32(ip)
#define PC rREG(r15)
#define SP rREG(rsp)
