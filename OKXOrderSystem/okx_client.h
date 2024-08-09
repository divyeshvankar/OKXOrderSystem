/*#ifndef OKX_CLIENT_H
#define OKX_CLIENT_H

#include <string>
#include <vector>
#include <iostream>
#include <string>
#include <utility>

class OKXClient {
public:
    OKXClient(const std::string& apiKey, const std::string& secretKey, const std::string& passphrase);

    void placeOrder(const std::string& symbol, const std::string& side, const std::string& type, double size, double price);
    void cancelOrder(const std::string& orderId);
    void modifyOrder(const std::string& orderId, double newSize, double newPrice);
    std::pair<double, double> getPriceLimits(const std::string& symbol);
    std::string getOrderBook(const std::string& symbol);
    std::string getCurrentPositions();

private:
    std::string apiKey;
    std::string secretKey;
    std::string passphrase;
    std::string baseUrl;

    std::string sendRequest(const std::string& endpoint, const std::string& method, const std::string& body = "");
    std::string createSignature(const std::string& timestamp, const std::string& method, const std::string& endpoint, const std::string& body);

    std::string getISO8601Timestamp();
};

#endif // OKX_CLIENT_H
#pragma once
*/


#ifndef OKX_CLIENT_H
#define OKX_CLIENT_H

#include <string>

class OKXClient {
public:
    OKXClient(const std::string& apiKey, const std::string& secretKey, const std::string& passphrase);

    void placeOrder(const std::string& symbol, const std::string& side, const std::string& type, double size, double price);
    void cancelOrder(const std::string& orderId);
    void modifyOrder(const std::string& orderId, double newSize, double newPrice);
    std::string getOrderBook(const std::string& symbol);
    std::string getCurrentPositions();
    std::string getInstrumentDetails(const std::string& symbol);

private:
    std::string apiKey;
    std::string secretKey;
    std::string passphrase;
    std::string baseUrl;

    std::string getISO8601Timestamp();
    std::string createSignature(const std::string& timestamp, const std::string& method, const std::string& endpoint, const std::string& body);
    std::string sendRequest(const std::string& endpoint, const std::string& method, const std::string& body = "");
};

#endif // OKX_CLIENT_H

