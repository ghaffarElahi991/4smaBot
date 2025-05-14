# app/database/create_table.py

from sqlalchemy import create_engine
from app.models.order import Base
from sqlalchemy.orm import sessionmaker
from app.database.sqlConfig import Config
DATABASE_URL = Config.DATABASE_URL

# Create an engine that stores data in the MySQL database
engine = create_engine(DATABASE_URL)

# Create all tables (including 'orders' table) in the specified database
Base.metadata.create_all(engine)

print("Table created successfully!")
