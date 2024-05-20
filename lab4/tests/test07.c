char **a(int x), *b(int y);

int f(int z), *g(int w);
int g(int w), f(int z);		/* conflicting types for 'g' */

char *a(int x);			/* conflicting types for 'a' */

int f(int z);

char *b(int y), f(int z);	/* conflicting types for 'f' */
