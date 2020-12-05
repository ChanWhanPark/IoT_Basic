import paho.mqtt.client as mqtt
import json
from influxdb import InfluxDBClient

server = "54.87.52.22"

def on_connect(client, userdata, flags, rc):
    print("Connected with RC : " + str(rc))
    client.subscribe("cpu/2016146022/evt/light")


def on_message(client, userdata, msg):
    print(msg.topic + " " + msg.payload.decode('utf-8'))

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

    if float(msg.payload) > 150.0:
        client.publish("cpu/2016146022/cmd/relay", "ON") # Relay ON
    else:
        client.publish("cpu/2016146022/cmd/relay", "OFF") # Relay OFF


dbclient = InfluxDBClient(server, 8086, None, None, 'weatherDB2')
client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message
client.connect(server, 1883, 60)
client.loop_forever()