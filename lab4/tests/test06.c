int x, *y;
int x, y;		/* conflicting types for 'y' */

int a[10], *p;
int *p, a[5];		/* conflicting types for 'a' */

int *b[10], **q;
int **b[10], **q;	/* conflicting types for 'b' */

int **q, **p;		/* conflicting types for 'p' */
