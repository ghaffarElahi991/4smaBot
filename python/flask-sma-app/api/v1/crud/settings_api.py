# settings_api.py

from flask import Blueprint, request, jsonify
from database.redisClient import RedisClient

settings_api = Blueprint('settings_api', __name__)

# Define the expected settings keys
SETTINGS_KEYS = [
    "profit", 
    "stoploss", 
    "investment_amount", 
    "max_open_trades", 
    "leverage"
]

# GET /api/v1/settings - Fetch all settings
@settings_api.route('/', methods=['GET'])
def get_settings():
    try:
        settings = {}

        for key in SETTINGS_KEYS:
            value = RedisClient.get_value(key)
            settings[key] = value if value is not None else ""

        return jsonify(settings), 200
    except Exception as e:
        return jsonify({"error": str(e)}), 500

# POST /api/v1/settings/update - Update one setting
@settings_api.route('/update', methods=['POST'])
def update_setting():
    try:
        data = request.get_json()
        key = data.get('key')
        value = data.get('value')

        if key is None or value is None:
            return jsonify({"error": "Missing 'key' or 'value' in request"}), 400

        # Save the new setting value to Redis
        RedisClient.set_value(key, value)

        return jsonify({"message": f"✅ Setting '{key}' updated to '{value}'"}), 200

    except Exception as e:
        return jsonify({"error": str(e)}), 500
