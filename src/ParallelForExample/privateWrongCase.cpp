#include<omp.h>
#include<stdio.h>


int main()
{
    int tmp=0, j = 1;
    #pragma omp parallel for
    for(j=1;j<=1000;j++)
    #pragma omp atomic
        tmp += j;        
    
    printf("%d\n", tmp);
}