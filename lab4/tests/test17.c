int a(int x, ...), b(int *p, int **q);
int f(int x, int y, ...);

int a(int x, int y), c(double *d);	/* conflicting types for 'a' */

int b(int *p, ...);			/* conflicting types for 'b' */

int c(double *d, int x);		/* conflicting types for 'c' */

int a(void);				/* conflicting types for 'a' */
int f(int a, int b, ...);
