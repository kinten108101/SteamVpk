char*
mk_strcat (char* dest,
          const char* src)
{
	while (*dest) dest++; /// if dest's value is \0 (null terminator) then escapes, otherwise moves ptr
	// while ( *(dest++) = *(src++) );
	/* The equivalent is
	char tmp;
	do
	{
		tmp=*(src); *(dest)=tmp;
		dest++; src++;
	} while (tmp != '\0')
	*/
	while (*src)
	{
		*dest = *src;
		++dest; ++src;
	}
	return --dest;
	//synonym: dest--; return dest;
	//NOT synonym: return dest--; // return first before decrement
}

uint16
maki_strlen	(const char* str)
{
	unsigned short int i = 0;
	while (*str) { str++; i++; }
	return i;
}