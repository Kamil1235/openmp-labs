// lab4.cpp — Задача 6
#include <omp.h>
#include <cstdio>
#include <vector>
#include <fstream>

struct Point { double x, y, z; };

int main(int argc, char** argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return 1;
    }
    const char* filename = argv[1];

    // 1. Чтение данных из файла
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
    if (N == 0) return 1;

    printf("Loaded %d points from %s\n", N, filename);

    // 2. Глобальная сумма
    double total_sum = 0.0;
    double t0 = omp_get_wtime();

    // 3. Функциональная декомпозиция с критическими секциями
#pragma omp parallel sections
    {
#pragma omp section
        {
            double sumX = 0.0;
            for (int i = 0; i < N; ++i) sumX += points[i].x;

#pragma omp critical
            {
                total_sum += sumX;
            }
        }

#pragma omp section
        {
            double sumY = 0.0;
            for (int i = 0; i < N; ++i) sumY += points[i].y;

#pragma omp critical
            {
                total_sum += sumY;
            }
        }

#pragma omp section
        {
            double sumZ = 0.0;
            for (int i = 0; i < N; ++i) sumZ += points[i].z;

#pragma omp critical
            {
                total_sum += sumZ;
            }
        }
    }

    double t1 = omp_get_wtime();

    // 4. Вычисление по формуле: (Σx + Σy + Σz) / 3N
    double result = total_sum / (3.0 * N);

    printf("Mode: sections + critical, threads: %d, time: %.6f s\n",
        omp_get_max_threads(), t1 - t0);
    printf("Global sum: %.4f\n", total_sum);
    printf("Formula result (total_sum / 3N): %.4f\n", result);

    return 0;
}