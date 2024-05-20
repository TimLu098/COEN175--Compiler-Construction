int main(void)
{
    main();

    x = 1;			/* 'x' undeclared */

    {
	int y;
	y = 1;
    }

    y = 1;			/* 'y' undeclared */
}

int x;
