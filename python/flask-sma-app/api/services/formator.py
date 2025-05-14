import pandas as pd

class Formator:

    @staticmethod
    # def Binance_klines_to_dataframe(klines: list) -> pd.DataFrame:
    def Binance_klines_to_dataframe(klines: list) -> pd.DataFrame:
        """
        Converts a list of klines JSON into a formatted pandas DataFrame.
        """
        df = pd.DataFrame(klines, columns=[
            'open_time', 'open', 'high', 'low', 'close', 'volume',
            'close_time', 'quote_asset_volume', 'number_of_trades',
            'taker_buy_base', 'taker_buy_quote', 'ignore'
        ])
        df['open_time'] = pd.to_datetime(df['open_time'], unit='ms')
        df['close_time'] = pd.to_datetime(df['close_time'], unit='ms')
        df = df.astype({
            'open': 'float',
            'high': 'float',
            'low': 'float',
            'close': 'float',
            'volume': 'float'
        })
        return df[['open_time', 'open', 'high', 'low', 'close', 'volume']]