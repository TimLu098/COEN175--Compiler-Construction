int a, b, c;
int **a, b, c;			/* conflicting types for 'a' */
int b[10], c, a;		/* conflicting types for 'b' */

int x, **y, *z[10];
int **z[10], x, **y;		/* conflicting types for 'z' */
int *y, x, *z[10];		/* conflicting types for 'y' */

int i, *j[3], k[7];
int *j[4], k[7], i;		/* conflicting types for 'j' */
int i, *k[7], *j[3];		/* conflicting types for 'k' */

int ***m, ***n[10];
int ***m, ****m;		/* conflicting types for 'm' */
int ***n(void);			/* conflicting types for 'n' */
int ***n[10], ***m(int x);	/* conflicting types for 'm' */
