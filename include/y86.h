#pragma once

/* **** */

typedef struct y86_tag** y86_h;
typedef y86_h const y86_href;

typedef struct y86_tag* y86_p;
typedef y86_p const y86_ref;

typedef enum y86_state_t y86_state_t;

/* **** */

enum y86_state_t {
	state_err,
	state_aok,
	state_halt,
	state_adr,
	state_ins,
//
	_y86_state_count,
};

/* **** */

#include <stddef.h>

/* **** */

y86_p y86_alloc(y86_href h2vm, void *const mem_data, size_t mem_alloc);
int y86_exit(y86_ref vm);
int y86_reset(y86_ref vm);
y86_state_t y86_state(y86_ref vm);
int y86_step(y86_ref vm);
