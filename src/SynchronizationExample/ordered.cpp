#include<omp.h>
#include<iostream>
#include<string.h>
#include<stdio.h>
using namespace std;

 unsigned int seed;
 #define n_points 100
 long sum = 0;
 int i;

 int a[100] ={0,};
 int b[100] = {0,};

void work1()
{
    #pragma omp for ordered 
    for (i = 1; i < n_points; i++) { 
        #pragma omp ordered
        b[i] = b[i-1] + a[i] ;
    }

    
}

void sub()
{
    work1();
    #pragma omp barrier
    #pragma omp single
    { 
        printf("work res1 -  %d\n", b[99] );
    }  
}

int main()
{
    for(int i=0;i<100;i++)
    {
        a[i] = 2;
        b[i ] = 0;
    }
    seed =time(nullptr);
    #pragma omp parallel num_threads(10)  shared(a, b)
    {
        sub();
    } 
    
}