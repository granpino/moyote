# Moyote
Sample Arduino to Raspberry data transfer by MQTT.
!20201030.jpg
The idea of this project is to show one of the ways you can send data from an arduino to a Raspberry pi.
- First update the arduino file with your wifi credentials, and the IP of the Pi.
- Load the moyote.ino to the Wemos D1 or other arduino.
- On the Raspberry pi install the software as indicated.
```
sudo pip install paho-mqtt.

sudo apt install  mosquitto mosquitto-clients.

sudo systemctl enable mosquitto.service.
```
The moyote.py file retrieves the data from the arduino, and stores it to the data.txt.
The clock.py script uses pygame to display the temperature and humidity. The clock.py script
will load the moyote.py at start.
The arduino is connected to a DHT22 sensor at pin D4. To exit the script click at the center
of the screen or press ESC. 
