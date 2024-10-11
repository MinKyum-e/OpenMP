#include<omp.h>
#include<iostream>
using namespace std;

 unsigned int seed;
 #define n_points 100000000
 long sum = 0;
 double x, y;
 int i;

void work1()
{
    #pragma omp for
    for (i = 0; i < n_points; i++) { 
        #pragma omp atomic  
        sum ++ ;
    }

    
}

void sub()
{
    work1();
    #pragma omp barrier
    #pragma omp single
    { 
        printf("work res1 -  %ld\n", sum );
    }  
}

int main()
{
    seed =time(nullptr);
    #pragma omp parallel num_threads(10)  
    {
        sub();
    } 
    
}