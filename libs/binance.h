
// #ifndef BINANCE_ORDER_CLIENT_H
// #define BINANCE_ORDER_CLIENT_H

// #include <iostream>
// #include <string>
// #include <map>
// #include <sstream>
// #include <iomanip>
// #include <chrono>
// #include <thread>
// #include <openssl/hmac.h>
// #include <curl/curl.h>

// #include <vector>

// #include <memory>
// #include "../includes/Constant.h"
// #include "../includes/HTTPClient.h"
// using namespace constant_nameSpace;
// class BinanceOrderClient {
// public:
//     static std::string placeOrder(const std::string& symbol,
//                                    const std::string& side, const std::string& orderType, const std::string& quantity,
//                                    const std::string& price);
//     static std::string getListenKey(const std::string& apiKey);  
//     static std::string LOT_SIZE_CALCULATOR(float quantity, const std::string& step_size_filter);
//     static std::string makePre(float f, int n); 
//     static LOTSIZE getLotSize(std::string& symbol);
// private:
//     static const std::string BINANCE_API_BASE_URL;
//     static const std::string END_POINT;

//     // static int countPresion(const std::string& number_str);
//     // static double CalculateLotQuantity(const std::string& text, size_t length_to_skip);
//     // // int countPresion(const std::string& number_str);
//     // double CalculateLotQuantity(const std::string& text, size_t length_to_skip);
//     // double CalculateLotQuantity(const std::string& text, size_t length_to_skip);
//     static std::string createParamsForOrder(const std::string& symbol, const std::string& side,
//                                             const std::string& orderType, const std::string& quantity,
//                                             const std::string& price, const std::string& apiKey);
    
//     static std::string hmacSha256(const std::string& key, const std::string& data);

// };

// const std::string BinanceOrderClient::BINANCE_API_BASE_URL = "https://api.binance.com/api/v3/";
// const std::string BinanceOrderClient::END_POINT = "order";

// std::string BinanceOrderClient::placeOrder(const std::string& symbol, const std::string& side,
//                                            const std::string& orderType, const std::string& quantity,
//                                            const std::string& price) {
//     try {
//         // Request parameters
//         // std::string symbol = "BTCUSDT";

//         LOTSIZE lot = BinanceOrderClient::getLotSize(symbol);

// // 2. Calculate quantity properly
//         std::string final_quantity = BinanceOrderClient::LOT_SIZE_CALCULATOR(std::stof(quantity), lot.stepSize);

//         std::string apiKey = "PuR4xDBcUuxQ0kD9ZU5k8KG1mmbkVVuBmi8NLTOWTSAXa1F2N8g772bkBuzCHNux";
//         std::string secretKey = "3TmSTkfyNO5ixwsFb0S4olLxt9FYdK7wX5DHHiA7JAwpJUlcUY4okwrvqiyHamXt";
//         std::string _BINANCE_API_BASE_URL  = "https://testnet.binance.vision/api/v3/";
//         std::string data = createParamsForOrder(symbol, side, orderType, final_quantity, price, apiKey);
        
//         // Create the signature
//         std::string signature = hmacSha256(secretKey, data);
        
//         // Construct the URL
//         std::string url = _BINANCE_API_BASE_URL + END_POINT + "?" + data + "&signature=" + signature;
        
//         // Send the request
//         std::vector<std::string> headers = {"X-MBX-APIKEY: " + apiKey};
//         std::string response = HTTPClient::sendPOSTRequest(url, "", headers);
        
//         // std::cout << response->getBody() << std::endl; // Logging
        
//         return response;
//     } catch (const std::exception& e) {
//         std::cerr << "Error placing order: " << e.what() << std::endl;
//         throw; // Rethrow the exception
//     }
// }

// std::string BinanceOrderClient::getListenKey(const std::string& apiKey) {
//     try {
       
        
//         const std::string END_POINT = "userDataStream";
//         std::string url = BINANCE_API_BASE_URL + END_POINT ;
        
