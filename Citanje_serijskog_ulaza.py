import serial
import time

#if __name__ == '_main__':
ser = serial.Serial('/dev/ttyUSB0', 115200, timeout = 1)
time.sleep(3)
ser.reset_input_buffer()
print("Serial OK")

try:
	while True:
		time.sleep(0.01)
		if ser.in_waiting > 0:
			line = ser.readline().decode('utf-8').rstrip()
			print(line)
except KeyboardInterrupt:
	print("Close Serial Communication.")		
	ser.close()
