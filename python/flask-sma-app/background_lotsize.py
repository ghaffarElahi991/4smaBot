# import json
# from exchanges.binance import BinanceClient
# from database.redisClient import RedisClient
# # Fetch the exchange information
# exchange_info = BinanceClient.fetch_exchangeInfo()
# c = RedisClient.get_client()
# # Loop through each symbol
# for symbol_info in exchange_info.get('symbols', []):
#     symbol_name = symbol_info.get('symbol')
#     for filt in symbol_info.get('filters', []):
#         if filt.get('filterType') == 'LOT_SIZE':
#             minkey = symbol_name+":minQty"
#             maxkey = symbol_name+":maxQty"
#             stepSizeQtykey = symbol_name+":stepSize"
#             RedisClient.set_value(minkey , f"{filt.get("minQty")}")
#             RedisClient.set_value(maxkey , f"{filt.get("maxQty")}")
#             RedisClient.set_value(stepSizeQtykey , f"{filt.get("stepSize")}")
#             print(f"Symbol: {symbol_name}")
#             print(json.dumps(filt, indent=4))  # Pretty-print the LOT_SIZE filter
#             print('-' * 40)  # Separator for clarity
import json
import time
from exchanges.binance import BinanceClient
from database.redisClient import RedisClient

def fetch_and_store_lot_size():
    # Fetch the exchange information
    exchange_info = BinanceClient.fetch_exchangeInfo()
    c = RedisClient.get_client()

    # Loop through each symbol
    for symbol_info in exchange_info.get('symbols', []):
        symbol_name = symbol_info.get('symbol')
        for filt in symbol_info.get('filters', []):
            if filt.get('filterType') == 'LOT_SIZE':
                minkey = symbol_name + ":minQty"
                maxkey = symbol_name + ":maxQty"
                stepSizeQtykey = symbol_name + ":stepSize"

                RedisClient.set_value(minkey, f"{filt.get('minQty')}")
                RedisClient.set_value(maxkey, f"{filt.get('maxQty')}")
                RedisClient.set_value(stepSizeQtykey, f"{filt.get('stepSize')}")

                # print(f"Symbol: {symbol_name}")
                # print(json.dumps(filt, indent=4))  # Pretty-print the LOT_SIZE filter
                # print('-' * 40)  # Separator for clarity

if __name__ == "__main__":
    while True:
        try:
            fetch_and_store_lot_size()
        except Exception as e:
            print(f"Error occurred: {e}")
        
        print("Waiting for 30 minutes...")
        time.sleep(1800)  # 1800 seconds = 30 minutes
