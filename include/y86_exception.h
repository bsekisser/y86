#pragma once

/* **** */

#include "y86.h"

/* **** */

int y86_exception_data_abort(y86_ref vm);
int y86_exception_halt(y86_ref vm);
int y86_exception_illegal_instruction(y86_ref vm);
//int y86_exception_irq(y86_ref vm);
int y86_exception_fetch_abort(y86_ref vm);
//int y86_exception_reset(y86_ref vm);
