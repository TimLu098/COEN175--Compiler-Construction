int main(void)
{
    char *x, **y, z;
    char *x;			/* redeclaration of 'x' */

    {
	int x, y, z;
	int *y, a, b;		/* redeclaration of 'y' */

	{
	    int x, y, y;	/* redeclaration of 'y' */
	}

	{
	    int z, a;
	    int a;		/* redeclaration of 'a' */
	}
    }
}

int f(void)
{
    int x, y;
    int y, z;			/* redeclaration of 'y' */
}
