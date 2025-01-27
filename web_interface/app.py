from flask import Flask, request, render_template, redirect, url_for
import sqlite3

app = Flask(__name__)

# Check if the license plate format is valid (3 letters & 3 numbers)
''' !!! Ver si puedo importar de validators nomas, even tho 
está en otra carpeta '''
def is_plate_valid(plate):
    if len(plate) != 6:
        return False
    letters = plate[:3]
    numbers = plate[3:]

    return letters.isalpha() and numbers.isdigit()

# Display the form and the list of license plates
@app.route('/')
def index():
    conn = sqlite3.connect('../database/cars.db')
    cursor = conn.cursor()
    
    # Fetch all license plates
    cursor.execute("SELECT * FROM cars")
    plates = cursor.fetchall()
    
    # Get error or success message from query parameters
    error = request.args.get('error')
    success = request.args.get('success')
    
    return render_template('index.html', plates=plates, error=error, success=success)

@app.route('/add_plate', methods=['POST'])
def add_plate():
    plate_number = request.form['plate']
    limit_time = 'limit_time' in request.form  # Check if the checkbox was checked
    start_time = request.form.get('start_time') if limit_time else None
    end_time = request.form.get('end_time') if limit_time else None
    

    conn = sqlite3.connect('../database/cars.db')
    cursor = conn.cursor()
    cursor.execute("SELECT * FROM cars")
    plates = cursor.fetchall()

    # Check if the license plate format is valid
    if not is_plate_valid(plate_number):
        return redirect(url_for('index', error="Invalid license plate format."))
    
    # Add plate to the database with optional time limits
    try:
        cursor.execute("INSERT INTO cars (plate_number, start_time, end_time) VALUES (?, ?, ?)", 
                       (plate_number.upper(), start_time, end_time))
        conn.commit()
        success = "License plate added successfully."
    except sqlite3.IntegrityError:
        error = "This license plate already exists."
        conn.close()
        return redirect(url_for('index', error=error))

    conn.close()
    return redirect(url_for('index', success=success))

if __name__ == '__main__':
    app.run(debug=True)