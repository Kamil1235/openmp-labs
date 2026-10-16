// lab1_1.cpp — задача 1
// Каждый поток печатает id, число потоков, "Hello World".
#include <omp.h>
#include <cstdio>
#include <cstdlib>

int main(int argc, char** argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <num_threads>\n", argv[0]);
        return 1;
    }
    int nthreads = atoi(argv[1]);
    omp_set_num_threads(nthreads);

#pragma omp parallel
    {
        int tid = omp_get_thread_num();
        int nth = omp_get_num_threads();
#pragma omp critical
        printf("Thread %d of %d: Hello World\n", tid, nth);
    }
    return 0;
}