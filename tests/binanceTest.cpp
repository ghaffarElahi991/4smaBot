
#include "../libs/binance.h"
#include "../includes/redis.h"
#include "../libs/backgroundServices/botprocess.h"
redisContext* redis_client = RedisConnection::get_client();
int main() {
    
        BotProcess::process( "PEPEUSDT" ,0.00001275 , redis_client);
        // BotProcess::process_EXIT( "BTCUSDT" ,"Take profit", 100000 , redis_client);
        // std::string res = BinanceOrderClient::placeOrder_EXIT("BTCUSDT","SELL",
        //     "MARKET",std::to_string(100.0/100000),
        //     "0.0");
        // std::cout <<res;
    
    }
