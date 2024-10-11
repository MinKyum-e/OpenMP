#include<omp.h>
#include<stdio.h>
#include<iostream>

using namespace std;

int taskA()
{
    int cnt =0 ;
    for(int i=0;i<1000000000;i++)
    {
        cnt++;
    }
    return cnt;
}
int taskB()
{

    int cnt =0 ;
    for(int i=1;i<10000;i++)
    {
        cnt++;
    }
    return cnt;
}
int taskC()
{

    int cnt =0 ;
    for(int i=1;i<=10;i++)
    {
        cnt++;
    }
    return cnt;
}

int main()
{
 
    #pragma omp parallel sections
    {

            #pragma omp section
            {
                int res1 = taskA();
                printf("A res : %d\n", res1);
            }
        
            #pragma omp section
            {
               int res2 = taskB();
                printf("B res : %d\n", res2);
            }
            #pragma omp section
            {
                int res3 = taskC();
                printf("C res : %d\n", res3);
            }
    }
}