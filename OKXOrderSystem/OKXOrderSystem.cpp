#include "okx_client.h"
#include <iostream>

int main() {
    const std::string apiKey = "6cd8938d-3e4d-4447-b864-2bb62a6ac4c8";
    const std::string secretKey = "813491CFDB3C732CC82F6A1BC28C71B4";
    const std::string passphrase = "Divyesh@2019";
    //const std::string apiKey = "f7db8d7a-b05e-458a-8cc7-b10252f60f30";
    //const std::string secretKey = "315579F51BC76893B614B795675E56374";
    //const std::string passphrase = "GoQuant1$";
    OKXClient client(apiKey, secretKey, passphrase);
    //const std::string s=client.getInstrumentDetails("MAGIC-USDT-SWAP");
    //std::cout << s << std::endl;
    // Example usage
    client.placeOrder("MAGIC-USDT-SWAP", "buy", "limit", 1, 0.3);
    //client.cancelOrder("12345");
    client.modifyOrder("12345", 2.0, 1.1);
    std::cout << client.getOrderBook("MAGIC-USDT-SWAP") << std::endl;
    std::cout << client.getCurrentPositions() << std::endl;

    return 0;
}


// apikey = "c42bcd4a-bdc1-4b76-8b08-27557732fc7a"
// secretkey = "D9F6D79BE0851EAFC73AD28AE6B02827"
// IP = ""
// API name = "Test API Demo"
// Permissions = "Read/Withdraw/Trade"


//apikey = "6cd8938d-3e4d-4447-b864-2bb62a6ac4c8"
//secretkey = "813491CFDB3C732CC82F6A1BC28C71B4"
//IP = ""
//API name = "otherdemo"
//Permissions = "Read/Withdraw/Trade"


//apikey = "00ecca74-58c4-40e3-91c2-1e4ffd2045c4"
//secretkey = "D94A73560432B9309E5FF60C19D8A430"
//IP = ""
//API name = "test1"
//Permissions = "Read/Withdraw/Trade"