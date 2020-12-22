import paho.mqtt.client as mqtt
import json
from influxdb import InfluxDBClient

server = "54.87.52.22"
def on_connect(client, userdata, flags, rc):
    print("Connected with RC : " + str(rc))
    client.subscribe("cpu/Project/evt/Button")

def on_message(client, userdata, msg):
    print(msg.topic + " " + msg.payload.decode("utf-8"))

    json_body = [
    {
        "measurement" : msg.topic,
        "tags" : {
            "host" : "server01",
            "region" : "us-west"
        },
        "fields" : {
            "value" : msg.payload
        }
    }
    ]
    dbclient.write_points(json_body)

    if (int(msg.payload) == 1):
        client.publish("esp/Project/evt/Mode", "1")
    elif (int(msg.payload) == 2):
        client.publish("esp/Project/evt/Mode", "2")
    elif (int(msg.payload) == 3):
        client.publish("esp/Project/evt/Mode", "3")
    elif (int(msg.payload) == 4):
        client.publish("esp/Project/evt/Mode", "4")

dbclient = InfluxDBClient(server, 8086, None, None, 'FinalDB')
client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message
client.connect(server, 1883, 60)
client.loop_forever()