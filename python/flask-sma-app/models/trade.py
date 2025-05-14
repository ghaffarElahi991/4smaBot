from models.db import db
from datetime import datetime

class Trade(db.Model):
    __tablename__ = 'trades'

    id = db.Column(db.Integer, primary_key=True)
    instrument_name = db.Column(db.String(100), nullable=False)
    reason = db.Column(db.String(100), nullable=True)
    exchange = db.Column(db.String(100), nullable=False)
    entry_price = db.Column(db.Float, nullable=False)
    exit_price = db.Column(db.Float, nullable=True)
    quantity = db.Column(db.Float, nullable=False)
    pnl = db.Column(db.Float, nullable=True)
    status = db.Column(db.String(50), nullable=False, default="OPEN")
    created_at = db.Column(db.DateTime, default=datetime.utcnow)
    updated_at = db.Column(db.DateTime, default=datetime.utcnow, onupdate=datetime.utcnow)

    def __repr__(self):
        return f'<Trade {self.instrument_name} - {self.exchange} - {self.status}>'
