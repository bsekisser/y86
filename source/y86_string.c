#include "y86_string.h"
#include "y86_core.h"

/* **** */

const char* y86_reg_lcase[_y86_reg_count] = {
	"rax", "rcx", "rdx", "rbx", "rsp", "rbp", "rsi", "rdi",
	"r8", "r9", "r10", "r11", "r12", "r13", "r14",
	"r15"
};

const char* y86_state_lcase[_y86_state_count] = {
	[state_adr] = "ADR",
	[state_aok] = "AOK",
	[state_halt] = "HALT",
	[state_ins] = "INS",
};
