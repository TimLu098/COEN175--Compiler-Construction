int f(int x, int *y, ...);
int g(int x, int *y, ...);

int f(int x, int *y, ...) {
    return 0;
}

int g(int x, int *y) { return 0; }	/* conflicting types for 'g' */

int h(int g, ...) { return 0; }

int f(int x, int *y, ...);
