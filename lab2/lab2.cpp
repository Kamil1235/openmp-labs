// lab2.cpp — задача 4
// Умножение больших матриц: последовательная и параллельная версии.
#include <omp.h>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <cmath>
#include <chrono>

struct Matrix {
    int n;
    std::vector<double> data;
    Matrix(int n_) : n(n_), data((size_t)n_* n_, 0.0) {}
    inline double& at(int i, int j) { return data[(size_t)i * n + j]; }
    inline double        at(int i, int j) const { return data[(size_t)i * n + j]; }
};

static Matrix make_matrix(int n) {
    Matrix m(n);
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            m.at(i, j) = ((i * 31 + j * 17) % 100) / 10.0;
    return m;
}

// Последовательная версия
static Matrix mul_seq(const Matrix& A, const Matrix& B) {
    int n = A.n;
    Matrix C(n);
    for (int i = 0; i < n; ++i)
        for (int k = 0; k < n; ++k) {
            double aik = A.at(i, k);
            const double* Brow = &B.data[(size_t)k * n];
            double* Crow = &C.data[(size_t)i * n];
            for (int j = 0; j < n; ++j)
                Crow[j] += aik * Brow[j];
        }
    return C;
}

// Параллельная версия
static Matrix mul_par(const Matrix& A, const Matrix& B, int nthreads) {
    int n = A.n;
    Matrix C(n);
    omp_set_num_threads(nthreads);
#pragma omp parallel for schedule(static)
    for (int i = 0; i < n; ++i)
        for (int k = 0; k < n; ++k) {
            double aik = A.at(i, k);
            const double* Brow = &B.data[(size_t)k * n];
            double* Crow = &C.data[(size_t)i * n];
            for (int j = 0; j < n; ++j)
                Crow[j] += aik * Brow[j];
        }
    return C;
}

int main(int argc, char** argv) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <size> <threads>\n", argv[0]);
        return 1;
    }
    int n = atoi(argv[1]);
    int nth = atoi(argv[2]);

    Matrix A = make_matrix(n), B = make_matrix(n);

    auto t0 = std::chrono::high_resolution_clock::now();
    Matrix Cseq = mul_seq(A, B);
    auto t1 = std::chrono::high_resolution_clock::now();

    auto t2 = std::chrono::high_resolution_clock::now();
    Matrix Cpar = mul_par(A, B, nth);
    auto t3 = std::chrono::high_resolution_clock::now();

    double seq = std::chrono::duration<double>(t1 - t0).count();
    double par = std::chrono::duration<double>(t3 - t2).count();

    double diff = 0.0;
    for (size_t k = 0; k < Cseq.data.size(); ++k)
        diff += std::fabs(Cseq.data[k] - Cpar.data[k]);

    printf("n=%d threads=%d\n", n, nth);
    printf("seq: %.4f s\n", seq);
    printf("par: %.4f s\n", par);
    printf("speedup: %.2fx\n", seq / par);
    printf("diff: %.6f\n", diff);
    return 0;
}