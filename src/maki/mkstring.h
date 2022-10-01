#ifndef _H_MAPP_STR
#define _H_MAPP_STR
// According to stack overflow, extern is already implicit if function
// has no body. Need to confirm this.
extern char*	mk_strcat(char* dest, const char* src);
extern int		mk_strlen(const char* str);
extern char*	mk_getstdin();
#endif