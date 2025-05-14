# routes/bot_process.py
from services.crud.trades import TradeCRUD
from database.redisClient import RedisClient

import json

class BotProcess:

    @staticmethod
    def process(message):
        exchange = message.get("exchange")
        reason = message.get("reason" , "")
        if exchange == "binance":
            return BotProcess.process_binance(message.get("data") , reason)
        else:
            print(f"[Error] Exchange {exchange} not supported yet.")
            return {"error": "Exchange not supported"}

    @staticmethod
    def process_binance(message , reason):
        """
        Specific processing logic for Binance exchange.
        Handles filled BUY/SELL orders.
        """
        try:
            print(type(message))
            message = json.loads(message)
            symbol = message.get("symbol")
            side = message.get("side", "BUY").upper()
            fills = message.get("fills", [])
            
            if not fills:
                print("[Error] No fills data available.")
                return {"error": "No fills data"}

            # We assume first fill price and qty
            fill_price = float(fills[0]["price"])
            fill_qty = float(fills[0]["qty"])
            commission = float(fills[0]["commission"])
            
            
            # commission

            if side == "BUY":
                # Insert into DB as an OPEN trade
                fill_qty = fill_qty - commission
                result = TradeCRUD.create_trade(
                    instrument_name=symbol,
                    exchange="binance",
                    entry_price=fill_price,
                    quantity=fill_qty,
                    status="OPEN"
                )
                print(f"[DB] Created new trade: {result}")
                return result
            elif side == "SELL":
            # RedisClient.set_value(f"{instrument_name}:primarykey", str(new_trade.id))
                TradeId = RedisClient.get_value(f"{symbol}:primarykey" )
                # RedisClient.set_value(f"{symbol}:isopen", "1")
            
                # fill_price = float(fills[0]["price"])
                updated_trade = TradeCRUD.close_order(
                    trade_id=int(TradeId),
                    exit_price=fill_price,  # New exit price
                    reason=reason  # Reason for closing
                )
                # You can add your sell logic here, for example update the DB
                print(f"[SELL] {fill_qty} {symbol} sold at {fill_price}")
                return updated_trade
            else:
                print(f"[Error] Invalid side: {side}")
                return {"error": "Invalid side"}

        except Exception as e:
            print(f"[Exception] {e}")
            return {"error": str(e)}

    @staticmethod
    def buy_order(symbol: str, quantity: float, price: float, order_type="LIMIT"):
        """
        Process a buy order.
        """
        print(f"[BUY] {quantity} {symbol} at {price} ({order_type})")
        # Here you would actually call your broker/exchange API to place a buy order
        return {
            "action": "buy",
            "symbol": symbol,
            "quantity": quantity,
            "price": price,
            "order_type": order_type,
            "status": "submitted"
        }

    @staticmethod
    def sell_order(symbol: str, quantity: float, price: float, order_type="LIMIT"):
        """
        Process a sell order.
        """
        print(f"[SELL] {quantity} {symbol} at {price} ({order_type})")
        # Here you would actually call your broker/exchange API to place a sell order
        return {
            "action": "sell",
            "symbol": symbol,
            "quantity": quantity,
            "price": price,
            "order_type": order_type,
            "status": "submitted"
        }

    @staticmethod
    def cancel_order(order_id: str):
        """
        Cancel an existing order.
        """
        print(f"[CANCEL] Order ID: {order_id}")
        # Actual cancel order logic here
        return {
            "action": "cancel",
            "order_id": order_id,
            "status": "cancellation_requested"
        }
