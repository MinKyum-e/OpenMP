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
    
    for(j=0;j<10000000000;j++)
    {
        result +=1;
    }

    auto stop = high_resolution_clock::now(); 
    auto duration = duration_cast<microseconds>(stop - start);
    cout << "Test sequential-    " << duration.count()<< " clock    " << "---result : "<< result <<"\n"; 


    //parallel
    result = 0;
    start = high_resolution_clock::now(); 
    #pragma omp  parallel num_threads(40) 
    {
        #pragma omp for reduction(+: result)
        for(j=0;j<10000000000;j++)
        {
            result +=1;
        }
    }
    stop = high_resolution_clock::now(); 
    duration = duration_cast<microseconds>(stop - start);

    cout << "Test parallel-       "<< duration.count() << " clock    " << "---result : "<< result <<"\n"; 


}