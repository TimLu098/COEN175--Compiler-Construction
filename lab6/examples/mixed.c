/* math.c */

int printf(char *s, ...);

int main(void)
{
    int a, b, c, x, y;
    double d, e, z;

    x = 100;
    y = 30;
    z = 200; /*2*/
    
    a = x + y + z;
    b = x - y - z;
    c = x * y * z;
    d = x / y + z;
    e = x % y - z;

    printf("%d\n", a);
    printf("%d\n", b);
    printf("%d\n", c);
    printf("%f\n", d);
    printf("%f\n", e);

    a = x > y;
    /*double z;   int y;*/
    b = z == y;               /*passed*/
    /*b = z < y;*/                /*issue*/
    b = x < y == z < y;   /*issue*/
    c = x >= y != y >= x;     /*passed*/
    /*d = x + y > z;      */  /*issue*/
    /*e = x - y < y * z; */   /*issue*/

    printf("%d\n", a);
    printf("%d\n", b);
    printf("%d\n", c);
    printf("%f\n", d);
    printf("%f\n", e);
}
