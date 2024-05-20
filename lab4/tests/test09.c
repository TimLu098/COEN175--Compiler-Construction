int f(void) { return 0; }
int f(void) { return 0; }	/* redefinition of 'f' */

int *g(void) { return 0; }
int *g(void);
int *g(void) { return 0; }	/* redefinition of 'g' */

int f(void) { return 0; }	/* redefinition of 'f' */
