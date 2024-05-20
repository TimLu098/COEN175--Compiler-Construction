int main(void)
{
    int x, y, z;

    {
	int *x, *y, *z;
    }

    {
	int x, y, z;

	{
	    int *x, *y, *z;
	}
    }
}

int f(void)
{
    int x, y, z;

    {
	int main;
    }

    {
	int main;
    }

    return 0;
}
