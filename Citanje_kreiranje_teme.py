import paho.mqtt.client as mqtt
import time
import serial

def on_connect(client, userdata, flags, rc):
    print(f"Connected with result code {rc}")

client = mqtt.Client()
client.on_connect = on_connect
client.connect("broker.emqx.io", 1883, 60)

ser = serial.Serial('/dev/ttyUSB0', 115200, timeout = 1)
time.sleep(3)
ser.reset_input_buffer()
print("Serial OK")

try:
	while True:
		time.sleep(0.01)
		if ser.in_waiting > 0:
			line = ser.readline().decode('utf-8').rstrip()
			client.publish('raspberry/topic', payload=line, qos=0, retain=False)
			print(f"send {line} to raspberry/topic")
except KeyboardInterrupt:
	print("Close Serial Communication.")		
	ser.close()
   
client.loop_forever()
