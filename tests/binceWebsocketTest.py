import json
import websocket

def on_message(ws, message):
    data = json.loads(message)
    print(data)
    # kline = data['k']  # 'k' is the kline data
    # print(f"Time: {kline['t']}, Open: {kline['o']}, High: {kline['h']}, Low: {kline['l']}, Close: {kline['c']}, Volume: {kline['v']}")

def on_error(ws, error):
    print(f"Error: {error}")

def on_close(ws, close_status_code, close_msg):
    print("WebSocket closed")

def on_open(ws):
    # Subscribe to the stream
    params = {
        "method": "SUBSCRIBE",
        "params": [
            "!ticker@arr"  # symbol@kline_interval (e.g., 1m = 1 minute)
        ],
        "id": 1
    }
    ws.send(json.dumps(params))

# Binance WebSocket endpoint
socket = "wss://stream.binance.com:9443/ws"

# Create WebSocket
ws = websocket.WebSocketApp(socket, on_open=on_open, on_message=on_message, on_error=on_error, on_close=on_close)

# Run forever
ws.run_forever()
