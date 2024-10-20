#include <iostream>
#include <complex>
#include <chrono>
#include <vector>
#include <thread>
// #include <jthread>
#include <mutex>

const int width = 1920;
const int height = 1080;
const int maxIter = 1000;
std::mutex mtx;

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

// تابع تسک محاسبات برای اجرای هر بخش از تصویر
void computeMandelbrot(int startY, int endY, int* image) {
    for (int y = startY; y < endY; ++y) {
        for (int x = 0; x < width; ++x) {
            std::complex<double> point(
                (x - width / 2.0) * 4.0 / width,
                (y - height / 2.0) * 4.0 / height
            );
            int result = mandelbrot(point);
            {
                std::scoped_lock lock(mtx);
                image[y * width + x] = result;
            }
        }
    }
}

int main() {
    auto start = std::chrono::high_resolution_clock::now();

    // ایجاد یک ماتریس برای نگهداری نتایج
    int* image = new int[width * height];

    int numThreads = std::thread::hardware_concurrency();
    std::vector<std::jthread> threads;

    // تقسیم کار بر اساس تعداد نخ‌ها
    int rowsPerThread = height / numThreads;

    for (int i = 0; i < numThreads; ++i) {
        int startY = i * rowsPerThread;
        int endY = (i == numThreads - 1) ? height : startY + rowsPerThread;
        threads.emplace_back(computeMandelbrot, startY, endY, image);
    }

    // نخ‌ها به طور خودکار با خروج از محدوده به پایان می‌رسند

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    std::cout << "jthread Elapsed time: " << elapsed.count() << " seconds\n";

    delete[] image;
    return 0;
}
