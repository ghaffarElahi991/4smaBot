import requests

class BinanceClient:
    BASE_URL = 'https://api.binance.com'
    
    @staticmethod
    def fetch_all_symbols(filter=None):
        url = f"{BinanceClient.BASE_URL}/api/v3/exchangeInfo"
        resp = requests.get(url)
        resp.raise_for_status()
        data = resp.json()
        if filter is None:
            return [s['symbol'] for s in data.get('symbols', [])]
        else:
          return [s['symbol'] for s in data.get('symbols', []) if s['symbol'].endswith(filter)]

    @staticmethod
    def fetch_exchangeInfo(filter=None):
        url = f"{BinanceClient.BASE_URL}/api/v3/exchangeInfo"
        resp = requests.get(url)
        resp.raise_for_status()
        return resp.json()


    @staticmethod
    def get_klines(symbol: str, interval: str = '1h', limit: int = 500):
        """
        Fetch Kline/candlestick data.
        :param symbol: Trading pair, e.g., 'BTCUSDT'
        :param interval: Interval string, e.g., '1m', '3m', '1h', '1d'
        :param limit: Number of data points
        """
        url = f"{BinanceClient.BASE_URL}/api/v3/klines"
        params = {
            'symbol': symbol,
            'interval': interval,
            'limit': limit
        }
        resp = requests.get(url, params=params)
        resp.raise_for_status()
        return resp.json()

    @staticmethod
    def get_order_book(symbol: str, limit: int = 100):
        """
        Fetch current order book (bids and asks).
        :param symbol: Trading pair
        :param limit: Depth limit (max 1000)
        """
        url = f"{BinanceClient.BASE_URL}/api/v3/depth"
        params = {
            'symbol': symbol,
            'limit': limit
        }
        resp = requests.get(url, params=params)
        resp.raise_for_status()
        return resp.json()

    @staticmethod
    def get_ticker_price(symbol: str):
        """Get latest price for a symbol."""
        url = f"{BinanceClient.BASE_URL}/api/v3/ticker/price"
        resp = requests.get(url, params={'symbol': symbol})
        resp.raise_for_status()
        return resp.json()

    @staticmethod
    def get_account_info(api_key: str, api_secret: str):
        """
        Placeholder for authenticated endpoint: fetch account info.
        Requires HMAC signing.
        """
        raise NotImplementedError("Authentication flows to be implemented")