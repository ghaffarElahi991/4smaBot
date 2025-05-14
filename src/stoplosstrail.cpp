#include <websocketpp/config/asio_client.hpp>
#include <websocketpp/client.hpp>
#include <iostream>
#include <nlohmann/json.hpp>
#include <fstream>
#include <chrono>
#include "../includes/redis.h"
#include "../libs/backgroundServices/botprocess.h"
#include "../libs/backgroundServices/indicators.h"
typedef websocketpp::client<websocketpp::config::asio_tls_client> client;
typedef websocketpp::lib::shared_ptr<websocketpp::lib::asio::ssl::context> context_ptr;
using namespace std::placeholders;
using json = nlohmann::json;
redisContext* redis_client = RedisConnection::get_client();
std::string getCurrentTimeInMillis() {
    auto now = std::chrono::system_clock::now();
    auto duration = now.time_since_epoch();
    auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
    return std::to_string(millis);
}

context_ptr on_tls_init(const char*, websocketpp::connection_hdl) {
    context_ptr ctx = websocketpp::lib::make_shared<boost::asio::ssl::context>(boost::asio::ssl::context::sslv23);
    ctx->set_options(boost::asio::ssl::context::default_workarounds |
                     boost::asio::ssl::context::no_sslv2 |
                     boost::asio::ssl::context::no_sslv3 |
                     boost::asio::ssl::context::single_dh_use);
    return ctx;
}
void parse_s_and_c(const std::string& input) {
    size_t pos = 0;
    while (pos < input.size()) {
        // Find next '{' and '}'
        size_t start = input.find('{', pos);
        if (start == std::string::npos) break;
        size_t end = input.find('}', start);
        if (end == std::string::npos) break;

        // Extract one JSON block
        std::string block = input.substr(start, end - start + 1);

        // Find "s" in block
        size_t s_pos = block.find("\"s\"");
        if (s_pos != std::string::npos) {
            size_t s_colon = block.find(':', s_pos);
            size_t s_quote_start = block.find('"', s_colon);
            size_t s_quote_end = block.find('"', s_quote_start + 1);
            std::string s_value = block.substr(s_quote_start + 1, s_quote_end - s_quote_start - 1);

            // Find "c" in block
            size_t c_pos = block.find("\"c\"");
            size_t c_colon = block.find(':', c_pos);
            size_t c_quote_start = block.find('"', c_colon);
            size_t c_quote_end = block.find('"', c_quote_start + 1);
            std::string c_value = block.substr(c_quote_start + 1, c_quote_end - c_quote_start - 1);
            
            // Find "c" in block
            // size_t h_pos = block.find("\"h\"");
            // size_t h_colon = block.find(':', h_pos);
            // size_t h_quote_start = block.find('"', h_colon);
            // size_t h_quote_end = block.find('"', h_quote_start + 1);
            // std::string h_value = block.substr(h_quote_start + 1, h_quote_end - h_quote_start - 1);
                        
            
            std::string SMA_KEY = "binance:sma:current:" + s_value;
            std::string SMA_LAST_KEY = "binance:sma:last:" + s_value;
            std::string SMA_LAST_HIGH_KEY = "binance:high:last:" + s_value;
            
            std::string SMA_VALUE = RedisHelper::get(SMA_KEY, redis_client);
            std::string SMA_VALUE_LAST = RedisHelper::get(SMA_LAST_KEY, redis_client);
            std::string symbol = s_value;
            std::string h_value = RedisHelper::get(SMA_LAST_HIGH_KEY, redis_client);
            std::string isopen = RedisHelper::get(symbol + ":isopen", redis_client);
            // std::cout << "high value : "<< h_value<<std::endl;
            if(!SMA_VALUE.empty() && !c_value.empty()  && !s_value.empty() && !h_value.empty()) {

                float sma_value = std::stof(SMA_VALUE);
                float sma_value_last = std::stof(SMA_VALUE_LAST);
                float close_value = std::stof(c_value);
                float high_value = std::stof(h_value);
                
                if(close_value > sma_value && close_value < sma_value_last)
                {
                    // if (isopen == "0")
                    // {
                    //     std::cout << s_value <<" SMA: " << sma_value << ", Close Price: " << close_value << std::endl;
                    //     BotProcess::process( s_value , close_value , redis_client);
            
                    // }
                    // std::string h_value = RedisHelper::get( "binance:high:last:"+symbol, redis_client);
                    // if(h_value.empty())
                    // {

                    // }
                 
                    float doNotTradePrice = Indicator::takeProfit(close_value, 0.5);
                    if(high_value > doNotTradePrice)
                    {
                        std::cout <<s_value<< " This is Do not Trade "<<std::endl;
                    }else
                    {
                        if (isopen == "0")
                        {
                            std::cout << s_value <<" SMA: " << sma_value << ", Close Price: " << close_value << std::endl;
                            BotProcess::process( s_value , close_value , redis_client);
                
                        }   
                    }

            
            
                }


            
            } 

            // RedisHelper::set(symbol + ":profitTarget", std::to_string(profit_target), redis_client);
            // RedisHelper::set(symbol + ":stoplossTarget", std::to_string(stoploss_target), redis_client);
            // // RedisClient.set_value(f"{instrument_name}:isopen", "1")
            // RedisHelper::set(symbol + ":isopen", "1", redis_client);
    // Fetch if trade is open

            // std::cout << "[Check Targets] " << symbol << " isopen: " << isopen << std::endl;

            if (isopen == "1") {
                float close_value = std::stof(c_value);
                
                // Trade is open, fetch profit and stoploss targets
                std::string profitStr = RedisHelper::get(symbol + ":profitTarget", redis_client);
                std::string stoplossStr = RedisHelper::get(symbol + ":stoplossTarget", redis_client);

                std::cout << "[Check Targets] profitTarget: " << profitStr << ", stoplossTarget: " << stoplossStr << std::endl;

                if (!profitStr.empty() && !stoplossStr.empty()) {
                    try {
                        float profit_target = std::stof(profitStr);
                        float stoploss_target = std::stof(stoplossStr);

                        std::cout << "[Check Targets] Current Price: " << close_value
                                << ", Profit Target: " << profit_target
                                << ", Stoploss Target: " << stoploss_target << std::endl;

                        if (close_value >= profit_target) {
                            std::cout << "[EXIT] Profit target hit for " << symbol << "! Exiting trade." << std::endl;
                            BotProcess::process_EXIT(symbol, "Profit Target Hit", close_value, redis_client);

                            // Mark as closed in Redis
                            RedisHelper::set(symbol + ":isopen", "0", redis_client);
                        }
                        else if (close_value <= stoploss_target) {
                            std::cout << "[EXIT] Stoploss target hit for " << symbol << "! Exiting trade." << std::endl;
                            BotProcess::process_EXIT(symbol, "Stoploss Hit", close_value, redis_client);

                            // Mark as closed in Redis
                            RedisHelper::set(symbol + ":isopen", "0", redis_client);
                        }
                        else {
                            // std::cout << "[Check Targets] No exit condition met yet for " << symbol << "." << std::endl;
                        }
                    } catch (const std::exception& e) {
                        std::cerr << "[Check Targets] Error parsing targets: " << e.what() << std::endl;
                    }
                } else {
                    std::cerr << "[Check Targets] Profit or Stoploss target missing for " << symbol << "." << std::endl;
                }
            } else {
                // std::cout << "[Check Targets] Trade is not open for " << symbol << ". Skipping." << std::endl;
            }
            // std::cout << "Symbol: " << s_value << ", Close Price: " << c_value << std::endl;
        
        
        }

        pos = end + 1; // move to next block
    }
}

