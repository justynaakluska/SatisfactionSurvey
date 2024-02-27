
The survey was conducted using buttons on a console, which operated with the ESP32. The data was stored in a database hosted on our Raspberry Pi. To communicate with the database, we sent our data over MQTT to our MQTT-broker. In our product, the Raspberry Pi functioned as the MQTT-broker, the host of our database, and the host of our webpage. The webpage was used as the visualizer for the collected satisfaction measurements data. Additionally, we provided our customers with a secure console by building our own private network with integrated firewalls.