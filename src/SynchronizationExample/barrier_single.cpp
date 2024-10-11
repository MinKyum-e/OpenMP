#include<omp.h>
#include<iostream>
using namespace std;

 unsigned int seed;
 #define n_points 1000
  #define n_points2 1000000000
 long sum = 0;
 double x, y;
 int i;

void work1()
{
    unsigned int thread_seed = time(nullptr)+ omp_get_thread_num(); //thread별로 seed를 지정해줘야함.
    #pragma omp for reduction(+: sum) private(x, y)
    for (i = 0; i < n_points; i++) {   
        // 시드 포인터를 rand_r에 전달
        x = (double)rand_r(&thread_seed) / RAND_MAX;
        y = (double)rand_r(&thread_seed) / RAND_MAX;
        if (x * x + y * y < 1.0f) {
            sum++;
        }
    }

    
}

void work2()
{
    unsigned int thread_seed = time(nullptr)+ omp_get_thread_num(); //thread별로 seed를 지정해줘야함.
    #pragma omp for reduction(+: sum) private(x, y)
    for (i = 0; i < n_points2; i++) {   
        // 시드 포인터를 rand_r에 전달
        x = (double)rand_r(&thread_seed) / RAND_MAX;
        y = (double)rand_r(&thread_seed) / RAND_MAX;
        if (x * x + y * y < 1.0f) {
            sum++;
        }
    }

    
}

void sub()
{
    work1();
    #pragma omp barrier
    #pragma omp single
    { 
        printf("work res1 -  %f\n",  (double)sum/n_points * 4);
        sum = 0;
    }  
    #pragma omp barrier

    work2();
    #pragma omp barrier
    #pragma omp single
    { 
        printf("work res2 -  %f\n",  (double)sum/n_points2 * 4);
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