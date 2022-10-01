#include "manager/addons_dict.h"

void
init_addonv (addonv* self)
{
	self->len = 0;
	self->push = addonv__push(addons_vector);
	self->pop = addonv__pop(addons_vector);
	return entries;
}

void
addonv__pop (addonv* self)
{
	--self->len;
}

void
addonv__push (addonv* self)
{
	++self->len;
}

void
addonv__test ()
{
	printf("Hi!\n");
}

void
load_order ()
{

}

void
load_info ()
{

}

void load_addons_iter ()
{

}

void startup ()
{
	// create dictionary
	addonv* addons_vector = (addonv*)malloc(sizeof(addonv));
	init_addonv(addons_vector);
	addons_vector->test();
}

void make_link ()
{
	
}

void import_addon (const char* vpk)
{
	// create an addon dir in addons
	// move vpk to addon dir
	// sync out some files
}

void merge_comfortable ()
{
	// merge headers to one, spread file data evenly.
}

void merge_compact ()
{
	// merge vpks into one. Only if their sizes allow it.
}