//         // Send the request
//          std::vector<std::string> headers;
//         std::string api_key_header = "X-MBX-APIKEY: "+apiKey;
//         headers.push_back(api_key_header);
//         std::cout << "Sending Request";
//         std::string response = HTTPClient::sendPOSTRequest(url, "", headers);
        
//         std::cout << response<< std::endl; // Logging
        
//         return response;
//     } catch (const std::exception& e) {
//         std::cerr << "Error placing order: " << e.what() << std::endl;
//         throw; // Rethrow the exception
//     }
// }
// std::string BinanceOrderClient::createParamsForOrder(const std::string& symbol, const std::string& side,
//                                                      const std::string& orderType, const std::string& quantity,
//                                                      const std::string& price, const std::string& apiKey) {
//     // Generate timestamp
//     long long timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(
//         std::chrono::system_clock::now().time_since_epoch()).count();
    
//     // Construct request parameters
//     std::stringstream ss;
//     // ss << "symbol=" << symbol << "&side=" << side << "&type=MARKET&timeInForce=GTC"
//     //    << "&quantity=" << quantity << "&price=" << price << "&timestamp=" << timestamp;
//     ss << "symbol=" << symbol << "&side=" << side << "&type=MARKET"
//        << "&quantity=" << quantity << "&timestamp=" << timestamp;
//     std::cout << ss.str()<<std::endl;
//     return ss.str();
// }

// std::string BinanceOrderClient::hmacSha256(const std::string& key, const std::string& data) {
//     unsigned char digest[EVP_MAX_MD_SIZE];
//     unsigned int digest_len;
    
//     HMAC(EVP_sha256(), key.c_str(), key.length(),
//          reinterpret_cast<const unsigned char*>(data.c_str()), data.length(), digest, &digest_len);
    
//     std::stringstream ss;
//     for (unsigned int i = 0; i < digest_len; ++i) {
//         ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(digest[i]);
//     }
//     return ss.str();
// }
// std::string BinanceOrderClient::LOT_SIZE_CALCULATOR(float quantity, const std::string& step_size_filter ) {
//     std::string stepSize = std::to_string(std::stof(step_size_filter));
//     std::cout << "Step size filter: " << stepSize << std::endl;
//      std::cout << "Quantity: " << quantity << std::endl;
    
//     if (stepSize.find("1.") != std::string::npos) {
//         // Only whole values are accepted, truncate to zero decimals
//         std::cout << "Whole values detected\n";
//         return makePre(quantity, 0);
//     } else {
//         // Extract the number of decimal places allowed based on the step size
//         size_t dotPosition = stepSize.find('.');
//         if (dotPosition != std::string::npos) {
//             size_t decimalPlaces = stepSize.size() - dotPosition - 1;
//             std::cout << "Decimal places: " << decimalPlaces << std::endl;
//             // Truncate the value to the specified number of decimal places
//             return makePre(quantity, decimalPlaces-1);
//         }
//     }
//     return std::to_string(quantity); // Default return value if step size is invalid
// }

// std::string BinanceOrderClient::makePre(float f, int n) {
//         // Convert float to string without rounding
//                 std::stringstream ss;
//                 ss << f;
//                 std::string s = ss.str();

//         std::cout << "float = " << f << " Changed to String = " << s << std::endl;
       
    
//     size_t ePosition = s.find('e');
//     size_t EPosition = s.find('E');
//     if (ePosition != std::string::npos || EPosition != std::string::npos) {
//         std::cout << "E found\n";
//         return std::to_string(f);
//     } else {
//         size_t dotPosition = s.find('.');
//         std::string i = s.substr(0, dotPosition);
//         std::string d = s.substr(dotPosition + 1);
//         std::cout << "Decimal part: " << d << std::endl;
//         std::cout << "Truncating to " << n << " decimal places\n";
//         std::string strValue = i + "." + d.substr(0, n);

       
//         return strValue;
//     }
// }


// LOTSIZE BinanceOrderClient::getLotSize(const std::string& symbol) {

