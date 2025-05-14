from exchanges.binance import BinanceClient
from api.services.formator import Formator
from api.services.indicators import Indicator
from database.redisClient import RedisClient
import time

def fetch_and_store_sma():
    try:
        symbols = BinanceClient.fetch_all_symbols("USDT")
        print(f"Total symbols fetched: {len(symbols)}")

        for symbol in symbols:
            try:
                key = f"binance:sma:current:{symbol}"
                keylast = f"binance:sma:last:{symbol}"

                keylastHigh = f"binance:high:last:{symbol}"

                # Fetch the last 55 candles for the 4-hour timeframe
                data = BinanceClient.get_klines(symbol, '4h', 55)
                if not data or len(data) < 50:
                    print(f"Not enough data for {symbol}, skipping...")
                    continue

                # Format the data into a DataFrame
                formatted_data = Formator.Binance_klines_to_dataframe(data)

                # Get the current and previous SMA
                last_sma, second_last_sma , high_value= Indicator.get_current_sma(formatted_data, 50)

                # Store SMA values in Redis
                RedisClient.set_value(key, float(last_sma))
                RedisClient.set_value(keylast, float(second_last_sma))
                RedisClient.set_value(keylastHigh, float(high_value))

                print(f"Stored SMA for {symbol}: current={last_sma}, last={second_last_sma} , high {high_value} {type(high_value)}")

                time.sleep(0.5)  # Sleep to avoid API rate limits for each symbol

            except Exception as inner_e:
                print(f"Error fetching or processing data for {symbol}: {str(inner_e)}")
    except Exception as e:
        print(f"Critical error: {str(e)}")

if __name__ == "__main__":
    while True:
        fetch_and_store_sma()  # Run the function
        print("Waiting for the next cycle...")
        time.sleep(1)  # Wait for 1 second before running again
