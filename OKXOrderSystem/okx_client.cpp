
#include "okx_client.h"
#include <chrono>
#include <iomanip>
#include <sstream>
#include <ctime>
#include <openssl/hmac.h>
#include <openssl/buffer.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/sha.h>
#include <curl/curl.h>
#include <iostream>
#include <string>
#include <vector>

// Base64 Encoding Function
std::string base64Encode(const unsigned char* data, size_t length) {
    BIO* bmem, * b64;
    BUF_MEM* bptr;
    b64 = BIO_new(BIO_f_base64());
    BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL); // Don't add newline characters
    bmem = BIO_new(BIO_s_mem());
    b64 = BIO_push(b64, bmem);
    BIO_write(b64, data, length);
    BIO_flush(b64);
    BIO_get_mem_ptr(b64, &bptr);

    std::string encoded(bptr->data, bptr->length);
    BIO_free_all(b64);

    return encoded;
}

// Write Callback Function
size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    size_t totalSize = size * nmemb;
    std::string* str = (std::string*)userp;
    str->append((char*)contents, totalSize);
    return totalSize;
}

OKXClient::OKXClient(const std::string& apiKey, const std::string& secretKey, const std::string& passphrase)
    : apiKey(apiKey), secretKey(secretKey), passphrase(passphrase), baseUrl("https://www.okx.com") {}

// Function to get the current timestamp in ISO8601 format
std::string OKXClient::getISO8601Timestamp() {
    using namespace std::chrono;
    auto now = system_clock::now();
    auto now_seconds = system_clock::to_time_t(now);
    std::tm tm = {};
    gmtime_s(&tm, &now_seconds);
    auto now_ms = duration_cast<milliseconds>(now.time_since_epoch()) % 1000;
    std::stringstream ss;
    ss << std::put_time(&tm, "%Y-%m-%dT%H:%M:%S")
        << '.' << std::setw(3) << std::setfill('0') << now_ms.count()
        << 'Z';
    return ss.str();
}

// Function to create HMAC-SHA256 signature
std::string OKXClient::createSignature(const std::string& timestamp, const std::string& method, const std::string& endpoint, const std::string& body) {
    std::string data = timestamp + method + endpoint + body;
    unsigned char* digest;
    digest = HMAC(EVP_sha256(), secretKey.c_str(), secretKey.length(), reinterpret_cast<const unsigned char*>(data.c_str()), data.length(), NULL, NULL);
    return base64Encode(digest, SHA256_DIGEST_LENGTH);
}

// Function to send a request to the OKX API
std::string OKXClient::sendRequest(const std::string& endpoint, const std::string& method, const std::string& body) {
    std::string url = baseUrl + endpoint;
    CURL* curl;
    CURLcode res;
    curl = curl_easy_init();
    std::string readBuffer;
    if (curl) {
        std::string timestamp = getISO8601Timestamp();
        std::string signature = createSignature(timestamp, method, endpoint, body);

        struct curl_slist* headers = NULL;
        headers = curl_slist_append(headers, ("OK-ACCESS-KEY: " + apiKey).c_str());
        headers = curl_slist_append(headers, ("OK-ACCESS-SIGN: " + signature).c_str());
        headers = curl_slist_append(headers, ("OK-ACCESS-TIMESTAMP: " + timestamp).c_str());
        headers = curl_slist_append(headers, ("OK-ACCESS-PASSPHRASE: " + passphrase).c_str());
        headers = curl_slist_append(headers, "Content-Type: application/json");
        headers = curl_slist_append(headers, "x-simulated-trading: 1");

        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        if (method == "POST") {
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body.c_str());
        }

        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }
        curl_easy_cleanup(curl);
    }
    return readBuffer;
}

// Functions for placing, modifying, canceling orders, etc.
void OKXClient::placeOrder(const std::string& symbol, const std::string& side, const std::string& type, double size, double price) {
    // Check and adjust price within the allowed range
    double maxBuyPrice = 0.4888;
    double minSellPrice = 0.4511;

    if (side == "buy" && price > maxBuyPrice) {
        std::cout << "Price exceeds maximum buy price. Adjusting to " << maxBuyPrice << std::endl;
        price = maxBuyPrice;
    }
    else if (side == "sell" && price < minSellPrice) {
        std::cout << "Price is below minimum sell price. Adjusting to " << minSellPrice << std::endl;
        price = minSellPrice;
    }

    // Ensure size is a multiple of the lot size
    double lotSize = 1.0;  // Replace this with the actual lot size for the instrument
    size = std::floor(size / lotSize) * lotSize;

    std::string endpoint = "/api/v5/trade/order";
    std::string body = R"({"instId":")" + symbol + R"(","tdMode":"cross","side":")" + side + R"(","ordType":")" + type + R"(","sz":")" + std::to_string(size) + R"(","px":")" + std::to_string(price) + R"("})";
    std::string response = sendRequest(endpoint, "POST", body);
    std::cout << "Place Order Response: " << response << std::endl;
}


void OKXClient::cancelOrder(const std::string& orderId) {
    std::string endpoint = "/api/v5/trade/cancel-order";
    std::string body = R"({"ordId":")" + orderId + R"("})";
    std::string response = sendRequest(endpoint, "POST", body);
    std::cout << "Cancel Order Response: " << response << std::endl;
}

void OKXClient::modifyOrder(const std::string& orderId, double newSize, double newPrice) {
    std::string endpoint = "/api/v5/trade/amend-order";
    std::string body = R"({"ordId":")" + orderId + R"(","newSz":")" + std::to_string(newSize) + R"(","newPx":")" + std::to_string(newPrice) + R"("})";
    std::string response = sendRequest(endpoint, "POST", body);
    std::cout << "Modify Order Response: " << response << std::endl;
}

std::string OKXClient::getOrderBook(const std::string& symbol) {
    std::string endpoint = "/api/v5/market/books?instId=" + symbol;
    std::string response = sendRequest(endpoint, "GET");
    return response;
}

std::string OKXClient::getCurrentPositions() {
    std::string endpoint = "/api/v5/account/positions";
    std::string response = sendRequest(endpoint, "GET");
    return response;
}
