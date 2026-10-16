// lab1_2.cpp — задача 2
// b[i] = (a[i-1]+a[i]+a[i+1])/3, разные типы schedule.
#include <omp.h>
#include <cstdio>
#include <cstdlib>

int main(int argc, char** argv) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <num_threads> <schedule: static|dynamic|guided> [chunk]\n", argv[0]);
        return 1;
    }
    int nthreads = atoi(argv[1]);
    const char* sched = argv[2];
    int chunk = (argc >= 4) ? atoi(argv[3]) : 0;

    const int N = 16000;
    static double a[N], b[N];

    for (int i = 0; i < N; ++i) a[i] = (double)i;

    omp_set_num_threads(nthreads);
    double t0 = omp_get_wtime();

    // schedule(runtime) — тип берётся из переменной окружения OMP_SCHEDULE
#pragma omp parallel for schedule(runtime)
    for (int i = 1; i < N - 1; ++i) {
        b[i] = (a[i - 1] + a[i] + a[i + 1]) / 3.0;
    }

    double t1 = omp_get_wtime();

    printf("schedule=%s chunk=%d threads=%d time=%.6f s\n",
        sched, chunk, nthreads, t1 - t0);
    printf("b[1]=%.3f b[100]=%.3f b[15998]=%.3f\n",
        b[1], b[100], b[N - 2]);
    return 0;
}