	NOTICE: Avoid whitespaces when naming the root directory. Working on a proper fix.
	NOTICE: Scripts use getopt which is not POSIX-compliant.
	TODO: Read ITCS till malloc
	TODO: Extract file from archive
	TODO: The Standard Library Maki libmaki.so
	TODO: The Extended Library Koji libkoji.so
	TODO: Central CLI
	TODO: VDF Parser
	TODO/DONE: Install P90, P90s, Vector SD, Bizon SD, HK443 SD, Raal MG, Lux's Survivor Legs
	TODO: Change force dir to default dir
	NOTICE:
	Multi-vpk addons support were added as a response to merging, a legacy practice that is not as flexible.
	When to group vpks into one folder: when it's the same addon split into parts (campaigns, modular huds, ...) Also, they are often made by the same author.
	When NOT to group vpks into one folder:
	When they are broadly, only thematically, related (eg all l4d2 survivor models, all default gun models) -> use separators
	When vpks complement each other and has no independent use, but can still be swapped around if needed (eg mrv voice goes with spectre coach, unless there are other robotic survivor models). There's also the matter of the display of credits.
	There will be no system to validate whether you've followed these rules. Use responsibly for your own sake!
	NOTICE:
	When to merge vpks into one: when you absolutely cant bear the ingame addon browser displaying multiple small addons. Although it is possible to merge, it is not recommended. This tool already serves to replace the addon browser, so you should keep the addon browser intact. Besides, modular vpk files are easier to manage.
	ADDENDUM:
	This will cause an error:
	// fileA.c
	void foo () { puts ("Hi"); }
	void fee () { void (*p) () = foo; }
	// fileA.h
	extern void foo ();
	extern void fee ();
	Fix is to turn foo () to static void. Or use PIC.
	// fileA.c
	static void foo () { puts ("Hi"); }
	void fee () { void (*p) () = foo; }
	// fileA.h
	extern void fee ();
	The error message went something like "relocation against `restxt_handler' in readonly section `.text'"
	ADDENDUM: Forgot to initialize the cURL handler of dlqueue.
	ADDENDUM: Relocation error strikes again! This time it seems to have to do something with stderr. Again, can't avoid these until I've learned how things work. For now, use PIC.
	TODO/DONE: mkstrcat is broken
	ADDENDUM: I edited strcat when I was sending a sample of it to Bocchi.
	ADDENDUM: Statements start getting executed in main(). Outside of main() there can only be declarations or definitions, not executions.
	TODO: Testing raw HTTP client and server model!