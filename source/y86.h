#pragma once

/* **** */

#include <assert.h>
#include <inttypes.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/* **** */

typedef struct y86_tag* y86_p;
typedef y86_p const y86_ref;

typedef uint32_t y86_reg_t;
typedef y86_reg_t* y86_reg_p;
typedef y86_reg_p const y86_reg_ref;

/* **** */

typedef enum y86_rreg_t y86_rreg_t;
enum y86_rreg_t {
	_y86_rax, _y86_reg_rcx, _y86_reg_rdx, _y86_reg_rbx, _y86_reg_rsp, _y86_reg_rbp, _y86_reg_rsi, _y86_reg_rdi,
	_y86_r8, _y86_r9, _y86_r10, _y86_r11, _y86_r12, _y86_r13, _y86_r14,
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
	y86_spr32_pc,
//
	_y86_spr32_count
};

typedef enum y86_sspr64_t y86_sspr64_t;
enum y86_sspr64_t {
	y86_spr64_cycle,
	y86_spr64_icount,
	y86_spr64_pc,
//
	_y86_spr64_count
};

typedef enum y86_state_t y86_state_t;
enum y86_state_t {
	state_err,
	state_aok,
	state_halt,
	state_adr,
	state_ins,
//
	_y86_state_count,
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
		unsigned cf:1;
		unsigned cold:1;
		unsigned cond:1;
		unsigned data_abort:1;
		unsigned fetch_abort:1;
		unsigned illegal_instruction:1;
		unsigned irq:1;
		unsigned run:1;
		unsigned sf:1;
		unsigned step:1;
		unsigned vf:1;
		unsigned zf:1;
	}flags;

	y86_state_t state;
}y86_t;


#define CYCLE rSPR64(cycle)
#define ICOUNT rSPR64(icount)
#define IR rSPR32(ir)
#define IP rSPR32(ip)
#define PC rSPR32(pc)
#define SP rREG(rsp)