void on_message(websocketpp::connection_hdl, client::message_ptr msg) {
    auto start = std::chrono::high_resolution_clock::now(); // Start time

    // std::cout << "Received: " << msg->get_payload() << std::endl;
    parse_s_and_c(msg->get_payload());

    auto end = std::chrono::high_resolution_clock::now(); // End time
    std::chrono::duration<double, std::milli> elapsed = end - start; // Time in milliseconds

    // std::cout << "on_message executed in " << elapsed.count() << " ms" << std::endl;
}

void on_open(websocketpp::connection_hdl hdl, client* c) {
    json subscribe_message = {
        {"method", "SUBSCRIBE"},
        {"params", {"!miniTicker@arr"}},
        {"id", 1}
    };
    // <symbol>@miniTicker
    std::string payload = subscribe_message.dump();
    websocketpp::lib::error_code ec;
    client::connection_ptr con = c->get_con_from_hdl(hdl, ec);
    if (!ec) c->send(con, payload, websocketpp::frame::opcode::text);
}

int main() {
    client c;
    std::string uri = "wss://stream.binance.com:9443/ws";
    c.clear_access_channels(websocketpp::log::alevel::all);
    c.clear_error_channels(websocketpp::log::elevel::all);
    c.init_asio();
    c.set_tls_init_handler(bind(&on_tls_init, "", ::_1));
    c.set_open_handler(bind(&on_open, ::_1, &c));
    c.set_message_handler(&on_message);

    websocketpp::lib::error_code ec;
    client::connection_ptr con = c.get_connection(uri, ec);

    if (ec) {
        std::cout << "could not create connection because: " << ec.message() << std::endl;
        return 0;
    }

    c.connect(con);
    c.run();
}
