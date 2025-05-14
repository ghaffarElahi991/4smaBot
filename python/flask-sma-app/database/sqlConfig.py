# # config.py

# import os

# class Config:
#     DATABASE_URL = os.getenv('DATABASE_URL', 'mysql+mysqlconnector://newuser:newpassword@localhost:3306/TradingSystem')
# app/database/connection.py

from sqlalchemy import create_engine
from sqlalchemy.orm import sessionmaker
# from app.database.redisClient import RedisClient


DATABASE_URL = "mysql+mysqlconnector://ghaffar:W9x@7z3#pQ1@localhost:3306/CryptoBot"

engine = create_engine(DATABASE_URL)
SessionLocal = sessionmaker(autocommit=False, autoflush=False, bind=engine)

def get_session():
    db = SessionLocal()
    try:
        yield db
    finally:
        db.close()


