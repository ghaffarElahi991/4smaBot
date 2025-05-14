import os
from dotenv import load_dotenv
from urllib.parse import quote_plus

password = "W9x@7z3#pQ1"
encoded_password = quote_plus(password)


load_dotenv()

class Config:
    SECRET_KEY = os.getenv('SECRET_KEY', 'dev-key')
    # SQLALCHEMY_DATABASE_URI  = "mysql+mysqlconnector://ghaffar:W9x@7z3#pQ1@localhost:3306/CryptoBot"
    SQLALCHEMY_DATABASE_URI= f"mysql+mysqlconnector://ghaffar:{encoded_password}@localhost:3306/CryptoBot"
    SQLALCHEMY_TRACK_MODIFICATIONS = False