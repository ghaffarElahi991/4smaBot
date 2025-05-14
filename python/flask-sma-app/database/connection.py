# app/database/connection.py

from sqlalchemy import create_engine
from sqlalchemy.orm import sessionmaker
from app.database.sqlConfig import DATABASE_URL

# Create the engine and session factory
engine = create_engine(DATABASE_URL)
Session = sessionmaker(bind=engine)

def get_session():
    """Returns a session to interact with the database."""
    return Session()

def create_all_tables():
    """Creates tables in the database if they don't already exist."""
    from app.models import order
    order.Base.metadata.create_all(engine)
