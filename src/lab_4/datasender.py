import paho.mqtt.client as mqtt
import serial, json

ser = serial.Serial(port = '/dev/ttyACM0', baudrate=115200, timeout=1) 

def on_connect(client, userdata, flags, rc):
    if rc == 0:
        client.connected = True
        print("Connected OK")
    else: 
        print("Bad connection, returned code: ", rc)
        client.loop_stop()
def on_disconnect(client, userdata, rc):
    if(rc == 0):
        print("Client disconneted OK")
    else:
        print("System disconnected via code: ", rc)

client = mqtt.Client("B80835")
client.connected = False
client.on_connect = on_connect
client.on_disconnect = on_disconnect

broker ="iot.eie.ucr.ac.cr"
port = 1883
topic = "v1/devices/me/telemetry"
device = "znHTWqolDS2o8oq2OMEC"
client.username_pw_set(device)
client.connect(broker, port)

dict = dict()
line = []
str = []

while True:
    for c in ser.read().decode('utf-8').replace('\r', ""):
        if c == '\n': # Detecta un fin de linea
            str.append("".join(line))

            dict["x"] = str[0]
            dict["y"] = str[1]
            dict["z"] = str[2]
            dict["Bateria"] = str[3]

            if(str[4] == "1"):
                alarm = "Bateria baja"
            else:
                alarm = "Bateria OK"

            dict["Alarma"] = alarm
            output = json.dumps(dict)
            print(output)
            
            client.publish(topic, output)
            client.loop()

            str = []
            line = []
        elif c == '\t':
            str.append("".join(line))
            line = []
        else:
            line.append(c)
