import talib
import pandas as pd

class Indicator:

    @staticmethod
    def calculate_sma(df: pd.DataFrame, window: int) -> pd.Series:
        """
        Calculate Simple Moving Average (SMA).
        """
        return talib.SMA(df['close'], timeperiod=window)
    @staticmethod
    def get_current_sma_(df: pd.DataFrame, window: int) -> float:
        """
        Calculate the latest SMA value (most recent).
        """
        sma_series = talib.SMA(df['close'], timeperiod=window)
        # print(sma_series)
        return sma_series.iloc[-1]

    @staticmethod
    def get_current_sma(df: pd.DataFrame, window: int) -> tuple[float, float]:
        """
        Calculate and return the latest and second-latest SMA values.
        """
        sma_series = talib.SMA(df['close'], timeperiod=window)
        last_high = df['high'].iloc[-1]
        # print(sma_series)
        return sma_series.iloc[-1], sma_series.iloc[-2] , last_high

    @staticmethod
    def calculate_ema(df: pd.DataFrame, window: int) -> pd.Series:
        """
        Calculate Exponential Moving Average (EMA).
        """
        return talib.EMA(df['close'], timeperiod=window)

    @staticmethod
    def calculate_rsi(df: pd.DataFrame, window: int = 14) -> pd.Series:
        """
        Calculate Relative Strength Index (RSI).
        """
        return talib.RSI(df['close'], timeperiod=window)

    @staticmethod
    def basic_crossover_strategy(df: pd.DataFrame, short_window: int, long_window: int) -> pd.DataFrame:
        """
        Example strategy: SMA crossover.
        """
        df['short_sma'] = talib.SMA(df['close'], timeperiod=short_window)
        df['long_sma'] = talib.SMA(df['close'], timeperiod=long_window)
        df['signal'] = 0
        df.loc[df['short_sma'] > df['long_sma'], 'signal'] = 1
        df.loc[df['short_sma'] <= df['long_sma'], 'signal'] = -1
        return df
    
    #curl http://localhost:5000/api/v1/sma/binance/all-symbols