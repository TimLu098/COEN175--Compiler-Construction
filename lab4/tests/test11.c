int a, g(void);

int main(void)
{
    int y, z;

    a = 1;
    x = 1;		/* 'x' undeclared */
    f();		/* 'f' undeclared */
    g();

    {
	int a, b, c;

	y = 1;

	{
	    int i;
	    i = 1;
	}

	i = 1;		/* 'i' undeclared */
    }

    b = 1;		/* 'b' undeclared */
}

int f(void);
