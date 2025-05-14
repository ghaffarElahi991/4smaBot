#!/usr/bin/env bash
set -e

# Root folder
mkdir -p flask-sma-app
cd flask-sma-app

# Core files
touch app.py config.py requirements.txt .env .gitignore README.md

# Instance config folder
mkdir -p instance
touch instance/config.py

# API package
mkdir -p api/services
touch api/__init__.py api/routes.py api/services/__init__.py api/services/sma_service.py

# Models package
mkdir -p models
touch models/__init__.py models/db.py

# Templates & static assets
mkdir -p templates static/css static/js
touch templates/index.html static/css/style.css static/js/main.js

# Fill .gitignore
cat > .gitignore << 'EOF'
__pycache__/
*.pyc
instance/
.env
EOF

# Fill requirements.txt
cat > requirements.txt << 'EOF'
Flask
Flask-SQLAlchemy
python-dotenv
pandas
EOF

echo "Project scaffold created under $(pwd)"
