#include "../smkl/type.h"

uint64_t
mk_pow (uint64_t base, uint64_t exponent)
{
	uint64_t ret = base;
	for (uint64_t i = 1; i<exponent; i++) ret*=base;
	if (exponent==0) ret=1;
	return ret;
}

uint64_t
mk_log (void)
{
	return 0;
}

uint64_t
mk_inverse_square (void)
{
	return 0;
}