#ifndef OKX_CLIENT_H
#define OKX_CLIENT_H

#include <string>

class OKXClient {
public:
    OKXClient(const std::string& apiKey, const std::string& secretKey, const std::string& passphrase);

    void placeOrder(const std::string& symbol, const std::string& side, const std::string& type, double size, double price);
    void cancelOrder(const std::string& instId, const std::string& ordId="", const std::string& clOrdId="");
    void modifyOrder(const std::string& ordId, const std::string& instId, const std::string& newSz = "", const std::string& newPx = "");
    void getPendingOrders(const std::string& ordType, const std::string& instType);
    void getOrderHistory(const std::string& symbol, const std::string& instType);
    void getOpenOrders(const std::string& instId, const std::string& instType);


    std::string getOrderBook(const std::string& symbol);
    std::string getCurrentPositions();
    //std::string getInstrumentDetails(const std::string& symbol);

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

