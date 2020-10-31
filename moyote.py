# Raspberry pi mqtt subscriber
# sudo pip install paho-mqtt
# sudo apt install  mosquitto mosquitto-clients
# sudo systemctl enable mosquitto.service
# https://www.switchdoc.com/2018/02/tutorial-installing-and-testing-mosquitto-mqtt-on-raspberry-pi/

import paho.mqtt.client as mqtt

MQTT_ADDRESS = 'localhost'
#MQTT_USER = 'user'
#MQTT_PASSWORD = 'password'
MQTT_TOPIC = 'bedroom/+'

y = True

def on_connect(client, userdata, flags, rc):
    """ The callback for when the client receives a CONNACK response from the server."""
    #rint('Connected with result code ' + str(rc))
    client.subscribe(MQTT_TOPIC)
    print('Connected..')
    file1 = open("data.txt", "w")

def on_message(client, userdata, msg):
    global y
    global file1
    """The callback for when a PUBLISH message is received from the server."""
    #print(msg.topic + '= ' + str(msg.payload))
    msg1 = (str(msg.payload) + '| ')
    Line = [msg1]
    if y == True:
        file1 = open("data.txt", "w")
        file1.writelines(Line)
        y = False
    else:
        file1.writelines(Line)
        file1.close()
        #print('file closed')
        y = True


def main():
    client = mqtt.Client()
    #mqtt_client.username_pw_set(MQTT_USER, MQTT_PASSWORD)
    client.on_connect = on_connect
    client.on_message = on_message
    client.connect(MQTT_ADDRESS, 1883)
    client.loop_forever()
   

if __name__ == '__main__':
    print('MQTT to InfluxDB bridge')
    main()
