import paho.mqtt.client as mqtt
import serial, json

ser = serial.Serial(
    port='COM4',\
    baudrate=9600,\
    parity=serial.PARITY_NONE,\
    stopbits=serial.STOPBITS_ONE,\
    bytesize=serial.EIGHTBITS,\
    timeout=0\
    )

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

broker ="demo.thingsboard.io"
port = 1883
topic = "v1/devices/me/telemetry"
device = "5Veyd3ytK8BuTsCpVjFM"
client.username_pw_set(device)
client.connect(broker, port)

dict = dict()
line = []
str = []

while True:
    for c in ser.read().decode('utf-8').replace('\r', ""):
        if c == '\n': # Detecta un fin de linea
            str.append("".join(line))

            dict["humedad"] = str[0]
            dict["temperatura"] = str[1]
            dict["Ult_riego"] = str[2]
            
            output = json.dumps(dict)
            print(output)
            print(output)
            
            # Se envían los datos en formato JSON hacia ThingsBoard
            client.publish(topic, output)
            client.loop()

            str = []
            line = []
        elif c == '\t':
            str.append("".join(line))
            line = []
        else:
            line.append(c)