//     redisContext* redis_client = RedisConnection::get_client();
//     std::string max_quantity = RedisHelper::get(symbol+":"+"maxQty", redis_client);
//     std::string min_quantity = RedisHelper::get(symbol+":"+"minQty", redis_client);
//     std::string stepSize = RedisHelper::get(symbol+":"+"stepSize", redis_client);
//     LOTSIZE empty;
//     empty.symbol = symbol;
//     empty.max_quantity = max_quantity;
//     empty.stepSize = stepSize;
//     std::cout << " max = " << empty.max_quantity << " min " << empty.max_quantity<<std::endl;
//     return empty;
// }
// #endif // BINANCE_ORDER_CLIENT_H
#ifndef BINANCE_ORDER_CLIENT_H
#define BINANCE_ORDER_CLIENT_H

#include <iostream>
#include <string>
#include <map>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <thread>
#include <openssl/hmac.h>
#include <curl/curl.h>

#include <vector>
#include "../includes/redis.h"
#include <memory>
#include "../includes/Constant.h"
#include "../includes/HTTPClient.h"
using namespace constant_nameSpace;

class BinanceOrderClient {
public:
    static std::string placeOrder(const std::string& symbol,
                                   const std::string& side, const std::string& orderType, const std::string& quantity,
                                   const std::string& price);
    static std::string getListenKey(const std::string& apiKey);  
    static std::string LOT_SIZE_CALCULATOR(float quantity, const std::string& step_size_filter);
    static std::string makePre(float f, int n); 
    static LOTSIZE getLotSize(const std::string& symbol);  // changed to const reference
private:
    static const std::string BINANCE_API_BASE_URL;
    static const std::string END_POINT;

    static std::string createParamsForOrder(const std::string& symbol, const std::string& side,
                                            const std::string& orderType, const std::string& quantity,
                                            const std::string& price, const std::string& apiKey);
    
    static std::string hmacSha256(const std::string& key, const std::string& data);
};

const std::string BinanceOrderClient::BINANCE_API_BASE_URL = "https://api.binance.com/api/v3/";
const std::string BinanceOrderClient::END_POINT = "order";

std::string BinanceOrderClient::placeOrder(const std::string& symbol, const std::string& side,
                                           const std::string& orderType, const std::string& quantity,
                                           const std::string& price) {
    try {
        // Request parameters
        LOTSIZE lot = BinanceOrderClient::getLotSize(symbol);
        std::cout <<lot.stepSize <<std::endl;
        std::cout <<std::stof(quantity) <<std::endl;
        // 2. Calculate quantity properly
        std::string final_quantity = BinanceOrderClient::LOT_SIZE_CALCULATOR(std::stof(quantity), lot.min_quantity);

        std::string apiKey = "PuR4xDBcUuxQ0kD9ZU5k8KG1mmbkVVuBmi8NLTOWTSAXa1F2N8g772bkBuzCHNux";
        std::string secretKey = "3TmSTkfyNO5ixwsFb0S4olLxt9FYdK7wX5DHHiA7JAwpJUlcUY4okwrvqiyHamXt";
        std::string _BINANCE_API_BASE_URL  = "https://testnet.binance.vision/api/v3/";
        std::string data = createParamsForOrder(symbol, side, orderType, final_quantity, price, apiKey);
        
        // Create the signature
        std::string signature = hmacSha256(secretKey, data);
        
        // Construct the URL
        std::string url = _BINANCE_API_BASE_URL + END_POINT + "?" + data + "&signature=" + signature;
        
        // Send the request
        std::vector<std::string> headers = {"X-MBX-APIKEY: " + apiKey};
        std::string response = HTTPClient::sendPOSTRequest(url, "", headers);
        
        return response;
    } catch (const std::exception& e) {
        std::cerr << "Error placing order: " << e.what() << std::endl;
        throw; // Rethrow the exception
    }
}

