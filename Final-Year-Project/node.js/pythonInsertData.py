#imports
import sqlite3
import sys
import time
import datetime
import serial

#constant variables
const_Sampletime = 1 #number of seconds between each sample

#setting up the serial port
ser = serial.Serial(
        port='/dev/ttyS0',
        baudrate = 9600,
        parity=serial.PARITY_NONE,
        stopbits=serial.STOPBITS_ONE,
        bytesize=serial.EIGHTBITS,
        timeout=1
        )

#function for adding data to the table
def add_data (dateTime, tempC, tempF):
	conn = sqlite3.connect('sensorData.db')
	curs = conn.cursor()
        curs.execute("INSERT INTO Treadings values((?), (?), (?))", (dateTime, tempC, tempF))
        conn.commit()
	print ("\nEntire database contents:\n")
        for row in curs.execute("SELECT * FROM Treadings"):
                print (row)
	conn.close()

#main loop for adding data
while True:
	for i in range (0, 3):
		#creating a timestamp
        	ts = time.time()
        	st = datetime.datetime.fromtimestamp(ts).strftime('%Y-%m-%d %H:%M:%S')
        	timeStamp = st
        	print timeStamp

		#getting the serial data in a readable format
        	print ser.readline()
		dataRaw = ser.readline()

		#cleaning the string
		print dataRaw
		dataRaw = dataRaw.strip()
		dataRaw = dataRaw.split(',')

		print dataRaw[0]
		print dataRaw[1]

		#adding the data to the database table and printing the table to console
		add_data(timeStamp, dataRaw[0], dataRaw[1])
		time.sleep(const_Sampletime)
