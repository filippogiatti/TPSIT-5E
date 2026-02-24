/******************************************************************************

Welcome to GDB Online.
GDB online is an online compiler and debugger tool for C, C++, Python, Java, PHP, Ruby, Perl,
C#, OCaml, VB, Swift, Pascal, Fortran, Haskell, Objective-C, Assembly, HTML, CSS, JS, SQLite, Prolog.
Code, Compile, Run and Debug online from anywhere in world.

*******************************************************************************/
#include <stdio.h>

int main()
{
    float k;
    for(k=0; k<100; k++ ){
        float d=((8*k)+1)/7;
        printf("Il numero è' %.2f\n", d);
        printf("k è': %.2f\n\n", k);
        float c=(9^7)%15;
        printf("Il c è' %f\n\n", c);
    }

    return 0;
}
