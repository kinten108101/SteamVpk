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

NOTICE: When to merge vpks into one: when you absolutely cant bear the ingame addon browser displaying multiple small addons. Although it is possible to merge, it is not recommended. This tool already serves to replace the addon browser, so you should keep the addon browser intact. Besides, modular vpk files are easier to manage.
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
The error message went something like "relocation against restxt_handler in readonly section .text"

ADDENDUM: Forgot to initialize the cURL handler of dlqueue.
ADDENDUM: Relocation error strikes again! This time it seems to have to do something with stderr. Again, can't avoid these until I've learned how things work. For now, use PIC.
TODO/DONE: mkstrcat is broken
ADDENDUM: I edited strcat when I was sending a sample of it to Bocchi.
ADDENDUM: Statements start getting executed in main(). Outside of main() there can only be declarations or definitions, not executions.
TODO: Testing raw HTTP client and server model!
TODO/CANCELLED: support for other add-on systems (like gma-bsed)
ADDENDUM: (very) low priority!
ADDENDUM: New make functions used today: wildcard (dir*), notdir (file*), foreach (var,list,cmd)
UNLOCKED 20/12: python; getopt, argparse as an OO alternative; python ignore spaces before '(' and '.'. 
UNLOCKED 21/12: inkscape cli, ubuntu mono (the closest to the font named "Monospace" in GBuilder), distraction-free mode, editorconfig
TODO/DONE: Stabilize some source files and the makefile itself
ADDENDUM: Variables CFLAGS and LIBS are now stored in a separate file. This is because the flags are added and updated by `make libs` (there we run pkg-config, curl-config, etc), but we always run `make libs` separately from `make sandbox` and we've only got runtime variables.
TODO: Add appropriate sandbox runners that are unit tests:
    TODO: First test: path in fopen, relative or canonical?
TODO: unit_queue_linkedlist.c *Queue implementation using singly linked list*
TODO: unit_queue_cyclic.c *Queue implementation using cyclic array*
TODO: unit_stack *Stack implementation using singly linked list*
TODO: unit_bst.c
TODO: unit_crc32.c *Cyclic Redundancy Check (CRC)*
TODO: unit_rope.c *Rope Data Structure*
TODO/DONE: unit_curl
TODO: unit_sort
TODO/DONE: unit_strcat
TODO: unit_workshop
UNLOCKED 2/1: sed p, $(eval ), ifdef, ifeq, := (simple expansion) vs = (recursive expansion)
TODO/DONE: GTK Builder!
ADDENDUM: I was having two problems with GtkBuilder. First, gtk_builder_get_object returns gobject, not gtk_widget, I tried to cast but the window wasn't working. Secondly, I was trying to put GtkApplication in UI definition but that didn't work as i was having continuous segfault. Solution to the first is that it's a gtk window problem. Last time I used gtk_window_new, and the parameter was the app pointer. This is because that function did TWO things: creating a window gobject AND call gtk_window_set_application!! Forgot to do the latter. As for the second problem, it seems so far that I can't use GtkBuilder before app activation.
TODO: Try out a Gdb frontend
TODO: load_addonlist_csv() !!
TODO/DONE: add a menu button
ADDENDUM: Still cant find a way to display shortcut labels. That will come later.
TODO: Shortcut labels
TODO: GMenu in UI descriptions!
TODO: Click callback
TODO: css!
ADDENDUM: What I did: abstract the compilation process to the point when it takes only a key stroke to build+run a source file. But haven't I once done this in the past?
MULTI:
1. RTFM (Archio Arc)
2. Debloatation, producible results (L4D2 arc)
3. Abstraction (GDSC arc)
4. (composition?)
LOG: Instead of focusing on GTK, I focused on DSA and backend stuff. Recall the VPK arc. Recall the CURL arc. Recall the TCP arc. Also, I only went to Getting Started when I should be focusing on experimentation: why do they use GtkWidget instead of GtkWindow? Why are these macros? It's GIO's dynamic typing system, but I never set out to clear those obstacle questions. Even taking note isn't enough.
LOG: The goal is to abstract things. We can't use Roll Up forever, we must use Webpack. We can't use that make script forever, we must make it more efficient, more organized, etc. Because *the faster you can do things, the better you can experiment*. To program close to metal is not to admire art, but to get the motivation to abstract. What kind of pathetic art is that?
TODO: reimplement GListStore
ADDENDUM: When we compile shared libraries, we need CFLAGS for the headers. When we compile executables, we need LIBS to know which shared library to refer to.