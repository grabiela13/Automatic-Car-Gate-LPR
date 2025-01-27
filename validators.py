import sqlite3
from datetime import datetime

# Check if the license plate format is valid (3 letters & 3 numbers)
def is_plate_valid(plate):
    if len(plate) != 6:
        return False
    letters = plate[:3]
    numbers = plate[3:]

    return letters.isalpha() and numbers.isdigit()

# Check if a car is allowed to enter 
def is_car_allowed(plate):
    # if not is_plate_valid(plate):
    #     return False # Not valid license plate
    
    conn = sqlite3.connect('database\cars.db')
    cursor = conn.cursor()

    cursor.execute('''SELECT start_time, end_time
                   FROM cars WHERE plate_number = ?''', 
                   (plate,))
    
    result = cursor.fetchone()
    conn.close()

    if result:
        start_time, end_time = result

        if start_time is None and end_time is None:
            return True # car allowed all day
        
        current_time = datetime.now().time()
        if start_time <= current_time and current_time <= end_time:
            return True # car is allowed now 
        
    return False # car is not allowed