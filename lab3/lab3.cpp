// lab3.cpp — Задача 5
#include <omp.h>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <fstream>

struct Point { double x, y, z; };

int main(int argc, char** argv) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <filename> <mode: for|sections>\n", argv[0]);
        return 1;
    }
    const char* filename = argv[1];
    const char* mode = argv[2];

    // 1. Чтение данных из файла (последовательно)
    std::ifstream file(filename);
    if (!file.is_open()) {
        fprintf(stderr, "Cannot open file %s\n", filename);
        return 1;
    }
    std::vector<Point> points;
    double x, y, z;
    while (file >> x >> y >> z) {
        points.push_back({ x, y, z });
    }
    file.close();

    int N = (int)points.size();
    if (N == 0) {
        fprintf(stderr, "No points loaded\n");
        return 1;
    }

    printf("Loaded %d points from %s\n", N, filename);

    double sumX = 0.0, sumY = 0.0, sumZ = 0.0;
    double t0 = omp_get_wtime();

    // 2. Вычисления
    if (mode[0] == 'f') {
        // Версия 1: распараллеливание цикла
#pragma omp parallel for reduction(+:sumX, sumY, sumZ)
        for (int i = 0; i < N; ++i) {
            sumX += points[i].x;
            sumY += points[i].y;
            sumZ += points[i].z;
        }
    }
    else if (mode[0] == 's') {
        // Версия 2: функциональная декомпозиция
#pragma omp parallel sections reduction(+:sumX, sumY, sumZ)
        {
#pragma omp section
            {
                for (int i = 0; i < N; ++i) sumX += points[i].x;
            }
#pragma omp section
            {
                for (int i = 0; i < N; ++i) sumY += points[i].y;
            }
#pragma omp section
            {
                for (int i = 0; i < N; ++i) sumZ += points[i].z;
            }
        }
    }

    double t1 = omp_get_wtime();

    // 3. Вывод
    printf("Mode: %s, threads: %d, time: %.6f s\n",
        (mode[0] == 'f' ? "parallel for" : "sections"),
        omp_get_max_threads(), t1 - t0);

    printf("Center: x=%.4f, y=%.4f, z=%.4f\n", sumX / N, sumY / N, sumZ / N);

    return 0;
}