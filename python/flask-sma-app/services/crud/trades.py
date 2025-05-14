from models.db import db
from models.trade import Trade
from datetime import datetime
from typing import List, Optional, Dict, Any
from database.redisClient import RedisClient
class TradeCRUD:
    """
    Static methods for all CRUD operations on Trade model
    """
    
    @staticmethod
    def create_trade(
        instrument_name: str,
        exchange: str,
        entry_price: float,
        quantity: float,
        exit_price: Optional[float] = None,
        pnl: Optional[float] = None,
        status: str = "OPEN",
        reason: str = ""
    ) -> 'Trade':
        """
        Create a new trade record and store primary key in Redis.
        """
        try:
            new_trade = Trade(
                instrument_name=instrument_name,
                exchange=exchange,
                entry_price=entry_price,
                exit_price=exit_price,
                quantity=quantity,
                pnl=pnl,
                status=status,
                reason=reason
            )
            
            db.session.add(new_trade)
            db.session.commit()
            print(f"creating new table {new_trade.id}")
            # Save the primary key (id) in Redis
            RedisClient.set_value(f"{instrument_name}:primarykey", str(new_trade.id))
            RedisClient.set_value(f"{instrument_name}:isopen", "1")
            RedisClient.set_value(f"{instrument_name}:sellQty" , str(quantity))
            return new_trade

        except Exception as e:
            db.session.rollback()
            raise e

    @staticmethod
    def get_trade_by_id(trade_id: int) -> Optional['Trade']:
        """
        Retrieve a single trade by ID
        """
        return Trade.query.get(trade_id)

    @staticmethod
    def get_all_trades(
        filters: Optional[Dict[str, Any]] = None,
        limit: Optional[int] = None
    ) -> List['Trade']:
        """
        Get all trades with optional filters and limit
        """
        query = Trade.query
        
        if filters:
            for key, value in filters.items():
                if hasattr(Trade, key):
                    query = query.filter(getattr(Trade, key) == value)
        
        if limit:
            query = query.limit(limit)
            
        return query.all()

    @staticmethod
    def update_trade(
        trade_id: int,
        update_data: Dict[str, Any]
    ) -> Optional['Trade']:
        """
        Update trade fields
        """
        trade = Trade.query.get(trade_id)
        if not trade:
            return None
            
        for key, value in update_data.items():
            if hasattr(trade, key) and key not in ['id', 'created_at']:
                setattr(trade, key, value)
                
        db.session.commit()
        return trade

    @staticmethod
    def close_order(
        trade_id: int,
        exit_price: Optional[float] = None,
        pnl: Optional[float] = None,
        reason: Optional[str] = None
    ) -> 'Trade':
        """
        Update an existing trade's status, exit_price, pnl, and reason.
        """
        # Fetch the trade by id
        trade = Trade.query.get(trade_id)

        if not trade:
            raise ValueError(f"Trade with ID {trade_id} not found")
        
        # If exit price is provided, calculate the PnL if it's not manually set
        if exit_price is not None:
            trade.exit_price = exit_price
            if pnl is None:  # If PnL is not provided, calculate it
                pnl = (exit_price - trade.entry_price) * trade.quantity

        # Update status and reason
        trade.status = "CLOSED"
        if pnl is not None:
            trade.pnl = pnl
        if reason is not None:
            trade.reason = reason

        # Set Redis flag indicating the trade is closed
        RedisClient.set_value(f"{trade.instrument_name}:isopen", "0")

        # Commit the changes to the database
        db.session.commit()

        # Optionally, print confirmation of the updated trade
        print(f"✅ Updated trade {trade.id}: Status={trade.status}, Exit Price={trade.exit_price}, PnL={trade.pnl}, Reason={trade.reason}")

        return trade



    @staticmethod
    def delete_trade(trade_id: int) -> bool:
        """
        Delete a trade record
        Returns True if deleted, False if not found
        """
        trade = Trade.query.get(trade_id)
        if not trade:
            return False
            
        db.session.delete(trade)
        db.session.commit()
        return True

    @staticmethod
    def get_open_trades() -> List['Trade']:
        """
        Get all trades with OPEN status
        """
        return Trade.query.filter_by(status="OPEN").all()

    @staticmethod
    def close_trade(
        trade_id: int,
        exit_price: float,
        pnl: Optional[float] = None
    ) -> Optional['Trade']:
        """
        Close a trade by updating status and exit price
        """
        trade = Trade.query.get(trade_id)
        if not trade:
            return None
            
        trade.status = "CLOSED"
        trade.exit_price = exit_price
        trade.pnl = pnl if pnl is not None else (exit_price - trade.entry_price) * trade.quantity
        
        db.session.commit()
        return trade

    @staticmethod
    def bulk_create_trades(trades_data: List[Dict[str, Any]]) -> List['Trade']:
        """
        Create multiple trades at once
        """
        trades = [
            Trade(
                instrument_name=data['instrument_name'],
                exchange=data['exchange'],
                entry_price=data['entry_price'],
                quantity=data['quantity'],
                exit_price=data.get('exit_price'),
                pnl=data.get('pnl'),
                status=data.get('status', "OPEN")
            )
            for data in trades_data
        ]
        
        db.session.bulk_save_objects(trades)
        db.session.commit()
        return trades