int f(void), *g(void), **h(void);
char *a(void);

int f(void) { return 0; }
int *g(void) { return 0; }

int *g(void), f(void);

int h(void) { return 0; }		/* conflicting types for 'h' */

char *a(void) { return 0; }
