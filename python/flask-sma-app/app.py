from flask import Flask, render_template
from config import Config
from models.db import db
from models.trade import Trade  # ✅ Correct import
from api.v1.botServices.SmaCalculator import sma_routes
from api.v1.crud.trades import trade_bp 
from api.v1.crud.settings_api import settings_api
baseV1 = '/api/v1'

def create_app():
    app = Flask(__name__, instance_relative_config=True)
    app.config.from_object(Config)
    try:
        app.config.from_pyfile('config.py', silent=True)
    except Exception:
        pass

    # Initialize extensions
    # db.init_app(app)
    db.init_app(app)
    with app.app_context():
        try:
            db.create_all()  # This will create all missing tables
            print("✅ Tables created successfully or already exist.")
        except Exception as e:
            print(f"❌ Error creating tables: {e}")
    # Register blueprints
    app.register_blueprint(sma_routes, url_prefix=baseV1 + '/sma')
    app.register_blueprint(trade_bp, url_prefix=baseV1 + '/orders')
    app.register_blueprint(settings_api, url_prefix=baseV1 + '/settings')
    @app.route('/')
    def home():
        return render_template('index.html')
    @app.route('/history')
    def history():
        return render_template('history.html')
    # return app
    
    @app.route('/setting')
    def setting():
        return render_template('setting.html')
    return app

if __name__ == '__main__':
    app = create_app()
    app.run(debug=True)                                                                                                                                                                                                                                                                           