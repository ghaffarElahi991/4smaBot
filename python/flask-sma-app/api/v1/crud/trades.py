from flask import Blueprint, request, jsonify
from services.crud.trades import TradeCRUD
from typing import Dict, Any
from api.v1.botServices.BotProcess import BotProcess
trade_bp = Blueprint('trades', __name__, url_prefix='/api/trades')

# @trade_bp.route('/', methods=['POST'])
# def create_trade():
#     """Create a new trade"""
#     data: Dict[str, Any] = request.get_json()
    
#     required_fields = ['instrument_name', 'exchange', 'entry_price', 'quantity']
#     if not all(field in data for field in required_fields):
#         return jsonify({'error': 'Missing required fields'}), 400
    
#     try:
#         trade = TradeCRUD.create_trade(
#             instrument_name=data['instrument_name'],
#             exchange=data['exchange'],
#             entry_price=float(data['entry_price']),
#             quantity=float(data['quantity']),
#             exit_price=float(data.get('exit_price')) if data.get('exit_price') else None,
#             pnl=float(data.get('pnl')) if data.get('pnl') else None,
#             status=data.get('status', 'OPEN')
#         )
#         return jsonify(trade_to_dict(trade)), 201
#     except Exception as e:
#         return jsonify({'error': str(e)}), 500

@trade_bp.route('/', methods=['POST'], strict_slashes=False)
def create_trade():
    """Create a new trade"""
    try:
        data = request.get_json()
        print(data)
        trade = BotProcess.process(data)
        print(trade)
        return jsonify(trade_to_dict(trade)), 201
    except Exception as e:
        return jsonify({'error': str(e)}), 500



@trade_bp.route('/<int:trade_id>', methods=['GET'])
def get_trade(trade_id: int):
    """Get a single trade by ID"""
    trade = TradeCRUD.get_trade_by_id(trade_id)
    if not trade:
        return jsonify({'error': 'Trade not found'}), 404
    return jsonify(trade_to_dict(trade))

@trade_bp.route('/', methods=['GET'])
def get_all_trades():
    """Get all trades with optional filtering"""
    filters = {}
    
    # Add filter parameters from query string
    if request.args.get('status'):
        filters['status'] = request.args.get('status')
    if request.args.get('exchange'):
        filters['exchange'] = request.args.get('exchange')
    # if request.args.get('instrument_name'):
    #     filters['instrument_name'] = request.args.get('instrument_name')
    
    limit = int(request.args.get('limit')) if request.args.get('limit') else None
    
    trades = TradeCRUD.get_all_trades(filters=filters, limit=limit)
    return jsonify([trade_to_dict(trade) for trade in trades])



@trade_bp.route('/<int:trade_id>', methods=['PUT'])
def update_trade(trade_id: int):
    """Update trade fields"""
    data: Dict[str, Any] = request.get_json()
    
    if not data:
        return jsonify({'error': 'No data provided'}), 400
    
    try:
        trade = TradeCRUD.update_trade(trade_id, data)
        if not trade:
            return jsonify({'error': 'Trade not found'}), 404
        return jsonify(trade_to_dict(trade))
    except Exception as e:
        return jsonify({'error': str(e)}), 500

@trade_bp.route('/<int:trade_id>', methods=['DELETE'])
def delete_trade(trade_id: int):
    """Delete a trade"""
    success = TradeCRUD.delete_trade(trade_id)
    if not success:
        return jsonify({'error': 'Trade not found'}), 404
    return jsonify({'message': 'Trade deleted successfully'}), 200

@trade_bp.route('/<int:trade_id>/close', methods=['POST'])
def close_trade(trade_id: int):
    """Close a trade"""
    data: Dict[str, Any] = request.get_json()
    
    if not data or 'exit_price' not in data:
        return jsonify({'error': 'exit_price is required'}), 400
    
    try:
        trade = TradeCRUD.close_trade(
            trade_id=trade_id,
            exit_price=float(data['exit_price']),
            pnl=float(data.get('pnl')) if data.get('pnl') else None
        )
        if not trade:
            return jsonify({'error': 'Trade not found'}), 404
        return jsonify(trade_to_dict(trade))
    except Exception as e:
        return jsonify({'error': str(e)}), 500

@trade_bp.route('/open', methods=['GET'])
def get_open_trades():
    """Get all open trades"""
    trades = TradeCRUD.get_open_trades()
    return jsonify([trade_to_dict(trade) for trade in trades])

@trade_bp.route('/bulk', methods=['POST'])
def bulk_create_trades():
    """Create multiple trades at once"""
    data: List[Dict[str, Any]] = request.get_json()
    
    if not isinstance(data, list):
        return jsonify({'error': 'Expected an array of trades'}), 400
    
    required_fields = ['instrument_name', 'exchange', 'entry_price', 'quantity']
    for trade_data in data:
        if not all(field in trade_data for field in required_fields):
            return jsonify({'error': 'Missing required fields in one or more trades'}), 400
    
    try:
        trades = TradeCRUD.bulk_create_trades(data)
        return jsonify([trade_to_dict(trade) for trade in trades]), 201
    except Exception as e:
        return jsonify({'error': str(e)}), 500

def trade_to_dict(trade) -> Dict[str, Any]:
    """Convert Trade object to dictionary"""
    return {
        'id': trade.id,
        'instrument_name': trade.instrument_name,
        'exchange': trade.exchange,
        'entry_price': trade.entry_price,
        'exit_price': trade.exit_price,
        'quantity': trade.quantity,
        'pnl': trade.pnl,
        'status': trade.status,
        'created_at': trade.created_at.isoformat() if trade.created_at else None,
        'updated_at': trade.updated_at.isoformat() if trade.updated_at else None
    }