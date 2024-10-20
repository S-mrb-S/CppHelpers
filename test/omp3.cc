#include <iostream>
#include <complex>
#include <chrono>
#include <omp.h>

const int width = 1920;
const int height = 1080;
const int maxIter = 1000;

// تابع برای محاسبه‌ی Mandelbrot
int mandelbrot(std::complex<double> c) {
    std::complex<double> z = 0;
    int iter = 0;
    while (std::abs(z) <= 2.0 && iter < maxIter) {
        z = z * z + c;
        iter++;
    }
    return iter;
}

int main() {
    auto start = std::chrono::high_resolution_clock::now();

    // ایجاد یک ماتریس برای نگهداری نتایج
    int* image = new int[width * height];

    // موازی سازی با OpenMP
    #pragma omp parallel for collapse(2)
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            std::complex<double> point(
                (x - width / 2.0) * 4.0 / width,
                (y - height / 2.0) * 4.0 / height
            );
            image[y * width + x] = mandelbrot(point);
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    std::cout << "OpenMP Elapsed time: " << elapsed.count() << " seconds\n";

    delete[] image;
    return 0;
}
