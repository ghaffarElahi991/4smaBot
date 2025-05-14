from flask import Blueprint, request, jsonify
# from api.services.sma_service import calculate_sma
from exchanges.binance import BinanceClient
from api.services.formator import Formator
from api.services.indicators import Indicator
from database.redisClient import RedisClient
import time
sma_routes = Blueprint('api', __name__)



sma_routes = Blueprint('sma_routes', __name__)

@sma_routes.route('/binance/all-symbols', methods=['GET'])
def calculate_sma_for_all_symbols():
    # window = request.args.get('window', type=int)
    # if not window:
    #     return jsonify({'error': 'Missing window parameter'}), 400

    try:
        symbols = BinanceClient.fetch_all_symbols("USDT")
        print(len(symbols))

        # data = BinanceClient.get_klines('BTCUSDT', '4h', 60)
        # print(f"data length = {len(data)}")
        # formatted_data = Formator.Binance_klines_to_dataframe(data)

        # smaValue = Indicator.get_current_sma(formatted_data , 50)
        # print(type(smaValue))
        for symbol in symbols:
            try:
                # symbol = "BTCUSDT"
                key = f"binance:sma:current:{symbol}"
                keylast = f"binance:sma:last:{symbol}"
                data = BinanceClient.get_klines(symbol, '4h', 55)
                formatted_data = Formator.Binance_klines_to_dataframe(data)
                last_sma, second_last_sma  = Indicator.get_current_sma(formatted_data , 50)
                # print(type(smaValue))
                # print(f"SMA for {symbol}: {smaValue}")
                RedisClient.set_value(key, float(last_sma))

                RedisClient.set_value(keylast, float(second_last_sma))
                time.sleep(0.5)  # To avoid hitting the API rate limit
            except Exception as inner_e:
                print(f"Error fetching data for {symbol}: {str(inner_e)}")
        # print(BinanceClient.get_klines('BTCUSDT', '4h', 50))
        # data = BinanceClient.get_klines('BTCUSDT', '4h', 50)
        # formatted_data = Formator.Binance_klines_to_dataframe(data)
        # print(formatted_data)
        # symbols = BinanceClient.fetch_all_symbols()
        # sma_results = {}
        # for symbol in symbols:
        #     try:
        #         sma_value = calculate_sma(symbol, window)
        #         sma_results[symbol] = sma_value
        #     except Exception as inner_e:
        #         sma_results[symbol] = f"Calculation error: {str(inner_e)}"

        return jsonify({})
    except Exception as e:
        return jsonify({'error': str(e)}), 500