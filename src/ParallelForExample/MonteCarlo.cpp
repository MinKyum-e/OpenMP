#include <iostream>
#include <omp.h>
#include <cstdlib>
#include <ctime>
#include <chrono>

using namespace std;
using namespace std::chrono;

#define SAMPLE_NUM 100000000

int main() {
    // 시드 변수 정의
    unsigned int seed = time(nullptr);
    long n_points = SAMPLE_NUM;

    // Sequential execution
    auto start = high_resolution_clock::now(); 
    long sum = 0;
    double x, y;
    for (int i = 0; i < n_points; i++) {   
        // 시드 포인터를 rand_r에 전달
        x = (double)rand_r(&seed) / RAND_MAX;
        y = (double)rand_r(&seed) / RAND_MAX;
        if (x * x + y * y < 1.0f) {
            sum++;
        }
    }
    auto stop = high_resolution_clock::now(); 
    auto duration = duration_cast<microseconds>(stop - start);
    cout << "Test sequential - " << duration.count() << " microseconds    " << "--- result : " << (double)sum / SAMPLE_NUM * 4<< "\n";

    //parallel - reduce
    start = high_resolution_clock::now(); 
    
    long points_per_thread = (long)((double)n_points/40);
    cout << points_per_thread<<endl;
    int sum2 = 0;
    int i;
#pragma omp parallel num_threads(8) private(x, y)
{
    unsigned int thread_seed = time(nullptr)+ omp_get_thread_num(); //thread별로 seed를 지정해줘야함.

    /*
        *같은 seed를 쓰면 느려지는 이유

        rand() 함수와 달리, rand_r()는 전역 변수나 정적 변수에 의존하지 않는다.
        rand()는 전역 상태로 관리되는 시드를 사용하기 때문에 스레드가 동시에 호출하면 race condition이 발생한다.
        반면 rand_r()는 호출자가 시드 값을 전달하기 때문에, 스레드 간에 공유되는 상태 없이 독립적으로 동작한다.
        하지만 rand_r을 사용하더라도 seed값을 global로 저장해두고 사용한다면? => rand()를 사용하는 것과 다름이없다.

        원인
        1. race condition : 
            - thread별 seed를 사용하지 않고 global seed를 그대로 사용하게 되면 모든 thread들이 같은 seed에 접근하게되고 rand_r을 실행할때마다
            seed 값을 업데이트하려고한다. 이때 race condition이 발생한다. => rand()를 사용하는 것과 다름이 없다.

        2. false sharing : 
            - rand_r을 할때마다 시드 값을 수정하게 된다. 글로벌 seed를 사용하는경우
            각 쓰레드가 시드를 갱신할때 CPU cache Coherence를 유지하기 위해 메모리 위치를 동기화해야하고 이로인해 액세스 비용이 발생한다.


        *왜 정확도가 떨어질까?
        1. 여러 쓰레드들이 같은 seed값을 읽어서 x, y값을 가져오기 때문에 중복되는 값이 많다.
        2. 순차적으로 seed값을 업데이트해야 rand의 의미가 있는 것인데 race condition으로 인해 이미 업데이트 한 값을 또 다시 seed로 지정하고
            또 똑같은 seed를 공유하게 되면서 비순차적으로 시드를 갱신하게 되어 업데이트 순서가 뒤엉켜 정확한 계산결과를 얻을 수 없다.,


    */
    #pragma omp for reduction(+: sum2)
    for (i = 0; i < n_points; i++) {   
        // 시드 포인터를 rand_r에 전달
        x = (double)rand_r(&thread_seed) / RAND_MAX;
        y = (double)rand_r(&thread_seed) / RAND_MAX;
        if (x * x + y * y < 1.0f) {
            sum2++;
        }
    }
}
    stop = high_resolution_clock::now(); 
     duration = duration_cast<microseconds>(stop - start);
    cout << "Test sequential - " << duration.count() << " microseconds    " << "--- result : " << (double)sum2 / SAMPLE_NUM * 4<< "\n";



}