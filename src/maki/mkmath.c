int 
mk_pow (int base, int exponent)
{
	int ret = base;
	for (int i = 1; i<exponent; i++) ret*=base;
	if (exponent==0) ret=1;
	return ret;
}