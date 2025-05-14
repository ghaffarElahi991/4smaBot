// constants.h
#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <string>
namespace constant_nameSpace{

struct LOTSIZE {
    std::string symbol;
    std::string max_quantity;
    std::string min_quantity;
    std::string stepSize;
  };
}
class Constants {


public:

    static const std::string BINANCE_API_KEY;
    static const std::string BINANCE_API_SECRET;
    static const std::string BINANACE_API_BASE_URL;

    static const std::string OKX_API_KEY;
    static const std::string OKX_API_SECRET; 
    static const std::string OKX_API_PASS;
    static const std::string OKX_API_BASE_URL;
    static const std::string DB_HOST;
    
    static const std::string DB_PORT;
    static const std::string DATABASE;
    static const std::string DB_USER;
    static const std::string DB_PASSWORD;
    static const std::vector<constant_nameSpace::LOTSIZE> LOTSIZES;
      static const std::string   profit_REDIS_KEY;
      static const std::string   stoploss_REDIS_KEY;
      static const std::string   investment_amount_REDIS_KEY;
      static const std::string   max_open_trades_REDIS_KEY;
      static const std::string   leverage_REDIS_KEY;
  
};

// Define the static member variables outside of the class definition
// API Key: PuR4xDBcUuxQ0kD9ZU5k8KG1mmbkVVuBmi8NLTOWTSAXa1F2N8g772bkBuzCHNux

// Secret Key: 3TmSTkfyNO5ixwsFb0S4olLxt9FYdK7wX5DHHiA7JAwpJUlcUY4okwrvqiyHamXt

const std::string Constants::BINANCE_API_KEY = "PuR4xDBcUuxQ0kD9ZU5k8KG1mmbkVVuBmi8NLTOWTSAXa1F2N8g772bkBuzCHNux";
const std::string Constants::BINANCE_API_SECRET = "3TmSTkfyNO5ixwsFb0S4olLxt9FYdK7wX5DHHiA7JAwpJUlcUY4okwrvqiyHamXt";
const std::string Constants::BINANACE_API_BASE_URL = "https://testnet.binance.vision/api";



// const std::string Constants::BINANCE_API_KEY = "SyKaeih4VGHqgyjRZb8DpGIVF2c9jZJaLloemDKr190DfWh0rLkSNOIeqofcQNVx";
// const std::string Constants::BINANCE_API_SECRET = "g2wThdEIqEOwiapaOuLZVBZmnxK9eFo8yxMBxGNAUWBLUfN9msw7CO3JowsgLAio";
// const std::string Constants::BINANACE_API_BASE_URL = "https://api.binance.com/api/v3/";
const std::string Constants::profit_REDIS_KEY = "profit";
const std::string Constants::stoploss_REDIS_KEY = "stoploss";
const std::string Constants::investment_amount_REDIS_KEY = "investment_amount";
const std::string Constants::max_open_trades_REDIS_KEY = "max_open_trades";
const std::string Constants::leverage_REDIS_KEY = "leverage";


const std::string Constants::OKX_API_KEY = "________15c9fa88-d1f5-4145-a2f0-0af0fea3a265!";
const std::string Constants::OKX_API_SECRET = "_________37D1478CB650145F080297EB19E4AFFD!";
const std::string Constants::OKX_API_PASS = "PAY.yors6vead8wray!";

const std::string Constants::OKX_API_BASE_URL = "https://api.binance.com/api/v3/";

// APIKEY = '15c9fa88-d1f5-4145-a2f0-0af0fea3a265'
// APISECRET = '37D1478CB650145F080297EB19E4AFFD'
// PASS = 'PAY.yors6vead8wray'

const std::string Constants::DB_HOST = "0.0.0.0";
const std::string Constants::DB_PORT = "5432";
const std::string Constants::DATABASE = "new_coindb_ghaffar";
const std::string Constants::DB_USER = "ghaffar_user";
const std::string Constants::DB_PASSWORD = "ghaffar3#24$24";

const std::vector<constant_nameSpace::LOTSIZE> Constants::LOTSIZES = {
  {"BTCUSDT", "1000000.00000000", "0.00001000"},
  {"ETHUSDT", "1000000.00000000", "0.01000000"},
  
};



#endif // CONSTANTS_H
