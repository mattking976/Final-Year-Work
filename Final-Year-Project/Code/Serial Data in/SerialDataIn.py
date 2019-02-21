import time
import datetime
import serial

ser = serial.Serial(
	port='/dev/ttyS0',
	baudrate = 9600,
	parity=serial.PARITY_NONE,
	stopbits=serial.STOPBITS_ONE,
	bytesize=serial.EIGHTBITS,
	timeout=1
	)
ts1 = time.time()
st1 = datetime.datetime.fromtimestamp(ts1).strftime('%Y-%m-%d')
timestamp1 = st1
brew_file = open("/home/pi/Documents/Brewfile" + timeStamp, "w")
brew_file.close()

while True:
	ts = time.time()
	st = datetime.datetime.fromtimestamp(ts).strftime('%Y-%m-%d %H:%M:%S')
	timeStamp = st
	print ser.readline()
	brew_file = open("/home/pi/Documents/Brewfile" + timeStamp, "w")
        brew_file.write(timeStamp)
	brew_file.write(ser.readline())
	brew_file.close()
	print(timeStamp)
	
