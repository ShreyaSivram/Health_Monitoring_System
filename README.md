# Health_Monitoring_System
This project integrates multiple environmental and health sensors with an ESP32 microcontroller to create a web server that displays real-time data. The sensors used include a DHT11 for temperature and humidity measurements, a DS18B20 for temperature readings, and an analog heartbeat sensor for monitoring heart rate. The ESP32 connects to a Wi-Fi network, enabling the hosted web server to be accessed remotely. The web interface provides a user-friendly display of the sensor data and dynamically adjusts the background color and weather icons based on the temperature and humidity conditions. This project demonstrates the potential of combining multiple sensors and IoT technology to monitor environmental and health parameters in real-time, offering a versatile solution for applications in home automation, weather monitoring, and personal health tracking.

Connect the circuit in the following way:
ESP32                   Breadboard
D18-                      DHT signal
3.3v-			    DHT anode
GND-	             DHT GND
3.3v-			    DS18B20 anode
GND-  		    DS18B20 GND
D5- 			    DS18B20 signal
4.7k resistor-       Between DS18B20 
			    anode and signal
D34-                      Heartbeat signal
3.3v- 		    Heartbeat anode
GND-		    Heartbeat signal

Connect the circuit and upload the code into the ESP32 board.
After uploading it successfully, check the serial monitor for the IP address to access the HTML page.
Copy this address and paste it into any internet browser to view the HTML page displaying all the information.
And thus, your own health monitoring system connected through a Web server is ready.



