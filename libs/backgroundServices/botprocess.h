// BotProcess.h
#ifndef BOT_PROCESS_H
#define BOT_PROCESS_H

#include <string>
#include <iostream>
#include "../binance.h"
#include <nlohmann/json.hpp>
#include "../../includes/HTTPClient.h"
#include "../../includes/redis.h"
#include "../../includes/Constant.h"

using json = nlohmann::json;
class BotProcess {
public:
static void process(const std::string& symbol, float price, redisContext* redis_client) {
    std::cout << "[process] Starting process for symbol: " << symbol << " at price: " << price << std::endl;

    // Step 1: Retrieve settings from Redis
    std::string amount_str = RedisHelper::get(Constants::investment_amount_REDIS_KEY, redis_client);
    std::string profit_str = RedisHelper::get(Constants::profit_REDIS_KEY, redis_client);
    std::string stoploss_str = RedisHelper::get(Constants::stoploss_REDIS_KEY, redis_client);

    std::cout << "[process] Retrieved from Redis: " << std::endl;
    std::cout << "   Amount String: " << amount_str << std::endl;
    std::cout << "   Profit % String: " << profit_str << std::endl;
    std::cout << "   Stoploss % String: " << stoploss_str << std::endl;

    if (!amount_str.empty() && !profit_str.empty() && !stoploss_str.empty()) {
        try {
            // Step 2: Convert strings to floats
            float amount = std::stof(amount_str);
            float profit_percentage = std::stof(profit_str);
            float stoploss_percentage = std::stof(stoploss_str);

            std::cout << "[process] Parsed values: " << std::endl;
            std::cout << "   Amount (float): " << amount << std::endl;
            std::cout << "   Profit % (float): " << profit_percentage << std::endl;
            std::cout << "   Stoploss % (float): " << stoploss_percentage << std::endl;

            if (amount > 0.0f && price > 0.0f) {
                // Step 3: Calculate order quantity
                float order_quantity = amount / price;
                std::cout << "[process] Calculated order quantity: " << order_quantity << std::endl;

                // Step 4: Place a market BUY order
                std::cout << "[process] Placing BUY market order..." << std::endl;
                std::string res = BinanceOrderClient::placeOrder(
                    symbol, "BUY", "MARKET", 
                    std::to_string(order_quantity), "0.0"
                );

                std::cout << "[process] Order response: " << res << std::endl;

                // Step 5: Calculate targets
                float profit_target = price * (1.0f + profit_percentage / 100.0f);
                float stoploss_target = price * (1.0f - stoploss_percentage / 100.0f);

                std::cout << "[process] Calculated targets: " << std::endl;
                std::cout << "   Profit Target Price: " << profit_target << std::endl;
                std::cout << "   Stoploss Target Price: " << stoploss_target << std::endl;

                // Step 6: Save targets to Redis
                RedisHelper::set(symbol + ":profitTarget", std::to_string(profit_target), redis_client);
                RedisHelper::set(symbol + ":stoplossTarget", std::to_string(stoploss_target), redis_client);
                // RedisClient.set_value(f"{instrument_name}:isopen", "1")
                RedisHelper::set(symbol + ":isopen", "1", redis_client);
                std::cout << "[process] Saved profitTarget and stoplossTarget to Redis." << std::endl;

                // Step 7: Optionally save buy quantity
                // RedisHelper::set(symbol + ":buyQty", std::to_string(order_quantity), redis_client);

                // Step 8: Process the order into database
                process_DB(res, "binance");
                std::cout << "[process] Processed order into database." << std::endl;
            } else {
                std::cerr << "[process] Invalid amount or price. Amount: " << amount << ", Price: " << price << std::endl;
            }
        } catch (const std::exception& e) {
            std::cerr << "[process] Exception while parsing or processing: " << e.what() << std::endl;
        }
    } else {
        std::cerr << "[process] Error: One or more Redis values are missing (amount, profit, or stoploss)." << std::endl;
    }

    std::cout << "[process] Process ended for symbol: " << symbol << std::endl;
}


    static void process_EXIT(const std::string& symbol, const std::string& reason, float price, redisContext* redis_client) {
        // Fetch sell quantity for the symbol from Redis
        std::string amount_str = RedisHelper::get(symbol + ":sellQty", redis_client);
        std::cout << "\nSell Ammount "<<amount_str<<std::endl;
        if (!amount_str.empty()) {
            try {
                // Optionally, you could validate that amount_str is a valid number
                float sell_qty = std::stof(amount_str);

                if (sell_qty > 0.0f) {
                    // Place a market SELL order
                    std::string res = BinanceOrderClient::placeOrder(symbol, "SELL", "MARKET", 
                                                                    amount_str, "0.0");

                    // Print the response
                    std::cout << res << std::endl;

                    // Process and log the exit in the database
                    process_DB_exit(res, "binance", reason);
                } else {
                    std::cerr << "Invalid sell quantity (<=0) for symbol: " << symbol << std::endl;
                }
            } catch (const std::exception& e) {
                std::cerr << "Error converting sell quantity to float for symbol: " << symbol 
                        << ", Error: " << e.what() << std::endl;
            }
        } else {
            std::cerr << "Sell quantity not found for symbol: " << symbol << std::endl;
        }
    }

    static void process_DB_exit(std::string& response ,std::string exchange,std::string reason) {
        // std::cout << "BotProcess [" << price << "] started." << std::endl;

        json subscribe_message = {
            {"exchange", exchange},
            {"data", response},
            {"reason", reason}
        };
        // <symbol>@miniTicker
        std::string payload = subscribe_message.dump();
        std::cout << payload;
        std::string url = "http://127.0.0.1:5000/api/v1/orders";
        std::string respon = HTTPClient::sendPOSTRequest(url, payload , {"Content-Type: application/json"});
        std::cout <<" API RESPONSE : \n";
        std::cout << respon;
    }
    static void process_DB(std::string& response ,std::string exchange) {
        // std::cout << "BotProcess [" << price << "] started." << std::endl;

        json subscribe_message = {
            {"exchange", exchange},
            {"data", response}
        };
        // <symbol>@miniTicker
        std::string payload = subscribe_message.dump();
        std::cout << payload;
        std::string url = "http://127.0.0.1:5000/api/v1/orders";
        std::string respon = HTTPClient::sendPOSTRequest(url, payload , {"Content-Type: application/json"});
        std::cout <<" API RESPONSE : \n";
        std::cout << respon;
    }
    

private:
    BotProcess() = delete;  // Prevent creating instances
};

#endif // BOT_PROCESS_H
