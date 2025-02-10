#include "y86.h"

/* **** */

#include "y86_fetch.h"
#include "y86_decode.h"
#include "y86_execute.h"
#include "y86_memory.h"
#include "y86_pc.h"
#include "y86_writeback.h"

#include "y86_string.h"

/* **** */

#include <inttypes.h>
#include <stdint.h>

/* **** */

static
int y86_step(y86_ref vm)
{
	if(y86_fetch(vm))
		return(-1);
	y86_decode(vm);
	if(y86_execute(vm))
		return(-1);
	if(y86_memory(vm))
		return(-1);
	y86_writeback(vm);
	y86_pc(vm);

	return(0);
}

int main(int argc, char** argv)
{
	y86_t yt, *vm = &yt;

	yt.mem.alloc = 1 * 1024 * 1024;
	yt.mem.data = malloc(yt.mem.alloc);

	assert(2 == argc);

	FILE* fp = 0;
	if(0 > (fp = fopen(argv[1], "r")))
		goto cleanup;

	if(0 > fread(yt.mem.data, yt.mem.alloc, 1, fp))
		goto cleanup;

	fclose(fp); fp = 0;

	for(unsigned x = 0; x < _y86_reg_count; x++)
		REGx(x) = 0;

	SP = 0x00010000;
	PC = 0;

	yt.state = state_aok;

	for(;state_aok == yt.state;)
		if(y86_step(vm))
			break;

	char flags[16] = "--------", *dst = flags;

	*dst++ = vm->flags.cf ? 'C' : 'c';
	*dst++ = vm->flags.sf ? 'N' : 'p';
	*dst++ = vm->flags.vf ? 'V' : 'v';
	*dst++ = vm->flags.zf ? 'Z' : 'z';
	*dst = 0;

	printf("cycles: 0x%016" PRIx64, CYCLE);
	printf(", icount: 0x%016" PRIx64, ICOUNT);
	printf(", pc: 0x%016" PRIx64, (uint64_t)PC);
	printf(", %s", flags);
	printf(", state(0x%08x): %s\n\n", vm->state, y86_state_lcase[vm->state]);

	for(unsigned x = 0; x < _y86_reg_count;) {
		for(unsigned y = 0; y < 4; y++, x++) {
			const uint64_t v = REGx(x);
			printf("%s: 0x%016" PRIx64 ", ", y86_reg_lcase[x], v);
		}

		putchar('\n');
	}

cleanup:
	if(yt.mem.data)
		free(yt.mem.data);

	if(fp)
		fclose(fp);
}
