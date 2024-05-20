

double mkdouble(int n, int d);

int printint(int n);
int printdouble(double d);

int main(void)
{
    int a;
    int b;
    int e;
    double c, d;

    /*e = c == d;*/

    c = mkdouble(3, 1);   
    d = mkdouble(2, 1);
    a = 2;
    b = 3;
    e = a < c;

     /*c = d / c;*/
    printint(e);
    /*printdouble(c);*/

}

