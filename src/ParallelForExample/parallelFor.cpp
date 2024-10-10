#include<iostream>
#include<omp.h>
#include<stdlib.h>

#include <chrono>

using namespace std;
using namespace std::chrono;

int main()
{
    //sequential
    auto start = high_resolution_clock::now(); 
    long j=0;
    long result = 0;
    
    for(j=0;j<1000000000;j++)
    {
        result +=j;
    }

    auto stop = high_resolution_clock::now(); 
    auto duration = duration_cast<microseconds>(stop - start);
    cout << "Test sequential-    " << duration.count()<< " clock    " << "---result : "<< result <<"\n"; 


    //parallel - reduction을 사용해 각각 private로 값을 저장하다가 마지막에 global로 합침
    result = 0;
    start = high_resolution_clock::now(); 
    #pragma omp  parallel num_threads(40) 
    {
        #pragma omp for reduction(+: result)
        for(j=0;j<1000000000;j++)
        {
            result +=j;
        }
    }
    stop = high_resolution_clock::now(); 
    duration = duration_cast<microseconds>(stop - start);

    cout << "Test parallel-       "<< duration.count() << " clock    " << "---result : "<< result <<"\n"; 


    //parallel  마지막으로 종료된 쓰레드의 마지막 result값을 복사함 => 시그마 1~ (1,000,000,000 / 40) 한 값이 result에 저장됨
    result = 0;
    start = high_resolution_clock::now(); 
    #pragma omp  parallel num_threads(40) 
    {
        #pragma omp for firstprivate(result) lastprivate(result)
        for(j=0;j<1000000000;j++)
        {
            result +=j;
        }
    }
    stop = high_resolution_clock::now(); 
    duration = duration_cast<microseconds>(stop - start);

    cout << "Test parallel-       "<< duration.count() << " clock    " << "---result : "<< result <<"\n"; 


}