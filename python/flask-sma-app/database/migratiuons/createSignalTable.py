# app/database/create_table.py
import sys
import os



# Add the 'app' directory to the Python path
sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), '../../..')))

from app.models.signal import Base  # Adjust this based on the correct import path


# from models.signal import Base  # Adjust this based on the correct import path

from sqlalchemy import create_engine
from app.models.signal import Base
from sqlalchemy.orm import sessionmaker
from app.database.sqlConfig import DATABASE_URL 
# DATABASE_URL = Config.DATABASE_URL

# Create an engine that stores data in the MySQL database
engine = create_engine(DATABASE_URL)

# Create all tables (including 'orders' table) in the specified database
Base.metadata.create_all(engine)

print("Table created successfully!")
