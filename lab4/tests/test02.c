int f(int x, int y, int z, int y);	/* redeclaration of 'y' */

int g(int x, int x, int y, ...) {	/* redeclaration of 'x' */
}

int h(int a, int b, ...) {
    int a;				/* redeclaration of 'a' */
}