std::string BinanceOrderClient::getListenKey(const std::string& apiKey) {
    try {
        const std::string END_POINT = "userDataStream";
        std::string url = BINANCE_API_BASE_URL + END_POINT;
        
        // Send the request
        std::vector<std::string> headers;
        std::string api_key_header = "X-MBX-APIKEY: "+apiKey;
        headers.push_back(api_key_header);
        std::cout << "Sending Request";
        std::string response = HTTPClient::sendPOSTRequest(url, "", headers);
        
        std::cout << response << std::endl; // Logging
        
        return response;
    } catch (const std::exception& e) {
        std::cerr << "Error placing order: " << e.what() << std::endl;
        throw; // Rethrow the exception
    }
}

std::string BinanceOrderClient::createParamsForOrder(const std::string& symbol, const std::string& side,
                                                     const std::string& orderType, const std::string& quantity,
                                                     const std::string& price, const std::string& apiKey) {
    // Generate timestamp
    long long timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();
    
    // Construct request parameters
    std::stringstream ss;
    ss << "symbol=" << symbol << "&side=" << side << "&type=MARKET"
       << "&quantity=" << quantity << "&timestamp=" << timestamp;
    std::cout << ss.str() << std::endl;
    return ss.str();
}

std::string BinanceOrderClient::hmacSha256(const std::string& key, const std::string& data) {
    unsigned char digest[EVP_MAX_MD_SIZE];
    unsigned int digest_len;
    
    HMAC(EVP_sha256(), key.c_str(), key.length(),
         reinterpret_cast<const unsigned char*>(data.c_str()), data.length(), digest, &digest_len);
    
    std::stringstream ss;
    for (unsigned int i = 0; i < digest_len; ++i) {
        ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(digest[i]);
    }
    return ss.str();
}

std::string BinanceOrderClient::LOT_SIZE_CALCULATOR(float quantity, const std::string& step_size_filter) {
    // step_size_filter is something like "0.01000000" or "1.00000000"
    auto stepSize = step_size_filter;
    std::cout << "Step size filter: " << stepSize << std::endl;
    std::cout << "Raw quantity: " << quantity << std::endl;

    // Find decimal point
    auto dotPos = stepSize.find('.');
    if (dotPos == std::string::npos || stepSize == "1" || stepSize == "1.0") {
        // No decimals allowed
        return makePre(quantity, 0);
    }

    // Get the string after the dot, then trim trailing zeros
    std::string decimals = stepSize.substr(dotPos + 1);
    while (!decimals.empty() && decimals.back() == '0') {
        decimals.pop_back();
    }
    size_t precision = decimals.size();  
    std::cout << "Significant decimal places: " << precision << std::endl;

    return makePre(quantity, precision);
}

std::string BinanceOrderClient::makePre(float f, int precision) {
    // Use stringstream to get full non-scientific representation
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(precision) << f;
    auto s = oss.str();

    // Remove trailing '.' if precision == 0
    if (precision == 0) {
        auto dot = s.find('.');
        if (dot != std::string::npos) {
            s = s.substr(0, dot);
        }
    }

    std::cout << "Final formatted quantity: " << s << std::endl;
    return s;
}

LOTSIZE BinanceOrderClient::getLotSize(const std::string& symbol) {
    redisContext* redis_client = RedisConnection::get_client();
    std::string max_quantity = RedisHelper::get(symbol + ":" + "maxQty", redis_client);
    std::string min_quantity = RedisHelper::get(symbol + ":" + "minQty", redis_client);
    std::string stepSize = RedisHelper::get(symbol + ":" + "stepSize", redis_client);
    
    LOTSIZE empty;
    empty.symbol = symbol;
    empty.max_quantity = max_quantity;
    empty.min_quantity = min_quantity;
    empty.stepSize = stepSize;
    std::cout << " max = " << empty.max_quantity << " min = " << empty.min_quantity <<"stepSize : " << empty.stepSize<< std::endl;
    
    return empty;
}

#endif // BINANCE_ORDER_CLIENT_H
