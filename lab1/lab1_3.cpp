// lab1_3.cpp — задача 3
// Печать id потоков в обратном порядке — 6 способов.
#include <omp.h>
#include <cstdio>
#include <cstdlib>
#include <vector>

// Способ 1. ordered + обратный индекс
static void method1(int nthreads) {
    omp_set_num_threads(nthreads);
#pragma omp parallel
    {
        int nth = omp_get_num_threads();
#pragma omp for ordered
        for (int k = 0; k < nth; ++k) {
#pragma omp ordered
            printf("[1 ordered] Thread %d of %d\n", nth - 1 - k, nth);
        }
    }
    printf("----\n");
}

// Способ 2. lock + эстафета по номеру
static void method2(int nthreads) {
    omp_set_num_threads(nthreads);
    static omp_lock_t lock;
    static int turn;
    omp_init_lock(&lock);
#pragma omp parallel
    {
        int tid = omp_get_thread_num();
        int nth = omp_get_num_threads();
        int expected = nth - 1 - tid;
        for (;;) {
            omp_set_lock(&lock);
            if (turn == expected) {
                printf("[2 lock] Thread %d of %d\n", tid, nth);
                ++turn;
                omp_unset_lock(&lock);
                break;
            }
            omp_unset_lock(&lock);
        }
    }
    omp_destroy_lock(&lock);
    printf("----\n");
}

// Способ 3. critical + счётчик
static void method3(int nthreads) {
    omp_set_num_threads(nthreads);
    static int next_to_print;
    next_to_print = nthreads - 1;
#pragma omp parallel
    {
        int tid = omp_get_thread_num();
        int nth = omp_get_num_threads();
        for (;;) {
            int done = 0;
#pragma omp critical
            {
                if (tid == next_to_print) {
                    printf("[3 critical] Thread %d of %d\n", tid, nth);
                    --next_to_print;
                    done = 1;
                }
            }
            if (done) break;
        }
    }
    printf("----\n");
}

// Способ 4. single + обратный цикл
static void method4(int nthreads) {
    omp_set_num_threads(nthreads);
#pragma omp parallel
    {
#pragma omp single
        {
            int nth = omp_get_num_threads();
            for (int i = nth - 1; i >= 0; --i)
                printf("[4 single] Thread %d of %d\n", i, nth);
        }
    }
    printf("----\n");
}

// Способ 5. master + обратный цикл
static void method5(int nthreads) {
    omp_set_num_threads(nthreads);
#pragma omp parallel
    {
#pragma omp master
        {
            int nth = omp_get_num_threads();
            for (int i = nth - 1; i >= 0; --i)
                printf("[5 master] Thread %d of %d\n", i, nth);
        }
    }
    printf("----\n");
}

// Способ 6. flag-эстафета
static void method6(int nthreads) {
    omp_set_num_threads(nthreads);
    volatile int flag[64];                 // массив фиксированного размера
    for (int i = 0; i < 64; ++i) flag[i] = 0;
    flag[nthreads - 1] = 1;

#pragma omp parallel
    {
        int tid = omp_get_thread_num();
        int nth = omp_get_num_threads();
        while (!flag[tid]) { /* spin */ }
        printf("[6 flag] Thread %d of %d\n", tid, nth);
#pragma omp flush(flag)
        if (tid > 0) {
            flag[tid - 1] = 1;
#pragma omp flush(flag)
        }
    }
    printf("----\n");
}

int main(int argc, char** argv) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <method 1..6> <num_threads>\n", argv[0]);
        return 1;
    }
    int method = atoi(argv[1]);
    int nth = atoi(argv[2]);
    switch (method) {
    case 1: method1(nth); break;
    case 2: method2(nth); break;
    case 3: method3(nth); break;
    case 4: method4(nth); break;
    case 5: method5(nth); break;
    case 6: method6(nth); break;
    default: fprintf(stderr, "Unknown method\n"); return 1;
    }
    return 0;
}