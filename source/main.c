#include "y86.h"
#include "y86_string.h"

/* **** */

#include <assert.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>

/* **** */

#define KB(_x) ((_x) * 1024)
#define MB(_x) KB(KB(_x))

int main(int argc, char** argv)
{
	assert(2 == argc);

	y86_ref vm = y86_alloc(0, 0, MB(1));

	FILE* fp = 0;
	if(0 > (fp = fopen(argv[1], "r")))
		goto cleanup;

	if(0 > fread(vm->mem.data, vm->mem.alloc, 1, fp))
		goto cleanup;

	fclose(fp); fp = 0;

	y86_reset(vm);

	for(;state_aok == y86_state(vm);)
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
	y86_exit(vm);

	if(fp)
		fclose(fp);
}
