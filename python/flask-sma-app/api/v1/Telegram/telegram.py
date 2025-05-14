# routes/trade_routes.py
import requests
from flask import Blueprint, request, jsonify
from datetime import datetime

# Telegram bot configuration
TELEGRAM_BOT_TOKEN = 'your_bot_token_here'
TELEGRAM_CHAT_ID = 'your_chat_id_here'  # Can be a user ID or group ID

trade_bp = Blueprint('trade_bp', __name__, url_prefix='/trade')

class TelegramMessages:
    BOT_TOKEN = TELEGRAM_BOT_TOKEN
    CHAT_ID = TELEGRAM_CHAT_ID

    @staticmethod
    def send_message(message):
        url = f"https://api.telegram.org/bot{TelegramMessages.BOT_TOKEN}/sendMessage"
        payload = {
            'chat_id': TelegramMessages.CHAT_ID,
            'text': message,
            'parse_mode': 'Markdown'
        }
        response = requests.post(url, json=payload)
        return response

    @staticmethod
    def create_trade_message(instrument_name, exchange, entry_price, quantity, status="OPEN", exit_price="N/A", pnl="N/A"):
        created_at = datetime.utcnow().strftime('%Y-%m-%d %H:%M:%S')
        message = (
            f"*New Trade Update*\n\n"
            f"*Instrument:* {instrument_name}\n"
            f"*Exchange:* {exchange}\n"
            f"*Entry Price:* {entry_price}\n"
            f"*Quantity:* {quantity}\n"
            f"*Status:* {status}\n"
            f"*Exit Price:* {exit_price}\n"
            f"*PnL:* {pnl}\n"
            f"*Time:* {created_at}"
        )
        return message

@trade_bp.route('/send', methods=['POST'])
def send_trade():
    data = request.get_json()

    required_fields = ['instrument_name', 'exchange', 'entry_price', 'quantity']
    for field in required_fields:
        if field not in data:
            return jsonify({"error": f"Missing field: {field}"}), 400

    instrument_name = data['instrument_name']
    exchange = data['exchange']
    entry_price = data['entry_price']
    quantity = data['quantity']
    status = data.get('status', 'OPEN')
    exit_price = data.get('exit_price', 'N/A')
    pnl = data.get('pnl', 'N/A')

    # Create message
    message = TelegramMessages.create_trade_message(
        instrument_name, exchange, entry_price, quantity, status, exit_price, pnl
    )

    # Send to Telegram
    response = TelegramMessages.send_message(message)

    if response.status_code == 200:
        return jsonify({"message": "Trade sent to Telegram!"}), 200
    else:
        return jsonify({"error": "Failed to send message to Telegram", "details": response.text}), 500
