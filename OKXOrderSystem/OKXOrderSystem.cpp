#include "okx_client.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>

void loadEnvFile(const std::string& filename) {
    std::ifstream envFile(filename);
    if (!envFile.is_open()) {
        std::cerr << "Error: Could not open .env file." << std::endl;
        return;
    }

    std::string line;
    while (std::getline(envFile, line)) {
        size_t pos = line.find('=');
        if (pos != std::string::npos) {
            std::string key = line.substr(0, pos);
            std::string value = line.substr(pos + 1);

            // Set the environment variable on Windows
            _putenv_s(key.c_str(), value.c_str());
        }
    }

    envFile.close();
}



int main() {
   
    loadEnvFile(".env");

    char* apiKey = nullptr;
    char* secretKey = nullptr;
    char* passphrase = nullptr;

    // Use _dupenv_s to safely retrieve environment variables
    _dupenv_s(&apiKey, nullptr, "OKX_API_KEY");
    _dupenv_s(&secretKey, nullptr, "OKX_SECRET_KEY");
    _dupenv_s(&passphrase, nullptr, "OKX_PASSPHRASE");

    if (!apiKey || !secretKey || !passphrase) {
        std::cerr << "Error: API key, secret key, or passphrase environment variable is not set." << std::endl;
        // Free memory before exiting
        free(apiKey);
        free(secretKey);
        free(passphrase);
        return 1;
    }

    OKXClient client(apiKey, secretKey, passphrase);

    // Free memory after usage
    free(apiKey);
    free(secretKey);
    free(passphrase);
   
    // Example usage
    //client.placeOrder("MAGIC-USDT-SWAP", "buy", "limit", 1, 0.3); // works for open order
    //client.placeOrder("MAGIC-USDT-SWAP", "buy", "limit", 1, 0.345); // works for filled order
    //client.getPendingOrders( "limit", "SWAP"); //works
    //client.getOrderHistory("MAGIC-USDT-SWAP","SWAP");
   //client.cancelOrder( "MAGIC-USDT-SWAP", "1702742015357800448"); //works
   client.getOpenOrders("MAGIC-USDT-SWAP", "SWAP"); //works
   //client.modifyOrder("1703908579612348416", "MAGIC-USDT-SWAP", "2", "0.352"); //works
    //client.getOpenOrders("MAGIC-USDT-SWAP", "SWAP"); //works

    //std::cout << client.getOrderBook("MAGIC-USDT-SWAP") << std::endl;
    //std::cout << client.getCurrentPositions() << std::endl;

    return 0;
}
