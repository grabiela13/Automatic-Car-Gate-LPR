import sqlite3

conn = sqlite3.connect('database/cars.db')
cursor = conn.cursor()

# Create table to store car plates
# start_time and end_time set to NULL means all day
cursor.execute('''CREATE TABLE IF NOT EXISTS cars (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    plate_number TEXT UNIQUE NOT NULL,
    start_time TIME, 
    end_time TIME
)''')


conn.commit()
conn.close()
